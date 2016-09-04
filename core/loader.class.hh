<?hh

namespace Ninjack\Core;

/**
 * A singleton class to load some component of the applciation (view, configuration, controller).
 *
 * @author Sulivan
 */
class Loader{

  /*
   * The unique instance of Loader.
   */
  private static ?Loader $instance;

  private function __construct(){

  }

  /**
   * Load a configuration file.
   *
   * @param string $file the configuration file name.
   *
   * @return Ninjack\Core\Configuration the configuration object loaded.
   */
  public function load_configuration(string $file) : Configuration{
    return Configuration::load($file);
  }

  /**
   * Load a controller of the application.
   *
   * @param string $conroller the controller name.
   *
   * @return ?Ninjack\Core\Controller the controller loaded or null if the controller
   * was not found.
   */
  public function load_controller(string $controller) : ?Controller{
    $path = CONTROLLER_PATH.strtolower($controller).".hh";


    include_once $path;


    if(class_exists($controller, false)){

      $reflector = new \ReflectionClass($controller);

      $obj = $reflector->newInstance();

      if($obj instanceof Controller){
        return $obj;
      }
    }

    return null;

  }

  /**
   * Load the content of a view and inject variables.
   *
   * @param string $file the file view file to load.
   * @param Map<string, mixed> the variables to inject to the view.
   *
   * @return the content generated.
   */
  public function load_view(string $file, Map $variables) : string{
    $path = VIEW_PATH.$file.".hh";

    if(!file_exists($path)){
      $path = CORE_VIEW_PATH.$file.".hh";
    }


    ob_start();
    $phpVariables = $variables->toArray();
    include ROOT."variables_loader.php";
    include $path;
    $content = ob_get_contents();
    ob_end_clean();

    return $content;
  }

  /**
   * Return the unique instance of Loader.
   *
   * @return Ninjack\Core\Loader the unique instance of Loader.
   */
  public static function get_instance() : Loader{
    if(self::$instance == null){
      self::$instance = new Loader();
    }
    return self::$instance;
  }

}
