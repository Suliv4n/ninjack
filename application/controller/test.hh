<?hh

use ninjack\core\Controller as Controller;
use ninjack\core\Response as Response;

class Test extends Controller{

  public function home() : Response{
    return new Response("default");
  }

  public function welcome() : Response{

    $this->add_to_view("name", "sulivan");

    return new Response("");
  }

}
