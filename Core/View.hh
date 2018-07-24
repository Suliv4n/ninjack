<?hh
namespace Ninjack\Core;
use Ninjack\Core\Application as Application;
use Ninjack\Core\Loader as Loader;
use Ninjack\Core\Helper\File as File;
use Ninjack\Core\Display\AssetsCompiler as AssetsCompiler;
use Ninjack\Core\Display\Media as Media;

/**
 * Class that represents view.
 *
 * @author Sulivan
 */
class View{

  /**
   * The view name.
   */
  private string $name;

  /**
   * The view variables.
   */
  private Map<string, mixed> $variables;

  private Map<string, AssetsCompiler> $assets_compilers = Map{};

  private ?string $theme = null;

  private Map<string, Media> $media = Map{};

  private ?Configuration $configuration;

  /**
   * The constructor.
   *
   * @param string $name the view name.
   */
  public function __construct(string $name, ?string $theme = null){
    $this->name = $name;
    $this->variables = new Map(null);
    $this->theme = $theme;

    $this->configuration = Application::get_instance()->loader()->load_configuration("view.hh");

    if($this->configuration !== null){

      $compilers = $this->configuration->get("assets_compilers");
      $media = $this->configuration?->get("media");

      if($compilers instanceof Map){
        $this->assets_compilers = $compilers;
        foreach($this->assets_compilers as $compiler){
  	        $compiler->initialize();
        }
      }

      if($media instanceof Map){
        $this->media = $media;
      }

    }

  }

  /**
   * The constructor.
   *
   * @param string $name the view name.
   */
  public function set_name(string $name) : void{
    $this->name = $name;
  }

  /**
   * Sets a view variable. if the variable already exists, erases it.
   *
   * @param string $name the variable name.
   * @param mixed the variable value.
   */
  public function set_variables(string $name, mixed $value) : void{
    $this->variables[$name] = $value;
  }

  /**
   * Sets view variables. If a variable already exists, erases it.
   *
   * @param Map<string, mixed> $variables a map of variables. The keys are variables name
   * and values variables values.
   */
  public function set_many_variables(Map $variables) : void{
    foreach ($variables as $name => $value) {
      $this->set_variables($name, $value);
    }
  }

  /**
   * Loads and returns the render.
   *
   * @return string the render.
   */
   <<__Memoize>>
  public function get_render() : string{
    $view_file = Application::get_instance()->loader()->get_view_file($this->name, $this->theme);

    Application::get_instance()->loader()->load_all_widgets();

    $auto_assets_deploy = $this->configuration?->get("auto_assets_deploy");

    if($auto_assets_deploy !== null && $auto_assets_deploy instanceof Vector){
      foreach ($auto_assets_deploy as $asset) {
        $asset_path = Application::get_instance()->get_file_from_application(Loader::ASSETS_PATH.$asset);
        if($asset_path !== null){
          $this->generate_asset($asset_path);
        }
      }
    }

    ob_start();
    $variables = $this->variables->toArray();
    if(!$this->variables->isEmpty()){
      include Application::get_instance()->get_application_path().DIRECTORY_SEPARATOR."variables_loader.php";
    }
    include $view_file;
    $content = ob_get_contents();
    ob_end_clean();

    return $content;
  }


  /**
   * Generates an asset and returns the XHPChild that calls the asset in the dom.
   *
   * @param string $path the path of the asset file to generate.
   * @param ?string $theme the theme to apply.
   *
   * @return \XHPChild The XHPChild that calls the asset in dom.
   */
  public function asset(string $path, ?string $theme = null) : \XHPChild{

    $url = Application::get_instance()->server()->get_root_url();

    if(strlen($path) > 0 && $path[0] == DIRECTORY_SEPARATOR){
      $path = substr($path, 1);
    }
    $path = Loader::ASSETS_PATH.$path;

    $path = $this->get_asset_absolute_path($path, $theme);

    if($path !== null){
      $public_path = $this->generate_asset($path);

      if($public_path == null){
        return "";
      }

      $relative_path = Application::get_instance()->get_relative_path($path);
      $uri = preg_replace("-".DIRECTORY_SEPARATOR."-", "/", $relative_path);
      $url = $url."/".$uri;

      $extension = pathinfo($public_path, PATHINFO_EXTENSION);
      switch($extension){
        case "js":
          $xhp = <script type="" src={$url} type="text/javascript"></script>;
          break;
        case "css":
        default:
          $url = File::change_extension($url, "css");
          $xhp = <link type="text/css" href={$url} media="screen" rel="stylesheet"></link>;
      }

      return $xhp;
    }

    return "";
  }

