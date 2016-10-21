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

  public function get_base_url() : string{
    return $_SERVER['REQUEST_SCHEME'].'://'.$_SERVER['SERVER_NAME'];
  }

  /**
   * Returns the post data given by name or the default value.
   *
   * @param string $name The name of the post data to return.
   * @param mixed default The default value to return if the
   * post data is not set.
   */
  public function post(string $name, mixed $default = null) : mixed{
    return isset($_POST[$name]) ? $_POST[$name] : $default;
  }

  /**
   * Returns true if the request is a POST method.
   *
   * @return true if the request is post, else false.
   */
  public function is_post() : bool{
    return $_SERVER['REQUEST_METHOD'] == 'POST';
  }

}
