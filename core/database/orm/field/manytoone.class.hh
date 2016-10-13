<?hh
namespace Ninjack\Core\Database\Orm\Field;
use Ninjack\Core\Database\Entity as Entity;
use Ninjack\Core\Database\Orm\Field\Field as Field;
use Ninjack\Core\TypeHelper as TypeHelper;
use Ninjack\Core\Database\Where as Where;

class ManyToOne extends Field{

  private string $source_column;

  public function __construct(string $source, string $target){
    parent::__construct($target);
    $this->source_column = $source;
  }

  <<__Override>>
  public function set(Entity $entity, mixed $value) : void{

      if($this->setter != null){
        $type = $this->setter->getParameters()[0]->getType();

        if($type == null){
          throw new Exception("No type in parameter of setter ".$this->name);
        }

        $generic = TypeHelper::get_generic_type($type);
        $type_str = TypeHelper::get_str_type($type);

        $target_class = new \ReflectionClass($type_str);

        if($generic == null){
          throw new Exception("No generic : ".$this->name);
        }


        $values = Entity::get($generic, Vector{
          new Where($this->name, $value ),
        });

        if($target_class->implementsInterface("Indexish")){
          $collection = $target_class->newInstanceWithoutConstructor();

          $entities = Entity::get($generic, Vector{
            new Where($this->name, $value)
          });

          foreach ($entities as $value) {
            $collection[] = $value;
          }

          if($this->setter != null){
            $this->setter->invoke($entity, $entities);
          }
        }

      }

  }

  <<__Override>>
  public function get_source_column() : string{
    return $this->source_column;
  }


}
