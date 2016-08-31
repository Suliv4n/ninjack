<?hh

use ninjack\core\Controller as Controller;
use ninjack\core\Response as Response;

class Test extends Controller{

  public function home() : Response{
    return new Response("default");
  }

  public function welcome(string $name) : Response{

    return new Response("default");
  }

}
