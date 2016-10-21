<?hh
namespace Ninjack\Core\Database\Orm\Field;
use Ninjack\Core\Database\Entity as Entity;
use Ninjack\Core\TypeHelper as TypeHelper;

/**
 * Database data field.
 *
 * @author Sulivan.
 */
class Field{

  /**
   * Name of the field.
   */
  protected string $name;

  /**
   * The setter method.
   */
  protected ?\ReflectionMethod $setter;

  /**
   * The getter method.
   */
  protected ?\ReflectionMethod $getter;

  /**
   * True if the field is a primary key.
   */
  protected bool $primary_key;

  /**
   * The constructor.
   */
  public function __construct(string $name, bool $primary_key = false){
    $this->name = $name;
    $this->primary_key = $primary_key;
  }

  public function set_setter(\ReflectionMethod $setter) : void{
    $this->setter = $setter;
  }

  public function set_getter(\ReflectionMethod $getter) : void{
    $this->getter = $getter;
  }

  public function get(Entity $entity) : mixed {
    if($this->getter != null){
      return $this->getter->invoke($entity);
    }
    return null;
  }

  public function set(Entity $entity, mixed $value) : void{
    if($this->setter != null){
      $value = TypeHelper::bind_parameter_value_type($this->setter->getParameters()[0], $value);
      $this->setter?->invoke($entity, $value);
    }
  }

  <<__Memoize>>
  public function get_choices() : Map<mixed,mixed> {
    $choices = Map{};

    if($this->setter != null){
      $class_target = $this->setter->getParameters()[0]->getType();


      if($class_target == null){
        throw new Exception("Return type is null.");
      }

      $generic = TypeHelper::get_generic_type($class_target);

      if($generic != null){
        $entities = Entity::get($generic);
      }
      else{
        $entities = Entity::get($class_target->__toString());
      }

      foreach ($entities as $entity){
        $choices[strval($entity->get_primary_key_value())] = $entity->__toString();
      }

    }
    return $choices;
  }

  public function get_name() : string{
    return $this->name;
  }

  public function get_source_column() : string{
    return $this->name;
  }

  public function is_primary_key() : bool{
    return $this->primary_key;
  }

  public function set_primary_key(bool $primary_key) : void{
    $this->primary_key = $primary_key;
  }

}
