<?hh
namespace Ninjack\Core\Exception;
use Ninjack\Core\Exception\NinjackException as NinjackException;


/**
 * Exceptions throws when a required file is not found
 */
class FileNotFoundException extends NinjackException{

  /**
   * Constructor.
   *
   * @param string $message The message of the exception.
   */
  public function __construct($message){
    parent::__construct($message);
  }
}
