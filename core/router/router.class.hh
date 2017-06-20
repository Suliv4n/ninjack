<?hh
namespace Ninjack\Core\Router;
use Ninjack\Core\Application as Application;
use Ninjack\Core\Loader as Loader;
use Ninjack\Core\Router\Route as Route;
use Ninjack\Core\Configuration as Configuration;

/**
 * Router to route the requested uri and find the action corrsponding to the uri.
 * The router configuration is the conf/route.hh file and must have a Map<string, string> $routes variable.
 *
 * @author Sulivan
 */
class Router {

  /**
   * The configuration of router.
   */
  private ?Configuration $configuration;

  /**
   * The routed route rule.
   */
  private ?Route $route;

  private Map<string, Route> $routes = Map{};


  /**
   * The constructor.
   */
  public function __construct(){
  }

  /**
   * Route the requested uri.
   *
   * @param string $uri the uri to route.
   *
   * @return ?string the action (ie "Controller:action/param1/param2") matched or null if the uri match no route.
   */
  public function route(string $uri) : ?Route{


    foreach ($this->routes as $name => $route) {
      $escaped = preg_replace("~/~", "\\/", $route->get_uri_pattern());

      $matches = Vector{};

      if(preg_match("/^".$escaped."$/", $uri, $matches)){

        $this->route = $route;

        foreach ($matches as $key => $value) {
          if(is_int($key)){
            $key = '$'.strval($key);
          }
          Application::get_instance()->get_request()->add_uri_parameters(strval($key), $value);
        }

        return $route;
      }

    }

    return null;

  }

  public function get_routed_route() : ?Route {
    return $this->route;
  }


  public function load() {
    $this->configuration = Application::get_instance()->loader()->load_configuration("route.hh");
    $routes = $this->configuration?->get("routes");

    if($routes !== null && $routes instanceof Map) {

      foreach ($routes as $name => $route) {

        if($route instanceof Route) {
          $this->routes[$name] = $route;
        }

      }

    }
  }

}
