<?hh
namespace Ninjack\Core;
use Ninjack\Core\TypeHelper as TypeHelper;
use Ninjack\Core\View as View;
use Ninjack\Core\Application as Application;
use Ninjack\Core\Form\Form as Form;
use Ninjack\Core\Response as Response;
use Ninjack\Core\Database\DBConnector as DBConnector;
use Ninjack\Core\Exception\NoActionException as NoActionException;



/**
 * The controller class.
 *
 * @author Sulivan
 */
class Controller{

  /**
   * The generated view.
   */
  private View $view;

  /**
   * True if the action requested is ran, else false.
   */
  private bool $ran = false;

  /**
   * The generated response.
   */
  private ?Response $response;

  private ?string $theme = null;

  protected Application $application;

  /**
   * Controller constructor.
   */
  public function __construct(){
    $this->view = new View("");
    $this->application = Application::get_instance();
  }

  /**
    * Run an action of the controller.
    * The action is a controller method.
    *
    * @param string $action the action name.
    * @param Vector<mixed> parameters to pass to the action.
    *
    * @throws Ninjack\Core\NoActionException if the action was not found.
    *
    */
  public final function run(string $action, Vector $parameters) : void{

    $class = new \ReflectionClass($this);
    $controller = strtolower(
      $class->getShortName()
    );

    $is_action = false;
    $exists = method_exists($this, $action);

    if($exists){
      $method = new \ReflectionMethod($this, $action);
      $is_action = isset($method->getAttributes()["Action"]);

      $theme = $this->theme ?? $this->application->get_configuration()->get_string("default_theme");


      $this->view = new View($controller."/".$action."/default",
        $theme
      );

      $parameters = $this->cleaned_parameters($action, $parameters);
      $this->response = new Response($this->view);
      call_user_method_array($action, $this, $parameters->toArray());
      $this->ran = true;
    }
    if(!$exists || !$is_action){
      throw new NoActionException($controller, $action);
    }

  }

  /**
   * Add a variable to the generated view.
   *
   * @param string $name the name of the variable.
   * @param mixed $value the value of the variable.
   */
  protected final function add_to_view(string $name, mixed $value) : void{
    $this->view->set_variables($name, $value);
  }

  /**
   * Returns the generated Response or null if no action was ran
   * by calling run() method.
   *
   * @return ?Ninjack\Core\Response the generated response or null if no action was ran.
   */
  public final function get_response() : ?Response{
    return $this->response;
  }

  public function get_database(string $name) : ?DBConnector{
    $connector = $this->application->load_database($name);
    if($connector != null){
      $connector->initialize();
      return $connector;
    }
    return null;
  }

  /**
   * Parse the parameters passed in parameters to match the arguments type required for a
   * specific action.
   *
   * Valid type are string, int, bool, float, and double. Other types can't be parsed.
   *
   * @param string $action the action name.
   * @param Vector<string> $parameters the parameters to clean.
   *
   * @return Vector<mixed> the cleaned parameters.
   */
  private final function cleaned_parameters(string $action, Vector<string> $parameters) : Vector<mixed>{
    $cleaned = new Vector(null);
    if(method_exists($this, $action)){
      $method = new \ReflectionMethod($this, $action);
      $method_parameters = $method->getParameters();
      foreach($method_parameters as $i => $parameter){
        if(isset($parameters[$i])){
          $cleaned->add(TypeHelper::bind_parameter_value_type($parameter, $parameters->get($i)));
        }
        else{
          //@todo error too few arguments
          break;
        }
      }
    }

    return $cleaned;
  }

  /**
   * Load a form given by name.
   *
   * @param string $name the name of the form to load.
   *
   * @return the generated Form.
   */
  public function load_form($name) : Form {
    return $this->application->loader()->load_form($name);
  }

}
