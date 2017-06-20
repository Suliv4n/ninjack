<?hh
namespace Ninjack\Core\Router;

use Ninjack\Core\Router\Parameter;

class Route{

  public function __construct(
    private string $uri_pattern,
    private string $controller,
    private string $action,
    private string $build_pattern = "",
    private Vector<Parameter> $parameters = Vector{}
  ){
  }

  public function get_uri_pattern() : string{
    return $this->uri_pattern;
  }

  public function get_controller() : string {
    return $this->controller;
  }

  public function get_action() : string {
    return $this->action;
  }

  public function get_parameters() : Vector<Parameter> {
    return $this->parameters;
  }

  public function get_vector_parameters() : Vector<string> {
    $parameters = Vector{};

    foreach ($this->parameters as $parameter) {
      $parameters[] = $parameter->get_value();
    }

    return $parameters;
  }

}
