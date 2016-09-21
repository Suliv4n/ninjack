<?hh
namespace Ninjack\Core\Exception;
use Ninjack\Core\Exception\NinjackException as NinjackException;


class CLIException extends NinjackException{


  public function __construct($message){
    parent::__construct($message);
  }
}
