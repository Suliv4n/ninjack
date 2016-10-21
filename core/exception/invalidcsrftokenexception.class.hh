<?hh
namespace Ninjack\Core\Exception;
use Ninjack\Core\Exception\NinjackException as NinjackException;

/**
 * Exception throws when a csrf token is not valid.
 */
class InvalidCSRFTokenException extends NinjackException{


  public function __construct(){
    $message = "Invalid csrf token value, or token expired.";

    parent::__construct($message);
  }
}
