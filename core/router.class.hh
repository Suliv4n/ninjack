<?hh
namespace ninjack\core;
use ninjack\core\Loader as Loader;

class Router {

  private Configuration $configuration;

  public function __construct(){
    $this->configuration = Loader::load_configuration("route.hh");
  }

  public function route($uri) : ?string{
    $routes = $this->configuration->get("routes");

    if($routes instanceof Map && $routes != null){
      foreach ($routes as $regex => $action) {
          $escaped = preg_replace("~/~", "\\/", $regex);

          if(preg_match("/^".$escaped."$/", $uri)){

            $action = preg_replace("/^".$escaped."$/", $action, $uri);

            $action_components =  $this->get_action_components($action);


            $controller = Loader::load_controller((string) $action_components["controller"]);

            call_user_method_array((string) $action_components["action"], $controller , (array) $action_components["parameters"]);

            return $action;
          }
      }
    }
    else{
      //@todo throw exception
    }
    return null;
  }

  private function get_action_components(string $action) : Map<string, mixed>{
    $components = new Map(null);

    $split = explode(":", $action);

    $components["controller"] = $split[0];

    $args = explode("/", $split[1]);

    $components["action"] = $args[0];

    $components["parameters"] = array_slice($args, 1);


    return $components;
  }


}
