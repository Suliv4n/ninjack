<?hh

namespace ninjack\core;

class Request{

  public function __construct(){

  }


  public function get_uri() : string{
    return $_SERVER["REQUEST_URI"];
  }

}
