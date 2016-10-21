<?hh
namespace Ninjack\Core;
use Ninjack\Core\Application as Application;

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

  private ?string $theme = null;

  /**
   * The constructor.
   *
   * @param string $name the view name.
   */
  public function __construct(string $name, ?string $theme = null){
    $this->name = $name;
    $this->variables = new Map(null);
    $this->theme = $theme;
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
  public function get_render() : string{
    $view_file = Application::get_instance()->loader()->get_view_file($this->name, $this->theme);

    Application::get_instance()->loader()->load_all_widgets();

    ob_start();
    $variables = $this->variables->toArray();
    if(!$this->variables->isEmpty()){
      include ROOT."variables_loader.php";
    }
    include $view_file;
    $content = ob_get_contents();
    ob_end_clean();

    return $content;
  }

  public function asset(string $path, ?string $theme = null) : string{

    $url = Application::get_instance()->get_request()->get_base_url();

    if(strlen($path) > 0 && $path[0] != "/"){
      $path = "/".$path;
    }

    if($this->theme != null || $theme != null){
      $asset_path = THEME_PATH.($theme ?? $this->theme).DS."assets".$path;
      if(file_exists($asset_path)){
        return $url."/theme/".($theme ?? $this->theme)."/assets".$path;
      }

    }

    return $url."/assets".$path;
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
