<?hh

namespace ninjack\core;

class Loader{

  public function __construct(){

  }

  public static function load_configuration($file) : Configuration{
    return Configuration::load($file);
  }

  public static function load_controller($controller) : ?Controller{
    $path = CONTROLLER_PATH.strtolower($controller).".hh";


    include_once $path;


    $reflector = new \ReflectionClass($controller);

    if(class_exists($controller, false)){
      $obj = $reflector->newInstance();

      if($obj instanceof Controller){
        return $obj;
      }
    }

    return null;

  }

}
