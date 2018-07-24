<?hh
namespace Ninjack\Core\Router;
use Ninjack\Core\Router\Parameter;
use Ninjack\Core\Application as Application;


/**
 * A route parameter which value is extract from request get parameters.
 *
 * @author Sulivan
 */
class GetParameter extends Parameter{

  public function __construct(string $name) {
    parent::__construct($name);
  }

  /**
   * Returns the parameter value from request get parameters.
   *
   * @return string The parameter value from request get parameters.
   */
  <<__Override>>
  public function get_value() : string {
    return strval(Application::get_instance()->get_request()->get($this->name,""));
  }
}
