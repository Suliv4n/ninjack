<?hh
namespace ninjack\core;

use ninjack\core\Response as Response;

class Application{
  private Request $request;
  private Router $router;

  private static ?Application $instance;

  private function __construct(){
    $this->router = new Router();
    $this->request = new Request();
  }

  public function run() : void{
    $uri = $this->request->get_uri();
    $action = $this->router->route($uri);

    $controller_name = $this->router->get_controller();
    $action_name = $this->router->get_action();

    $controller = $this->load_controller($controller_name);

    if($controller != null){
      if(method_exists($controller, $action_name)){
        call_user_method($action_name, $controller);
        $response = $controller->get_response();
        echo $response->get_render();
      }
    }
  }

  public static function get_instance() : Application{
    if(!isset(self::$instance)){
      self::$instance = new Application();
    }

    return self::$instance;
  }

  public function get_request() : Request{
    return $this->request;
  }

  public function load_controller(string $controller) : ?Controller{
    return Loader::load_controller($controller);
  }

  public function get_routed_controller() : string{
    return $this->router->get_controller();
  }

  public function get_routed_action() : string{
    return $this->router->get_action();
  }

}
