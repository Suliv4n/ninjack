<?hh
namespace Ninjack\Core\Display\AssetsCompiler;
use Ninjack\Core\Display\AssetsCompiler as AssetsCompiler;
use Ninjack\Core\Application as Application;



/**
 * Compile .less file into .css file.
 *
 * @author Sulivan.
 */
class LessCompiler implements AssetsCompiler{

  /**
   * Less compiler
   */
  private \lessc $less;

  /**
   * Constructor
   */
  public function __construct(){
    $this->less = new \lessc();
  }

  /**
   * Initializes the LessCompiler.
   */
  public function initialize() : void{
    $this->less->setImportDir(Application::get_instance()->get_assets_directories()->toArray());
  }

  /**
   * Compiles the content of the file which the filename is passed in argument
   * and contains less.
   *
   * @param string $filename the path of the file to compile.
   *
   * @return string the file compiled content.
   */
  public function compile(string $filename) : string{

    $assets_directories = Application::get_instance()->get_assets_directories()->toArray();


    $relative_path = "";

    foreach($assets_directories as $dir){
      $regex = "/^".preg_quote($dir, "/")."?/";
      if(preg_match($regex, $filename)){
        $relative_path = dirname(preg_replace($regex, "", $filename));
        break;
      }
    }

    $assets_directories = array_map(($dir) ==> { return $dir.$relative_path; }, $assets_directories);

    $this->less->setImportDir($assets_directories);

    $compiledCss = $this->less->compileFile($filename);

    $this->less = $this->less = new \lessc();

    return $compiledCss;
  }

  /**
   * Returns the target extension (always return "css").
   *
   * @return string "css"
   */
  public function get_target_extension() : string{
    return "css";
  }
}
