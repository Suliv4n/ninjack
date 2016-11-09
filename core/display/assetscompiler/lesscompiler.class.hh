<?hh
namespace Ninjack\Core\Display\AssetsCompiler;
use Ninjack\Core\Display\AssetsCompiler as AssetsCompiler;
use Ninjack\Core\Application as Application;

class LessCompiler implements AssetsCompiler{
  private \lessc $less;

  public function __construct(){
    $this->less = new \lessc();
  }

  public function initialize() : void{
    $this->less->setImportDir(Application::get_instance()->get_assets_directories()->toArray());
  }

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

    return $this->less->compileFile($filename);
  }

  public function get_target_extension() : string{
    return "css";
  }
}
