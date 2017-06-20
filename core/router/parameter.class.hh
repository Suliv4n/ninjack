<?hh
namespace Ninjack\Core\Router;

use Ninjack\Core\Exception\NinjackException;
use Ninjack\Core\Router\GetParameter as GetParameter;
use Ninjack\Core\Router\PostParameter as PostParameter;
use Ninjack\Core\Router\UriParameter as UriParameter;
use Ninjack\Core\Application as Application;

abstract class Parameter {
  protected string $name;

  public function __construct(string $name) {
    $this->name = $name;
  }

  public abstract function get_value() : string;

  public function get_name() : string {
    return $this->name;
  }

  public static function from_get(string $name) : GetParameter {
    return new GetParameter($name);
  }

  public static function from_post(string $name) : PostParameter {
    return new PostParameter($name);
  }

  public static function from_uri(string $name) : UriParameter {
    return new UriParameter($name);
  }
}
