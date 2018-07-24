<?hh
namespace Ninjack\Core\Exception;


/**
 * Exception for ninjack Application.
 *
 * @author Sulivan
 */
class NinjackException extends \Exception{

  /**
   * The http code status of the response if the exception is not catch.
   */
  private int $http_code = 500;

  /**
   * The constructor
   * @param string $message the exception message.
   * @param int $http_code the http code status of the response if the exception is not catch by default 500.
   */
  public function __construct(string $message, int $http_code = 500){
    parent::__construct($message);
    $this->http_code = $http_code;
  }

  /**
   * Returns the http code status to response when the
   * exception is not catch.
   *
   * @return int the http code of the exception.
   */
  public function get_http_code() : int{
    return $this->http_code;
  }


}
