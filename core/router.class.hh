<?hh
namespace Ninjack\Core;
use Ninjack\Core\Loader as Loader;

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
   * The action components container (controller, action, parameters).
   */
  private Map<string,string> $action_components;

  /**
   * The routed route rule.
   */
  private ?string $route;

  /**
   * The target action routed.
   */
  private ?string $target;

  /**
   * The constructor.
   */
  public function __construct(){
    $this->action_components = new Map(null);
  }

  /**
   * Route the requested uri.
   *
   * @param string $uri the uri to route.
   *
   * @return ?string the action (ie "Controller:action/param1/param2") matched or null if the uri match no route.
   */
  public function route($uri) : ?string{
    $this->configuration = Application::get_instance()->loader()->load_configuration("route.hh");

    $routes = $this->configuration?->get("routes");

    if($routes instanceof Map && $routes != null){
      foreach ($routes as $regex => $action) {
          $escaped = preg_replace("~/~", "\\/", $regex);

          if(preg_match("/^".$escaped."$/", $uri)){
            $this->route = $regex;
            $this->target = $action;
            $action = preg_replace("/^".$escaped."$/", $action, $uri);
            $this->action_components =  $this->get_action_components($action);
            $controller = Application::get_instance()->loader()->load_controller((string) $this->action_components["controller"]);
            //call_user_method_array((string) $this->action_components["action"], $controller , (array) $this->action_components["parameters"]);
            return $action;
          }
      }
    }
    else{
      //@todo throw exception
    }

    return null;
  }

  /**
   * Parses an action query (ie "Controller:action/param/param")
   * and returns a map that contains the components (controller, action ,parameters).
   *
   * @param string the action query string.
   *
   * @return Map<string, string> the components of the action.
   */
  private function get_action_components(string $action) : Map<string, string>{
    $components = new Map(null);

    $split = explode(":", $action);

    $components["controller"] = $split[0];

    $args = explode("/", $split[1]);

    $components["action"] = $args[0];

    $components["parameters"] = implode("/",array_slice($args, 1));

    return $components;
  }


  /**
   * Returns the routed action name.
   *
   * @return string the routed action name.
   */
  public function get_action() : string{
    return $this->action_components["action"];
  }

  /**
   * Returns the routed controller name.
   *
   * @return string the routed controller name.
   */
  public function get_controller() : string{
    return $this->action_components["controller"];
  }

  /**
   * Returns the routed action parameters string value.
   *
   * @return Vector<string> the routed action parameters string value.
   */
  public function get_parameters() : Vector<string>{
    return new Vector(explode("/", $this->action_components["parameters"]));
  }


  /**
   * Returns a tuple of two strings corresponding to the route rule that matched
   * the client request uri. The two strings are null if the uri didn't match any rule.
   * The first string of the tuple is the regex matched by the uri.
   * The second string is the action related to the match rule (ie : "Controler:action/param1/param2" ) .
   *
   * @return ?string  a tuple of two strings corresponding to the route rule.
   */
  public function get_rule() : (?string, ?string){
    return tuple($this->route, $this->target);
  }

}
