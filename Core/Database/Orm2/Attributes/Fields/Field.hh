<?hh
namespace Ninjack\Core\Database\Orm2\Attributes\Fields;

class Field implements \HH\ParameterAttribute, \HH\InstancePropertyAttribute
{

  <<__LateInit>>
  private string $property;
  private ?\ReflectionMethod $setter;

  public function set_property(\ReflectionProperty $property) : void
  {
    $this->property = $property;
  }

  public function set_setter(?\ReflectionMethod $setter) : void
  {
    $this->setter = $setter;
  }

  public function get_field_name() : string
  {
    return $this->property->getName();
  }

}
