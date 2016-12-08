<?hh
namespace Ninjack\Core\Helper;
use Ninjack\Core\Application;

class Url{

  public static function absolute_url(?string $uri = null){
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

  public static function is_absolute_url(string $url) : bool{
    return preg_match("/^(https?:)?\/\//", $url) > 0;
  }

}
