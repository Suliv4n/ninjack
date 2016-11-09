<?hh
namespace Ninjack\Core;

use Ninjack\Core\Response as Response;
use Ninjack\Core\Session as Session;
use Ninjack\Core\Loader as Loader;
use Ninjack\Core\Autoloader as Autoloader;
use Ninjack\Core\Configuration as Configuration;
use Ninjack\Core\Database\DBConnector as DBConnector;
use Ninjack\Core\Exception\NoActionException as NoActionException;
use Ninjack\Core\Exception\CLIException as CLIException;
use Ninjack\Core\Server as Server;

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

  private Configuration $configuration;

  private Vector<string> $cli_arguments;

  private Map<string,string> $parents = Map{};

  private bool $is_running = false;
  private bool $is_initialized = false;

  private Server $server;

  /**
    * The application constructor.
    */
  private function __construct(){
    $this->router = new Router();
    $this->request = new Request();
    $this->loader = Loader::get_instance();

    $this->session = Session::get_instance();

    $this->configuration = Configuration::load(ROOT.Loader::CONFIGURATION_PATH."application.hh");

    $this->cli_arguments = Vector{};

    $this->server = new Server();

  }

  private function initialize() : void {
    $space = $this->configuration->get_string("space", "");

    $application_name = "";
    $package_path = $this->get_application_path();

    foreach (explode(".", $space) as $package) {
      if(!empty($space)){
        $application_name = ".".$space;
        if(basename($package_path) != $space){
          die("error");
        }
        $package_path = dirname($package_path);
      }
    }


    $application_name = basename($package_path).$application_name;

    Autoloader::add_scope("Application\\".implode("\\", array_map(($package) ==> { return ucfirst($package); }, explode(".", $application_name))), ROOT);


    $extends = $this->configuration->get("extends");

    if($extends != null && $extends instanceof Vector){
      foreach ($extends as $application) {
        $path_parts = explode(DS, $application);
        $name = array_pop($path_parts);
        $parent_path = implode(DS, $path_parts).DS.str_replace(".", DS, $name);
        $this->parents[$name] = $parent_path;
        $namespace = implode("\\", array_map(($name) ==> ucfirst($name),explode(".",$name)));
        Autoloader::add_scope("Application\\".$namespace, $parent_path);
      }
    }



    $this->is_initialized = true;
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

    if(!$this->is_initialized){
      $this->initialize();
    }

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

  /**
   * Run the application from the cli.
   */
  public function run_command() : void{
    if(!$this->is_cli()){
      throw new CLIException("Command can be run only on CLI");
    }
    $command_query = $this->get_cli_arguments()[1];
    $command_name = explode(":", $command_query)[0];
    $command_method = explode(":", $command_query)[1];

    $command = $this->loader->load_command($command_name);
    if($command != null){
      //@todo arguments
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

  public function get_configuration() : Configuration{
    return $this->configuration;
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

   /**
    * Returns true if the application is ran via cli.
    *
    * @return true if the application is ran via cli, else false.
    */
   public function is_cli() : bool{
     return substr(PHP_SAPI, 0, 3) == "cli";
   }

   /**
    * Set the cli arguments if the application is ran via cli.
    *
    * @param Vector<string> $argv The cli arguments.
    */
   public function set_cli_arguments(Vector<string> $argv) : void{
     if($this->is_cli()){
       $this->cli_arguments = $argv;
     }
   }

   /**
    * Returns the cli arguments.
    *
    * @return Vector<string> the cli arguments.
    */
   public function get_cli_arguments() : Vector<string>{
     return $this->cli_arguments;
   }

   /**
    * Returns the absolute file path of given file in the application.
    * If the file was not found in the application, it will find and return the file in the parents
    * applications.
    *
    * @param string $file the file to search.
    *
    * @return ?string the absolute filepath of the application or parent applications. Returns null
    * if the file not exists in the application or parent applications.
    */
   public function get_file_from_application(string $file) : ?string{
     if(strlen($file) > 0 && $file[0] == DS){
       $file = substr($file, 1);
     }

     $filepath = ROOT.$file;

     if(file_exists($filepath)){
       return $filepath;
     }
     else{
       foreach($this->parents as $parent){
          //@todo parents of parent.
         $filepath = $parent;
         if(strlen($filepath) > 0 && $filepath[strlen($filepath)-1] != DS){
           $filepath .= DS;
         }
         $filepath .= $file;

         if(file_exists($filepath)){
           return $filepath;
         }
       }
     }

     return null;
   }

   public function get_public_path() : string{
     return $this->server->get_root_path();
   }

   public function get_application_path() : string{
     return dirname($this->get_public_path());
   }

   public function get_assets_directories() : Vector<string>{
      $directories = Vector{};

      $directories[] = $this->get_application_path().DIRECTORY_SEPARATOR.Loader::ASSETS_PATH;

      foreach ($this->parents as $parent) {
        $directories[] = $parent.DIRECTORY_SEPARATOR.Loader::ASSETS_PATH;
      }

      return $directories;
   }

}
