<?hh

namespace Ninjack\Core;
use Ninjack\Core\View as View;
use Ninjack\Core\Helper\File as File;

/**
 * A singleton class to load some component of the applciation (view, configuration, controller).
 *
 * @author Sulivan
 */
class Loader{

  private bool $widgets_loaded = false;

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
  public function get_view_file(string $file) : string{
    $path = VIEW_PATH.$file.".hh";

    if(!file_exists($path)){
      $path = CORE_VIEW_PATH.$file.".hh";
    }

    return $path;
  }

    public function load_view(string $name) : View{
      return new View($name);
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

  public function load_all_widgets() : void{
    if(!$this->widgets_loaded){
      $this->load_widgets_dir(WIDGET_PATH);


    }
  }

  private function load_widgets_dir(string $path) : void{
    $files = File::scandir($path);

    if($files != null){
      foreach ($files as $file) {
        if(!is_dir($file)){
          if(preg_match("-\.xhp\.hh$-", $file)){
            include_once $file;
          }
        }
        else{
          $this->load_widgets_dir($file);
        }
      }
      $this->widgets_loaded = true;
    }
  }

}
