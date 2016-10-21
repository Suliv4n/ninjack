<?hh
namespace Ninjack\Core;

/**
 * This class autoload is used to autoload other classes.
 *
 * @author Sulivan
 */
class Autoloader{

  private static Map<string, string> $scopes = Map{
    "Ninjack" => NINJACK_PATH,
  };

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
    $parts = preg_split("#\\\#", $class);

    //@todo constant ?
    $scope = "";
    if(self::$scopes->containsKey($parts[0])){
        $scope = self::$scopes[$parts[0]];
        array_shift($parts);
    }

    $className = array_pop($parts);

    $path = strtolower(implode(DIRECTORY_SEPARATOR, $parts));

    foreach (Vector{'class', 'enum', 'interface'} as $type) {

      $file = strtolower($className).".".$type.".hh";
      $filepath = $scope.$path.DIRECTORY_SEPARATOR.$file;

      if(file_exists($filepath)){
        require $filepath;
      }

    }

  }
}
