<?hh
namespace Ninjack\Core;
use Ninjack\Core\View as View;
use Ninjack\Core\Response as Response;
use Ninjack\Core\Application as Application;
use Ninjack\Core\NoActionException as NoActionException;

class Controller{

  private View $view;
  private bool $ran = false;
  private ?Response $response;

  public function __construct(){
    $this->view = new View("");
  }

  public function run(string $action, Vector $parameters){

    $controller = strtolower(get_class($this));
    if(method_exists($this, $action)){
      $this->view = new View($controller."/".$action);
      $parameters = $this->cleaned_parameters($action, $parameters);
      $this->response = new Response($this->view);
      call_user_method_array($action, $this, $parameters->toArray());
      $this->ran = true;
    }
    else{
      throw NoActionException($controller, $action);
    }

  }



  protected function add_to_view(string $name, mixed $value) : void{
    $this->view->set_variables($name, $value);
  }

  public function get_response() : ?Response{
    return $this->response;
  }

  private function cleaned_parameters(string $action, Vector<string> $parameters) : Vector<mixed>{
    $cleaned = new Vector(null);
    if(method_exists($this, $action)){
      $method = new \ReflectionMethod($this, $action);
      $method_parameters = $method->getParameters();
      foreach($method_parameters as $i => $parameter){
        if(isset($parameters[$i])){
          $parameter_type = $parameter->getType();
          if($parameter_type != null){

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
