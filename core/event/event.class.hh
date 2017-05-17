<?hh
namespace Ninjack\Core\Event;

class Event {

  private string $name;
  private Map<string, mixed> $parameters;

  public function __construct(string $name, Map<string, mixed> $parameters){
    $this->name = $name;
    $this->parameters = $parameters;
  }

  private function get_namespace(string $event_name) : string {
    $parts = explode(".", $event_name);

    return implode(".", array_slice($parts, 0, count($parts)));
  }

  public function get_name() : string{
    return $this->name;
  }

  public function get_short_name() : string{
    return array_pop(explode(".", $this->name));
  }

  public function get_parameter(string $name, mixed $default = null) : mixed {

    if($this->parameters->containsKey($name)){
      return $this->parameters[$name];
    }

    return $default;
  }



}
