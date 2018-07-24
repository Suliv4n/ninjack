<?hh
namespace Ninjack\Core\Exception;
use Ninjack\Core\Exception\NinjackException as NinjackException;


/**
 * Exceptions that could be thrown by CLI manager.
 */
class CLIException extends NinjackException{

  /**
   * Constructor.
   *
   * @param string $message The message of the exception.
   */
  public function __construct($message){
    parent::__construct($message);
  }
}
