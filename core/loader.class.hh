<?hh

namespace Ninjack\Core;

class Loader{

  private static ?Loader $instance;

  private function __construct(){

  }

  public function load_configuration(string $file) : Configuration{
    return Configuration::load($file);
  }

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

  public static function get_instance() : Loader{
    if(self::$instance == null){
      self::$instance = new Loader();
    }
    return self::$instance;
  }

}
