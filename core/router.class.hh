<?hh
namespace Ninjack\Core;
use Ninjack\Core\Loader as Loader;

class Router {

  private Configuration $configuration;
  private Map<string,string> $action_components;
  private string $route;
  private string $target;

  public function __construct(){
    $this->configuration = Loader::load_configuration("route.hh");
    $this->action_components = new Map(null);
    $this->route = "";
    $this->target = "";
  }

  public function route($uri) : ?string{
    $routes = $this->configuration->get("routes");

    if($routes instanceof Map && $routes != null){
      foreach ($routes as $regex => $action) {
          $escaped = preg_replace("~/~", "\\/", $regex);

          if(preg_match("/^".$escaped."$/", $uri)){
            $this->route = $regex;
            $this->target = $action;
            $action = preg_replace("/^".$escaped."$/", $action, $uri);
            $this->action_components =  $this->get_action_components($action);
            $controller = Loader::load_controller((string) $this->action_components["controller"]);
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

  private function get_action_components(string $action) : Map<string, string>{
    $components = new Map(null);

    $split = explode(":", $action);

    $components["controller"] = $split[0];

    $args = explode("/", $split[1]);

    $components["action"] = $args[0];

    $components["parameters"] = implode("/",array_slice($args, 1));


    return $components;
  }


  public function get_action() : string{
    return $this->action_components["action"];
  }

  public function get_controller() : string{
    return $this->action_components["controller"];
  }

  public function get_parameters() : Vector<string>{
    return new Vector(explode("/", $this->action_components["parameters"]));
  }



  public function get_rule() : (string, string){
    return tuple($this->route, $this->target);
  }

}
