<?hh

namespace ninjack\core;

class Loader{

  public static function load_configuration(string $file) : Configuration{
    return Configuration::load($file);
  }

  public static function load_controller(string $controller) : ?Controller{
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

  public static function load_view(string $file) : string{
    $path = VIEW_PATH.$file.".hh";


    ob_start();
    include $path;
    $content = ob_get_contents();
    ob_end_clean();

    return $content;
  }

}
