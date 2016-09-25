<?hh
namespace Ninjack\Core;

use Ninjack\Core\Response as Response;
use Ninjack\Core\Session as Session;
use Ninjack\Core\Loader as Loader;
use Ninjack\Core\Database\DBConnector as DBConnector;
use Ninjack\Core\Exception\NoActionException as NoActionException;
use Ninjack\Core\Exception\CLIException as CLIException;

/**
 * The Ninjack Application singleton class. It handles the client request,
 * and generate the http response.
 *
 * @author Sulivan
 *
 */
class Application{

  /**
    * The client request handle.
    */
  private Request $request;

  /**
    * The application router.
    */
  private Router $router;

  /**
    * The application loader.
    */
  private Loader $loader;

  /**
    * The unique Application instance.
    */
  private static ?Application $instance;

  /**
   * The current session.
   */
   private Session $session;

   private Vector<string> $cli_arguments;

  /**
    * The application constructor.
    */
  private function __construct(){
    $this->router = new Router();
    $this->request = new Request();
    $this->loader = Loader::get_instance();

    $this->session = Session::get_instance();

    $this->cli_arguments = Vector{};
  }

  /**
    * Runs the application, route the client request
    * and execute the action congigured in config/route.hh.
    * The Application can run only once a session.
    *
    * @throws Ninjack\Core\NoActionException if the controller or the action was not found.
    *
    * @see Ninjack\Core\NoActionException
    */
  public function run() : void{
    //@todo no run more than one time
    $uri = $this->request->get_uri();
    $action = $this->router->route($uri);

    $controller_name = $this->router->get_controller();
    $action_name = $this->router->get_action();
    $parameters = $this->router->get_parameters();

    $controller = $this->load_controller($controller_name);

    if($controller != null){

        $controller->run($action_name, $parameters);

        $response = $controller->get_response();


        if($response != null){
          $response->render();
        }

    }
    else{
      throw new NoActionException($controller_name);
    }

  }

  public function run_command() : void{
    if(!$this->is_cli()){
      throw new CLIException("Command can be run only on CLI");
    }
    $command_query = $this->get_cli_arguments()[1];
    $command_name = explode(":", $command_query)[0];
    $command_method = explode(":", $command_query)[1];

    $command = $this->loader->load_command($command_name);
    if($command != null){
      //t@todo arguments
      $command->go($command_method, Vector{});
    }
    else{
      throw new CLIException("Command not found");
    }
  }

  /**
    * Returns the unique instance of Application.
    *
    * @return Ninjack\Core\Application the unique instance of Application.
    *
    */
  public static function get_instance() : Application{
    if(!isset(self::$instance)){
      self::$instance = new Application();
    }

    return self::$instance;
  }

  /**
   * Returns the client request.
   *
   * @see Ninjack\Core\Request
   *
   * @return Ninjack\Core\Request the client request.
   */
  public function get_request() : Request{
    return $this->request;
  }


  /**
   * Returns the controller by name or null
   * if the controller was not found.
   *
   * @param string $controller the controller name.
   *
   * @return ?Ninjack\Core\Controller the controller or null if it was not found.
   *
   */
  public function load_controller(string $controller) : ?Controller{
    return $this->loader->load_controller($controller);
  }


  /**
   * Return the controller name routed by the router or null if the router
   * was not lauched or didn't find the route.
   *
   * @return ?string the controller name routed by the router or null if the router
   * was not lauched or didn't find the route.
   */
  public function get_routed_controller() : ?string{
    return $this->router->get_controller();
  }

  /**
   * Return the action name routed by the router or null if the router
   * was not lauched or didn't find the route.
   *
   * @return ?string the controller name routed by the router or null if the router
   * was not lauched or didn't find the route.
   */
  public function get_routed_action() : string{
    return $this->router->get_action();
  }

  /**
   * Returns a tuple of two strings corresponding to the route rule that matched
   * the client request uri. The two strings are null if the uri didn't match any rule.
   * The first string of the tuple is the regex matched by the uri.
   * The second string is the action related to the match rule (ie : "Controler:action/param1/param2" ) .
   *
   * @see Ninjack\Core\Router::get_route_rule()
   *
   * @return ?string  a tuple of two strings corresponding to the route rule.
   */
  public function get_route_rule() : (?string, ?string){
    return $this->router->get_rule();
  }

  /**
   * Returns the loader of the application.
   *
   * @return Ninjack\Core\Loader the loader of the application.
   */
  public function loader() : Loader{
    return $this->loader;
  }

  /**
   * Load a database connection given by name.
   *
   * @param string $name the name of the database to load.
   *
   * @return Ninjack\Core\Database\Connector;
   *
   */
  public function load_database(string $name) : ?DBConnector{
    $configuration = $this->loader->load_configuration("databases.hh");

    $databases = $configuration->get("databases");

    if($databases instanceof Map){
      $configuration->get("databases");
      if(!empty($databases[$name]) && $databases[$name] instanceof Map){
        return new DBConnector(
          $databases[$name]["dbdriver"],
          $databases[$name]["hostname"],
          $databases[$name]["port"],
          $databases[$name]["username"],
          $databases[$name]["password"],
          $databases[$name]["database"],
          $databases[$name]["charset"],
        );
      }
    }

    return null;

  }

  /**
   * Returns the current session.
   *
   * @return the current session.
   */
   public function session() : Session{
    return $this->session;
   }

   public function is_cli() : bool{
     return substr(PHP_SAPI, 0, 3) == "cli";
   }

   public function set_cli_arguments(Vector<string> $argv) : void{
     if($this->is_cli()){
       $this->cli_arguments = $argv;
     }
   }

   public function get_cli_arguments() : Vector<string>{
     return $this->cli_arguments;
   }

}
