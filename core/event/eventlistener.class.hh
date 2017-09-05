<?hh
namespace Ninjack\Core\Event;
use Ninjack\Core\Event\Event;

/**
 * Event listener.
 *
 * @author Sulivan
 */
class EventListener {

  /**
   * Call back with an event object in parameter calling when
   * the listened Event is triggered.
   */
  private (function(Event): void) $callback;

  /**
   * Constructor.
   *
   * @param the event callback.
   */
  public function __construct((function(Event): void) $callback) {
    $this->callback = $callback;
  }


  /**
   * Trigger an event, and execute the callback.
   *
   * @param $event event triggered.
   */
  public function trigger(Event $event) : void{
    $callback = $this->callback;
    $callback($event);
  }

}
