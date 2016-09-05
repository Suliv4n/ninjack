<?hh
namespace Ninjack\Core;

/**
 * This class autoload is used to autoload other classes.
 *
 * @author Sulivan
 */
class Autoloader{

  /*
   * Registers the autoloader.
   */
  public static function register(){
    spl_autoload_register(array(__CLASS__, 'autoload'));
  }

  /**
   * Autoload a class which name is passed in parameter.
   *
   * @param string $class the class name.
   *
   */
  private static function autoload(string $class) : void{
    $parts = preg_split("#\\\#", strtolower($class));

    //@todo constant ?
    if($parts[0] == "ninjack"){
        array_shift($parts);
    }

    $className = array_pop($parts);

    $path = implode(DIRECTORY_SEPARATOR, $parts);

    foreach (Vector{'class', 'enum', 'interface'} as $type) {

      $file = strtolower($className).".".$type.".hh";
      $filepath = ROOT.$path.DIRECTORY_SEPARATOR.$file;


      if(file_exists($filepath)){
        require $filepath;
      }

    }

  }
}
