<?hh
namespace Ninjack\Core\Event;
use Ninjack\Core\Event\Event;

class EventListener {

  private (function(Event): void) $callback;

  public function __construct((function(Event): void) $callback) {
    $this->callback = $callback;
  }


  public function trigger(Event $event) : void{
    $callback = $this->callback;
    $callback($event);
  }

}
