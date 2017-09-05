<?hh
namespace Ninjack\Core\Router;
use Ninjack\Core\Router\Parameter;
use Ninjack\Core\Application as Application;


/**
 * A route parameter which value is extract from uri pattern group.
 *
 * @author Sulivan
 */
class UriParameter extends Parameter{

  public function __construct(string $name) {
    parent::__construct($name);
  }

  /**
   * Returns the parameter value from uri pattern group.
   *
   * @return string The parameter value from uri pattern group.
   */
  <<__Override>>
  public function get_value() : string {
    return strval(Application::get_instance()->get_request()->uri_parameter($this->name, ""));
  }
}
