<?hh
namespace Ninjack\Core;

use Ninjack\Core\Router\Router as Router;
use Ninjack\Core\Response as Response;
use Ninjack\Core\Session as Session;
use Ninjack\Core\Loader as Loader;
use Ninjack\Core\Autoloader as Autoloader;
use Ninjack\Core\Configuration as Configuration;
use Ninjack\Core\Database\DBConnector as DBConnector;
use Ninjack\Core\Exception\NoActionException as NoActionException;
use Ninjack\Core\Exception\CLIException as CLIException;
use Ninjack\Core\Server as Server;
use Ninjack\Core\Helper\File as File;
use Ninjack\Core\Event\EventManager as EventManager;
use Ninjack\Core\Event\EventListener as EventListener;

/**
 * The Ninjack Application singleton class. It handles the client request,
 * and generate the http response.
 *
 * @author Sulivan
 *
 */
class Application{

  /**
   * The name of the application
   */
   private string $name = "";

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
   * Events Manager
   */
   private EventManager $event_manager;

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

    $this->event_manager = new EventManager();

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
        $package_path = realpath(dirname($package_path));
      }
    }


    $application_name = basename($package_path).$application_name;

    $this->name = $application_name;

    Autoloader::add_scope(
      "Application\\".implode("\\", array_map(($package) ==> { return ucfirst($package); }, explode(".", $application_name))),
      ROOT
    );


    $extends = $this->configuration->get("extends");

    if($extends != null && $extends instanceof Vector){
      foreach ($extends as $application) {
        $path_parts = explode(DS, $application);
        $name = array_pop($path_parts);
        $parent_path = implode(DS, $path_parts).DS.str_replace(".", DS, $name);
        $this->parents[$name] = realpath($parent_path);
        $namespace = implode("\\", array_map(($name) ==> ucfirst($name),explode(".",$name)));
        Autoloader::add_scope("Application\\".$namespace, $parent_path);
      }
    }

    $this->load_events();

    $this->is_initialized = true;

    $this->event_manager->trigger("ninjack.core.application_initialized");
  }

  private function load_events() : void{
    $configuration = $this->loader->load_configuration("events.hh");

    $events = $configuration?->get("events");

    if($events !== null && $events instanceof Map){
      foreach ($events as $name => $callback) {
        $this->event_manager->on($name, new EventListener($callback));
      }
    }

  }

  public function get_event_manager() : EventManager{
    return $this->event_manager;
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

    $this->router->load();

    if(!$this->is_initialized){
      $this->initialize();
    }

    $uri = $this->request->get_uri();
    $route = $this->router->route($uri);

    if($route != null){

      $controller_name = $route->get_controller();
      $action_name = $route->get_action();
      $parameters = $route->get_vector_parameters();

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

  }

  /**
   * Run the application from the cli.
   */
  public function run_command() : void{

    if(!$this->is_initialized){
      $this->initialize();
    }

    if(!$this->is_cli()){
      throw new CLIException("Command can be run only on CLI");
    }
    $command_query = $this->get_cli_arguments()[1];
    $command_name = explode(":", $command_query)[0];
    $command_method = explode(":", $command_query)[1];

    //Command is like this :
    //hhvm ninja Class:method arg1 arg2
    $arguments = $this->cli_arguments->skip(2);

    $command = $this->loader->load_command($command_name);

    if($command != null){
      $command->go($command_method, $arguments);
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

  public function get_router() : Router{
    return $this->router;
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

    if($configuration !== null){

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
    * Returns the current server.
    *
    * @return the current server.
    */
    public function server() : Server{
     return $this->server;
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
   public function get_file_from_application(string $file) : ?string
   {

     $filepath = ROOT.$file;

     if(file_exists($filepath)){
       return $filepath;
     }

     return $this->get_file_from_parent_applications($file);
   }

   /**
    * Returns the absolute file path of a parent application.
    *
    * @param string $file the file to search.
    * @param string $from application name frow which to find the file
    *
    * @return ?string the absolute filepath of a parent application. Returns null
    * if the file does not exist in any parent application.
    */
   public function get_file_from_parent_applications(string $file, ?string $from = null) : ?string{

     if($from !== null && !$this->parents->containsKey($from) && $this->name !== $from){
        return null;
     }

     $applications_from = $this->parents->keys();
     if($from !== $this->name){
       $applications_from = $applications_from->takeWhile( $value ==> $value !== $from);
     }

     foreach($applications_from as $name){

       $filepath = $this->get_file_from_extended_application($file, $name);

       if(file_exists($filepath)){
         return $filepath;
       }
     }

     return null;


   }

   /**
    * Returns the absolute file path of one of parents application given by its name.
    *
    * @param string $file the file to search.
    * @param string $application_name the name of the application where the file is searched.
    *
    * @return ?string the absolute filepath of a parent application. Returns null
    * if the file does not exist in any parent application.
    */
   protected function get_file_from_extended_application(string $file, string $application_name) : ?string{

     if(!isset($this->parents[$application_name])){
       return null;
     }

     $filepath = $this->parents[$application_name].DIRECTORY_SEPARATOR.$file;

     if(file_exists($filepath)){
       return $filepath;
     }

     return null;

   }

   public function get_public_path() : string{
       return $this->server->get_root_path();
   }

   public function get_application_path() : string{
     if(!$this->is_cli()){
       return dirname($this->get_public_path());
     }
     else{
       //assume we passed by the ninja script to get here
       return dirname($this->get_file_from_application("/ninja"));
     }
   }

   public function get_assets_directories() : Vector<string>{
      $directories = Vector{};

      $directories[] = $this->get_application_path().DIRECTORY_SEPARATOR.Loader::ASSETS_PATH;

      foreach ($this->parents as $parent) {
        $directories[] = $parent.DIRECTORY_SEPARATOR.Loader::ASSETS_PATH;
      }

      return $directories;
   }

   public function get_applications_directories() : Vector<string>{
     $directories = Vector{};

     $directories->add($this->get_application_path());
     $directories->addAll($this->parents);

     return $directories;
   }

   public function get_relative_path(string $filename) : ?string{

     foreach ($this->get_applications_directories() as $dir) {
       $regex = "/^".preg_quote($dir, "/")."/";
       if(preg_match($regex, $filename)){
         $relative_path = preg_replace($regex, "", $filename);

         if(strlen($relative_path) && $relative_path[0] == DIRECTORY_SEPARATOR){
           $relative_path = substr($relative_path, 1);
         }
         return $relative_path;
       }
     }
     return null;
   }

   public function belong_to(string $filepath) : (?string, ?string){

     $filepath = File::realpath($filepath);

     if(preg_match("/^".preg_quote($this->get_application_path(), "/")."/", $filepath)){
       return tuple(
        $this->name,
        preg_replace("/^".preg_quote($this->get_application_path().DIRECTORY_SEPARATOR, "/")."?/", "", $filepath)
      );
     }

     $ordered_parents = $this->parents;
     uasort($ordered_parents, ($a, $b) ==> {return count(explode($a, DIRECTORY_SEPARATOR)) < count(explode($b, DIRECTORY_SEPARATOR));});


     foreach ($ordered_parents as $name => $application_path) {

       if(preg_match("/^".preg_quote($application_path, "/")."./", $filepath)){
         return tuple($name, preg_replace("/^".preg_quote($application_path.DIRECTORY_SEPARATOR, "/")."?/", "", $filepath,));
       }

     }

     return tuple(null, null);
   }

}