  /**
   * Returns the absolute path of the asset path in parameter.
   *
   * @param string The asset path.
   * @param ?sting $theme The theme to apply.
   *
   * @return string The absolute path of the asset path in parameter.
   */
  private function get_asset_absolute_path(string $path, ?string $theme = null) : ?string{
    if($this->theme != null || $theme != null){
      $asset_path = Application::get_instance()->get_file_from_application(
        Loader::THEME_PATH.($theme ?? $this->theme).DIRECTORY_SEPARATOR.$path
      );

      if(file_exists($asset_path)){
        return $asset_path;
      }
    }


    $asset_path = Application::get_instance()->get_file_from_application($path);

    return $asset_path;

  }

  /**
   * Generates an asset and put it in public asset directory.
   *
   * @param string $filepath the path of the file to generate.
   *
   * @return string The public path of asset generated.
   */
  private function generate_asset(string $filepath) : ?string{



    $extension = pathinfo($filepath, PATHINFO_EXTENSION);

    $content = $this->get_asset_content($filepath);

    $relative_path = Application::get_instance()->get_relative_path($filepath);
    $relative_path = preg_replace("/^".preg_quote(Loader::ASSETS_PATH, "/")."/", "", $relative_path);
    $public_assets_path = Application::get_instance()->get_public_path().DIRECTORY_SEPARATOR."generate".DIRECTORY_SEPARATOR.$relative_path;

    //when the file path is a dir we generate all files in it
    if(is_dir($filepath)){

      $files = File::scandir($filepath);

      if($files === null){
        return null;
      }

      foreach ($files as $file) {
        $this->generate_asset($file);
      }

      return $public_assets_path;
    }

    if(!file_exists(dirname($public_assets_path))){
      if(!mkdir(dirname($public_assets_path), 0755, true)){
        return null;
      }
    }

    if($this->assets_compilers->containsKey($extension)){
      $public_assets_path = File::change_extension($public_assets_path, $this->assets_compilers[$extension]->get_target_extension());
    }

    if($extension == "css" || ($this->assets_compilers->containsKey($extension) && $this->assets_compilers[$extension]->get_target_extension() == "css")){
      if(!empty($this->media)){
        foreach($this->media as $name => $media){
          $media_content = $this->get_asset_content(File::change_extension($filepath, $name.".".$extension));
          if($media_content != null){
            $content = $content.$media->wrap($media_content);
          }
        }
      }
    }

    $status = file_put_contents($public_assets_path, $content) !== false;

    return !$status ? null : $public_assets_path;
  }

  /**
   * Returns the content of the file which path is in parameter.
   * Eventually compiles it.
   *
   * @param string the filepath.
   *
   * @return ?string $filepath The file content or null if file doesn't exist.
   */
  public function get_asset_content(string $filepath) : ?string{
    $extension = pathinfo($filepath, PATHINFO_EXTENSION);

    if(!file_exists($filepath) || !is_file($filepath)){
      return null;
    }

    /*
    $relative_path = substr($filepath, strlen(Application::get_instance()->get_application_path().DIRECTORY_SEPARATOR));
    $relative_path = preg_replace("/^".preg_quote(Loader::ASSETS_PATH, "/")."/", "", $relative_path, 1);
    */



    if($this->assets_compilers->containsKey($extension)){
      $content = $this->assets_compilers[$extension]->compile($filepath);
    }
    else{
      $content = file_get_contents($filepath);
    }


    return $content;
  }


  /**
   * Prints the render.
   */
  public function render() : void{
    echo $this->get_render();
  }

  /**
   * Returns the theme name of the view or null if no theme is used.
   *
   * @return ?string the theme name of the view or null if no theme is used.
   */
  public function get_theme() : ?string {
    return $this->theme;
  }

}
