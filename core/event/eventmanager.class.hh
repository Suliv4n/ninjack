<?hh
namespace Ninjack\Core\Event;
use Ninjack\Core\Event\EventListner as EventListner;
use Ninjack\Core\Event\Event as Event;

class EventManager {

  private Map<string, EventListener> $event_listeners = Map{};

  public function __construct(){

  }

  public function on(string $event_name, EventListener $listener) : void{
    $this->event_listeners[$event_name] = $listener;
  }


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

  private function is_namespaced(string $event_name) : bool{
    return strpos($event_name, ".") !== false;
  }

  private function get_event_name(string $event_name) : string{
    $parts = explode(".", $event_name);

    return array_pop($parts);
  }

}
