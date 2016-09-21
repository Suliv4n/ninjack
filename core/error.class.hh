<?hh
namespace Ninjack\Core;
use Ninjack\Core\Loader as Loader;
use Ninjack\Core\Application as Application;

/**
 * Singleton class to handle exceptions.
 *
 * @author Sulivan
 */
class Error{

  /**
   * The unique instance of Error.
   */
  private static ?Error $instance;

  /**
   * The exceptions handler.
   *
   * @param \Exception $exception exception to handle.
   */
  public function handler(\Exception $exception) : void{
    $variables = Map{
      "error" => $exception,
      "back_trace" => debug_backtrace(),
      "controller" => Application::get_instance()->get_routed_controller(),
      "action" => Application::get_instance()->get_routed_action(),
      "route" => Application::get_instance()->get_route_rule(),
    };

    if($exception instanceof NinjackException){
      $http_code = $exception->get_http_code();
    }
    else{
      $http_code = 500;
    }

    http_response_code($http_code);

    $view = Application::get_instance()->loader()->load_view($http_code.".dev");
    $view->set_many_variables($variables);

    echo $view->get_render();
  }

  /**
   * Set the exception handler.
   */
  public function set_exception_handler() : void{
    set_exception_handler(array($this, "handler"));
  }

  /**
   * Return the unique instance of Error.
   *
   * @return Ninjack\Core\Error the unique instance of error.
   */
  public static function get_instance() : Error{
    if(self::$instance === null){
      self::$instance = new Error();
    }

    return self::$instance;
  }


}
