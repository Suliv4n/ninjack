<?hh
namespace Ninjack\Core\Database\Orm2;

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
        $constructor_parameters->add($data[$field->get_field_name()]);
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
