<?hh
namespace Ninjack\Core\Router;
use Ninjack\Core\Router\Parameter;
use Ninjack\Core\Application as Application;

class GetParameter extends Parameter{

  public function __construct(string $name) {
    parent::__construct($name);
  }

  <<__Override>>
  public function get_value() : string {
    return strval(Application::get_instance()->get_request()->get($this->name,""));
  }
}
