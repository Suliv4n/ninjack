<?hh
namespace Ninjack\Core\Event;

/**
 * Represents an event.
 *
 * @author Sulivan.
 */
class Event {

  private string $name;
  private Map<string, mixed> $parameters;

  /**
   * The constructor.
   *
   * @param string $name The event name (can be namespaced by using dots like namepsace.name).
   * @param Map<string, mixed> $parameters The event parameters.
   */
  public function __construct(string $name, Map<string, mixed> $parameters){
    $this->name = $name;
    $this->parameters = $parameters;
  }

  /**
   * Extracts the nampespace of the event name in parameter.
   *
   * @param string $event_name the event name.
   *
   * @return string the event name.
   */
  private function get_namespace(string $event_name) : string {
    $parts = explode(".", $event_name);

    return implode(".", array_slice($parts, 0, count($parts)));
  }

  /**
   * Returns the name of the event.
   *
   * @return string The name of the event.
   */
  public function get_name() : string{
    return $this->name;
  }

  /**
   * Returns the name of the event without namespace.
   *
   * @return the name of the event without namepsace.
   */
  public function get_short_name() : string{
    return array_pop(explode(".", $this->name));
  }

  /**
   * Returns a parameter value form its name.
   * If the parameter name doesn't exists, returns the default value.
   *
   * @param $name the parameter name.
   * @param mixed $default the default value. (null by default)
   *
   * @return mixed The parameter value, or default value if
   */
  public function get_parameter(string $name, mixed $default = null) : mixed {

    if($this->parameters->containsKey($name)){
      return $this->parameters[$name];
    }

    return $default;
  }



}
