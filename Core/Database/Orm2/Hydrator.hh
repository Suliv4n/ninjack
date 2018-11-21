<?hh
namespace Ninjack\Core\Database\Orm2;

use Ninjack\Core\Database\Orm2\Attributes\Fields\Foreign;

class Hydrator
{
  public function hydrate(string $class_name, Map<string, mixed> $data) : mixed
  {
    $class_map = new ClassMap($class_name);
    $class_map->perform_map();

    $constructor_parameters = Vector{};

    foreach ($class_map->get_fields() as $field)
    {
      if ($data->containsKey($field->get_field_name()))
      {
        $field_value = $data[$field->get_field_name()];



        if ($field instanceof Foreign)
        {
          $target_classname = $field->get_target_classname();

          if (
            \get_class($field_value) !== $target_classname &&
            $field_value instanceof \Traversable
          )
          {
            $field_value = $this->hydrate($target_classname, $field_value);
          }
        }

        $constructor_parameters->add($field_value);
      }
      else
      {
        $constructor_parameters->add(null);
      }

    }

    $instance = $class_map->get_class_instance($constructor_parameters);

    return $instance;
  }
}
