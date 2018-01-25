<?hh

namespace Ninjack\Core;
use Ninjack\Core\View as View;
use Ninjack\Core\Controller as Controller;
use Ninjack\Core\Cli\Command as Command;
use Ninjack\Core\Form\Form as Form;
use Ninjack\Core\Helper\File as File;
use Ninjack\Core\Autoloader as Autoloader;
use Ninjack\Core\Application as Application;
use Ninjack\Core\Exception\FileNotFoundException as FileNotFoundException;
use Ninjack\Core\Exception\NinjackException as NinjackException;

/**
 * A singleton class to load some component of the applciation (view, configuration, controller).
 *
 * @author Sulivan
 */
class Loader{

  const string CONFIGURATION_PATH = "conf".DIRECTORY_SEPARATOR;
  const string THEME_PATH = "theme".DIRECTORY_SEPARATOR;
  const string FORM_PATH = "form".DIRECTORY_SEPARATOR;
  const string VIEW_PATH = "view".DIRECTORY_SEPARATOR;
  const string ASSETS_PATH = "assets".DIRECTORY_SEPARATOR;
  const string CORE_VIEW_PATH = CORE_PATH."view".DIRECTORY_SEPARATOR;
  const string CONTROLLER_PATH = "controller".DIRECTORY_SEPARATOR;
  const string XHP_PATH = "xhp".DIRECTORY_SEPARATOR;
  const string COMMAND_PATH = "command".DIRECTORY_SEPARATOR;

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
   * @throws Ninjack\Core\Exception\FileNotFoundException when no configuration file was found.
   *
   * @return Ninjack\Core\Configuration the configuration object loaded.
   */
  public function load_configuration(string $file) : ?Configuration{

    $filepath = Application::get_instance()->get_file_from_application(self::CONFIGURATION_PATH.$file);

    if($filepath === null){
      //throw new FileNotFoundException("Configuration file ". $file ." was not found.");
      return null;
    }

    return Configuration::load($filepath);
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
    $path = Application::get_instance()->get_file_from_application(self::CONTROLLER_PATH.strtolower($controller).".hh");

    if($path === null){
      throw new FileNotFoundException("Controller file '".$controller."' not found in application.");
    }

    $namespace = Autoloader::get_namespace_filepath($path);

    $controller = $namespace."\\".$controller;

    include_once $path;


    if(class_exists($controller, true)){

      $reflector = new \ReflectionClass($controller);

      $obj = $reflector->newInstance();

      if($obj instanceof Controller){
        return $obj;
      }
    }



    return null;

  }

  /**
   * Load a command given by name.
   * @param string $command the command name.
   *
   * @return ?Ninjack\Core\Cli\Command the command loaded, or null
   * if the command is not found.
   */
  public function load_command(string $command) : ?Command{
    $path = Application::get_instance()->get_file_from_application(self::COMMAND_PATH.strtolower($command).".hh");

    if($path === null){
      throw new NinjackException("Commande ".$command." could not be found.");
    }

    include_once $path;

    $namespace = Autoloader::get_namespace_filepath($path);
    $command = $namespace."\\".$command;

    //@todo namespace
    if(class_exists($command, false)){

      $reflector = new \ReflectionClass($command);

      $obj = $reflector->newInstance();

      if($obj instanceof Command){
        return $obj;
      }

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
  public function get_view_file(string $file, ?string $theme = null) : string{

    if($theme != null){
      $path = Application::get_instance()->get_file_from_application(self::THEME_PATH.self::VIEW_PATH.$file.".hh");

      if($path !== null){
        return $path;
      }
    }

    $path = Application::get_instance()->get_file_from_application(self::VIEW_PATH.$file.".hh");

    if($path == null){
      $path = self::CORE_VIEW_PATH.$file.".hh";
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
    foreach (Application::get_instance()->get_applications_directories() as $dir) {
      if(!$this->widgets_loaded){
        $this->load_widgets_dir($dir.DIRECTORY_SEPARATOR.Loader::XHP_PATH);
      }
    }
    $this->widgets_loaded = true;
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
    }
  }

  /**
   * Returns a form configuration absolute path given by name.
   *
   * @param string $name the name of the form to load.
   *
   * @return the configuration absolute path of the form configuration.
   */
  public function get_form_path(string $name) : ?string {
    return Application::get_instance()->get_file_from_application(self::FORM_PATH.$name.".form.hh");
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
   * @return Ninjack\Core\Loader the uniq instance of Loader.
   */
  public static function get_instance() : Loader{
    if(self::$instance == null){
      self::$instance = new Loader();
    }
    return self::$instance;
  }

}
