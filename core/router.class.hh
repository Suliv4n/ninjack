<?hh
namespace ninjack\core;

class Router {

  private Configuration $configuration;

  public function __construct(){
    $this->configuration = Loader::load_configuration("route.hh");
  }

  public function route($uri) : ?string{
    $routes = $this->configuration->get("routes");

    if($routes instanceof Map && $routes != null){
      foreach ($routes as $regex => $action) {
          $escaped = preg_replace("~/~", "\\/", $regex);

          if(preg_match("/".$escaped."/", $uri)){
            return $action;
          }
      }
    }
    else{
      //@todo throw exception
    }
    return null;
  }


}
