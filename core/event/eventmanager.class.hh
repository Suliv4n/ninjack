<?hh
namespace Ninjack\Core\Event;
use Ninjack\Core\Event\EventListner as EventListner;
use Ninjack\Core\Event\Event as Event;


/**
 * The event manager that handles event listeners.
 */
class EventManager {

  /**
   * The event listeners
   */
  private Map<string, EventListener> $event_listeners = Map{};

  /**
   * the constructor
   */
  public function __construct(){

  }

  /**
   * Add an event listener.
   *
   * @param string $event_name the name of the event to listen.
   * @param EventListener $listener the listener.
   */
  public function on(string $event_name, EventListener $listener) : void{
    $this->event_listeners[$event_name] = $listener;
  }


  /**
   * Trigger an event.
   *
   * @param string $event_name The name of the event to trigger.
   * @param Map<string, mixed> $parameters The parameters to pass to the event.
   */
  public function trigger(string $event_name, Map<string, mixed> $parameters = Map{}) : void{

    $listeners = Vector{};

    if($this->is_namespaced($event_name)){
      if($this->event_listeners->containsKey($event_name)){
        $listeners[] = $this->event_listeners[$event_name];
      }
    }
    else{

      foreach ($this->event_listeners as $full_name => $listener) {
        if($this->get_event_name($full_name) === $event_name){
          $listeners[] = $listener;
        }
      }

    }

    if($listeners->count() > 0){

      foreach ($listeners as $listener) {
        $event = new Event($event_name, $parameters);
        $listener->trigger($event);
      }

    }

  }


  /**
   * Tests if the event name in parameter  is namespaced.
   *
   * @param string $event_name the name to test.
   *
   * @return true if the event name is namepsaced, else false.
   */
  private function is_namespaced(string $event_name) : bool{
    return strpos($event_name, ".") !== false;
  }

  /**
   * Returns the name of the event without namespace.
   *
   * @param string $event_name the event name.

   * @return string The event name without namspace.
   */
  private function get_event_name(string $event_name) : string{
    $parts = explode(".", $event_name);

    return array_pop($parts);
  }

}
