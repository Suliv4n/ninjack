<?hh
namespace Ninjack\Core\Database\Orm2;

use Ninjack\Core\Database\Orm2\Attributes\Fields\Field;
use Ninjack\Core\Database\Orm2\Attributes\Fields\Id;

class ClassMap<T>
{
  private Vector<Field> $fields;

  public function __construct(
    private string $class_name
  )
  {
    $this->fields = new Vector();
  }

  public function perform_map() : void
  {
    $class_reflection = new \ReflectionClass($this->class_name);

    $constructor = $class_reflection->getConstructor();

    if ($constructor === null)
    {
      return;
    }

    $constructor_parameters = $constructor->getParameters();

    foreach ($constructor_parameters as $parameter)
    {
      $field = $parameter->getAttributeClass(Field::class);
      $field_id = $parameter->getAttributeClass(Id::class);

      if (($field ?? $field_id) !== null)
      {
        $this->parse_simple_field(($field ?? $field_id), $class_reflection, $parameter);
        $this->fields->add(($field ?? $field_id));
        continue;
      }
    }
  }

  private function parse_simple_field(Field $field, \ReflectionClass $entity_class, \ReflectionParameter $target_parameter) : void
  {
    $property_name = $target_parameter->getName();

    if (!$entity_class->hasProperty($property_name))
    {
      throw new \Exception("Invalid entity class. Parameter in constructor must be property of the entity class.");
    }

    $field->set_property($entity_class->getProperty($property_name));
    $setter = null;

    $setter_name = "set_" . \ucfirst($property_name);

    if ($entity_class->hasMethod($setter_name))
    {
      $setter = $entity_class->getMethod($setter_name);
    }


    $field->set_setter($setter);
  }

  public function get_fields() : Vector<Field>
  {
    return $this->fields;
  }

  public function get_class_instance(Vector<mixed> $constructor_parameters) : mixed
  {
    $class_reflection = new \ReflectionClass($this->class_name);

    return $class_reflection->newInstanceArgs($constructor_parameters);
  }

}
