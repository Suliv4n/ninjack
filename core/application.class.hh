<?hh
namespace ninjack\core;

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

    $split = explode(":",$action);

    $controller = $this->load_controller($split[0]);

    var_dump($controller);
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

}
