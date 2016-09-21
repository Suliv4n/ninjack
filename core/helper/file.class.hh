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
}
