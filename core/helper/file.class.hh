<?hh
namespace Ninjack\Core\Helper;

/**
 * File helper class.
 *
 * @author Sulivan
 */
class File{

  /**
   * Returns all the absolute path of files in a given directory, expect the
   * the excluded file. If the dirctory doesn't or actually
   * isn't a directory, returns null.
   *
   * @param string path path of the directory.
   * @param Vector<string> $exclude the files to exclude, by default contains "."
   * and "..".
   *
   * @return the absolute files path of he directory or null if the directory
   * path is not valid.
   */
  public static function scandir(string $path, Vector<string> $exclude = Vector{".",".."}) : ?Vector<string>{
    $files = Vector{};
    if(file_exists($path) && is_dir($path)){
      $scan = scandir($path);
      foreach ($scan as $file) {
        if(!in_array($file, $exclude)){
          if($path[strlen($path)-1] == DS){
            $files->add($path.$file);
          }
          else{
            $files->add($path.DS.$file);
          }
        }
      }

      return $files;
    }

    return null;
  }

  public static function path_to_namespace(string $path) : string{

    $path = self::remove_root_path($path);

    $namespace_parts = Vector{};

    $directories = explode(DS, $path);
    foreach ($directories as $dir) {
      $namespace_parts->add(ucfirst($dir));
    }


    return implode("\\", $namespace_parts);
  }

  public static function remove_root_path(string $path) : string{
    $root_regex = "/^".preg_quote(ROOT, "/")."/";
    return preg_replace($root_regex, "", $path);
  }

  public static function is_absolute_path(string $path) : bool{
    return strlen($path) > 0 && $path[0] == "/";
  }

}
