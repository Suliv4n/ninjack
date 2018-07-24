<?hh
namespace Ninjack\Core\Database\Orm\Field;
use Ninjack\Core\Database\Entity as Entity;
use Ninjack\Core\Database\SQLWhere as SQLWhere;
use Ninjack\Core\Database\Orm\Field\Field as Field;

class ForeignKey extends Field{

  private string $target_column;

  public function __construct(string $name, string $target_column, bool $primary_key){
    parent::__construct($name, $primary_key);
    $this->target_column = $target_column;
  }

  <<__Override>>
  public function set(Entity $entity, mixed $value) : void{

    if($this->setter != null){
      $class_target = $this->setter->getParameters()[0]->getType();

      if($class_target == null){
        throw new Exception("Target type is null.");
      }

      $values = Entity::get($class_target->__toString(), Vector{
        new SQLWhere($this->target_column, $value ),
      });

      if(count($values) > 0 && $this->setter != null){
        $this->setter->invoke($entity, $values[0]);
      }
    }
  }

}
