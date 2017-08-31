<?hh
namespace Ninjack\Core\Helper;
use Ninjack\Core\Application;
use Ninjack\Core\Router\Router;

/**
 * A helper class to manipulate URL.
 */
class Url{


  /**
   * Returns the absolute URL the URI in parameter.
   * If the uri is already an absolute URL then it returns the uri.
   * If the uri is null it returns the absolute root url.
   *
   * @param ?string $uri the uri
   *
   * @return string the absolute url of the uri in parameter.
   */
  public static function absolute_url(?string $uri = null) : string{
    $url = Application::get_instance()->server()->get_root_url();


    if($uri === null){
      return $url;
    }
    else if(self::is_absolute_url($uri)){
      return $uri;
    }
    else if(strlen($uri) > 0 && $uri[0] == "/"){
      return $url.$uri;
    }
    else{
      $current = Application::get_instance()->get_request()->get_uri();
      if(strlen($current) > 0 && $current[strlen($current)] === "/"){
        $current = substr($current, 0, strlen($current) - 1);
      }
      return $url.$current."/".$uri;
    }

  }

  /**
   * Test if the url in paramter is absolute.
   *
   * @param string the url to test.
   *
   * @return true if the url is absolute else false.
   */
  public static function is_absolute_url(string $url) : bool{
    return preg_match("/^(https?:)?\/\//", $url) > 0;
  }


  public static function create_url_by_reference(string $reference, mixed ...$parameters) :string{

    return Application::get_instance()->router()->create_url_by_reference($reference, new Vector($parameters));

  }

}
