<?hh
namespace Ninjack\Core\Router;
use Ninjack\Core\Router\Parameter;

/**
 * Represents a route that can be handle by the router.
 *
 * @author Sulivan
 */
class Route{

  /**
   * The constructor/
   *
   * @param string $uri_pattern The regex pattern of the uri.
   * @param string $controller The controller corresponding to the route.
   * @param string $action The controller action corresponding to the route.
   * @param string $build_pattern The pattern using to build an absolute url.
   * @param Vector<Parameter> The route Parameters
   */
  public function __construct(
    private string $uri_pattern,
    private string $controller,
    private string $action,
    private string $build_pattern = "",
    private Vector<Parameter> $parameters = Vector{}
  ){
  }


  /**
   * Returns the pattern of the uri.
   *
   * @return string The pattern of the uri.
   */
  public function get_uri_pattern() : string{
    return $this->uri_pattern;
  }

  /**
   * Returns the route build pattern.
   *
   * @return string The route build pattern.
   */
  public function get_build_pattern() : string {
    return $this->build_pattern;
  }

  /**
   * Returns the controller corresponding to the route.
   *
   * @return string The controller corresponding to the route.
   */
  public function get_controller() : string {
    return $this->controller;
  }

  /**
   * Returns the controller action corresponding to the route.
   *
   * @return string The controller action corresponding to the route.
   */
  public function get_action() : string {
    return $this->action;
  }

  /**
   * Returns the route parameters.
   *
   * @return Vector<Parameter> The route parameters.
   */
  public function get_parameters() : Vector<Parameter> {
    return $this->parameters;
  }

  /**
   * Returns the route parameters values.
   *
   * @return Vector<string> The route parameters value.
   */
  public function get_vector_parameters() : Vector<string> {
    $parameters = Vector{};

    foreach ($this->parameters as $parameter) {
      $parameters[] = $parameter->get_value();
    }

    return $parameters;
  }

}
