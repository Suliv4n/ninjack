<?hh

namespace Ninjack\Core;

/**
 * Represents a client Request.
 *
 * @author Sulivan
 */
class Request{

  private Map<string, string> $uri_parameters = Map{};

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
   * Returns the get  data given by name or the default value.
   *
   * @param string $name The name of the get data to return.
   * @param mixed default The default value to return if the
   * post data is not set.
   */
  public function get(string $name, mixed $default = null) : mixed{
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

  public function add_uri_parameters(string $name, string $value) : void{
    $this->uri_parameters[$name] = $value;
  }

  public function uri_parameter(string $name, ?string $default) : ?string{
    return isset($this->uri_parameters[$name]) ? $this->uri_parameters[$name] : $default;
  }

}
