<?hh

namespace Ninjack\Core;
use Ninjack\Core\View as View;
use Ninjack\Core\Controller as Controller;
use Ninjack\Core\Cli\Command as Command;
use Ninjack\Core\Form\Form as Form;
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

    //@todo namespace
    if(class_exists($controller, false)){

      $reflector = new \ReflectionClass($controller);

      $obj = $reflector->newInstance();

      if($obj instanceof Controller){
        return $obj;
      }
    }



    return null;

  }

  public function load_command(string $command) : ?Command{
    $path = COMMAND_PATH.strtolower($command).".hh";


    include_once $path;

    //@todo namespace
    if(class_exists($command, false)){

      $reflector = new \ReflectionClass($command);

      $obj = $reflector->newInstance();

      if($obj instanceof Command){
        return $obj;
      }
      die('here');
    }

    return null;

  }

  /**
   * Returns the  view absolute file path.
   *
   * @param string $file the name of the view file
   * (without extension)
   *
   * @return the  view absolute file path.
   */
  public function get_view_file(string $file) : string{
    $path = VIEW_PATH.$file.".hh";

    if(!file_exists($path)){
      $path = CORE_VIEW_PATH.$file.".hh";
    }

    return $path;
  }

  /**
   * Returns the view given by name.
   *
   * @param sting $name The name of the view to load.
   *
   * @return the loaded view.
   */
  public function load_view(string $name) : View{
    return new View($name);
  }

  /**
   * Load all the widgets.
   * Widgets are xhp elements so no namespace supported.
   */
  public function load_all_widgets() : void{
    if(!$this->widgets_loaded){
      $this->load_widgets_dir(WIDGET_PATH);
    }
  }

  /**
   * Load widgets in a given directory recursively.
   * Widgets are xhp elements so no namespace supported.
   *
   * @param string $path the directory to load.
   */
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

  /**
   * Returns a form configuration absolute path given by name.
   *
   * @param string $name the name of the form to load.
   *
   * @return the configuration absolute path of the form configuration.
   */
  public function get_form_path(string $name) : string {
    return FORM_PATH.$name.".form.hh";
  }

  /**
   * Load a form and its configuration given by name.
   *
   * @param string $name the name of the form to load.
   *
   * @return the generated forms.
   */
  public function load_form($name) : Form{
    $form = new Form();
    $form->load_form($name);

    return $form;
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
