<?hh
namespace ninjack\core;
use ninjack\core\Response as Response;
use ninjack\core\Application as Application;

class Controller{

  private string $viewname;
  private Map<string, mixed> $variables;

  public function __construct(){
    $this->variables = new Map(null);
    $this->viewname = "";
  }

  public function get_view_name() : string{
    if(empty($this->viewname)){

      $controller = Application::get_instance()->get_routed_controller();
      $action = Application::get_instance()->get_routed_action();

      $this->set_view_name($controller."/".$action);
    }
    return $this->viewname;
  }

  protected function set_view_name(string $name) : void{
    $this->viewname = trim(strtolower($name));
  }

  protected function add_to_view(string $name, mixed $value) : void{
    $this->variables[$name] = $value;
  }

  public function get_response() : Response{
    return new Response($this->get_view_name(), $this->variables);
  }

}
