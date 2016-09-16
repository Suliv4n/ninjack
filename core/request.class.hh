<?hh

namespace Ninjack\Core;

/**
 * Represents a client Request.
 *
 * @author Sulivan
 */
class Request{

  /**
   * Constructor.
   */
  public function __construct(){

  }

  /**
   * Returns the requested uri.
   *
   * @return string requested uri.
   */
  public function get_uri() : string{
    return $_SERVER["REQUEST_URI"];
  }

  public function post(string $name, mixed $default = null) : mixed{
    return isset($_POST[$name]) ? $_POST[$name] : $default;
  }

}
