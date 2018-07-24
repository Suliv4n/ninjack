<?hh
namespace Ninjack\Core\Router;

use Ninjack\Core\Exception\NinjackException;
use Ninjack\Core\Router\GetParameter as GetParameter;
use Ninjack\Core\Router\PostParameter as PostParameter;
use Ninjack\Core\Router\UriParameter as UriParameter;
use Ninjack\Core\Application as Application;

/**
 * The route parameter handler.
 *
 * @author Sulivan.
 */
abstract class Parameter {

  /**
   * The name of the paramter.
   */
  protected string $name;

  /**
   * Contructor.
   *
   * @param string $name Name of the parameter.
   */
  public function __construct(string $name) {
    $this->name = $name;
  }


  /**
   * Returns the value of the parameter.
   *
   * @return string the parameter value.
   */
  public abstract function get_value() : string;

  /**
   * Returns the parameter name.
   *
   * @return string The parameter name.
   */
  public function get_name() : string {
    return $this->name;
  }

  /**
   * Create a get paramter.
   *
   * @param string $name The parameter name.
   *
   * @return GetParameter the get parameter.
   */
  public static function from_get(string $name) : GetParameter {
    return new GetParameter($name);
  }

  /**
   * Create a post paramter.
   *
   * @param string $name The parameter name.
   *
   * @return PostParameter the get parameter.
   */
  public static function from_post(string $name) : PostParameter {
    return new PostParameter($name);
  }

  /**
   * Create a uri paramter.
   *
   * @param string $name The parameter name.
   *
   * @return PostParameter the uri parameter.
   */
  public static function from_uri(string $name) : UriParameter {
    return new UriParameter($name);
  }
}
