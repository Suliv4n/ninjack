<?hh
namespace Ninjack\Core;

/**
 * This class autoload is used to autoload other classes.
 *
 * @author Sulivan
 */
class Autoloader{

  private static Map<string, string> $scopes = Map
  {
    "Ninjack" => NINJACK_PATH,
  };

  /*
   * Register the autoloader.
   */
  public static function register() : void{
    spl_autoload_register(array(__CLASS__, 'autoload'));
  }

  public static function add_scope(string $namespace, string $directory) : bool
  {
    if(self::$scopes->containsKey($namespace))
    {
      return false;
    }

    self::$scopes[$namespace] = $directory;

    return true;
  }

  public static function get_namespace_filepath(string $filepath) : string
  {

    if(!is_dir($filepath)){
      $filepath = dirname($filepath);
    }

    $scopes = self::$scopes->toArray();
    uasort(&$scopes, ($a, $b) ==> {return strlen($a) < strlen($b);});

    $namespace = "";
    foreach ($scopes as $ns => $path) {
      //var_dump($path, $filepath, preg_match("/^".preg_quote($path, "/")."/", $filepath));
      if(preg_match("/^".preg_quote($path, "/")."/", $filepath)){
        $namespace = $ns;
        $filepath = preg_replace("/^".preg_quote($path, "/")."/", "", $filepath);
        break;
      }
    }


    foreach (explode(DIRECTORY_SEPARATOR, $filepath) as $part) {
      if(strlen($part) > 0){
        $namespace .= "\\".ucfirst($part);
      }
    }

    return $namespace;

  }

  /**
   * Autoload a class which name is passed in parameter.
   *
   * @param string $class the class name.
   *
   */
  private function autoload(string $class) : void
  {
    $parts = explode("\\", $class);

    $scope = "";
    foreach (self::$scopes as $namespace => $path)
    {
      if (preg_match("/^" . preg_quote($namespace, "/") . "/", $class))
      {
        $scope = $path;
        $scopeParts = explode("\\", $namespace);

        array_splice(&$parts, 0, count($scopeParts));

        break;
      }
    }

    $className = array_pop(&$parts);

    $path = implode(DIRECTORY_SEPARATOR, $parts);

    $file = $className . ".hh";
    $filepath = $scope.$path.DIRECTORY_SEPARATOR.$file;

    if (file_exists($filepath))
    {
      require $filepath;
    }

  }
}
