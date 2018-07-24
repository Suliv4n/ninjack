<?hh
namespace Ninjack\Core\Exception;
use Ninjack\Core\Exception\NinjackException as NinjackException;

/**
 * The exception throwed when to controller or no action were found.
 *
 * @author Sulivan
 */
class NoActionException extends NinjackException{

  /**
   * The constructor.
   *
   * @param string $controller the controller name.
   * @param ?string $action the action name or null if just the controller was not found.
   */
  public function __construct(string $controller, ?string $action = null){
    $message = "";
    if($action == null){
      $message = "Controller ".$controller." not found.";
    }
    else{
      $message = "Action ".$action." not found in controller ".$controller.".";
    }

    parent::__construct($message);
  }
}
