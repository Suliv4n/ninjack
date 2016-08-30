<?hh
namespace ninjack\core;

class Autoloader{

  public static function register(){
    spl_autoload_register(array(__CLASS__, 'autoload'));
  }

  public static function autoload($class){
    $parts = preg_split("#\\\#", $class);

    //@todo constant ?
    if($parts[0] == "ninjack"){
        array_shift($parts);
    }

    $className = array_pop($parts);

    $path = implode(DIRECTORY_SEPARATOR, $parts);
    $file = strtolower($className).".class.hh";

    $filepath = ROOT.$path.DIRECTORY_SEPARATOR.$file;

    if(file_exists($filepath))
      require $filepath;

  }
}
