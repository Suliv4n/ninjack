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

  public function cleaned_parameters(string $action, Vector<string> $parameters) : Vector<mixed>{
    $cleaned = new Vector(null);
    if(method_exists($this, $action)){
      $method = new \ReflectionMethod($this, $action);
      $method_parameters = $method->getParameters();
      foreach($method_parameters as $i => $parameter){
        if(isset($parameters[$i])){
          $parameter_type = $parameter->getType();
          if($parameter_type != null){
            //autre ou type string

            $target_type = preg_replace("~^HH\\\\~", "", $parameter_type->__toString());

            $target_parameter = $parameters->get($i);

            switch ($target_type) {
              case "boolean":
              case "bool":
                $target_parameter = boolval($target_parameter);
                break;
              case "int":
              case "integer":
                $target_parameter = intval($target_parameter);
                break;
              case "float":
                $target_parameter = floatval($target_parameter);
                break;
              case "double":
                $target_parameter = doubleval($target_parameter);
                break;

            }

            $cleaned->add($target_parameter);

          }
          else{
            $cleaned->add($parameters->get($i));
          }
        }
        else{
          //@todo error too few arguments
          break;
        }
      }
    }

    return $cleaned;
  }

}
