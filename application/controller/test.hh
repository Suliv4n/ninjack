<?hh

use Ninjack\Core\Controller as Controller;
use Ninjack\Core\Response as Response;

class Test extends Controller{

  public function home() : Response{
    return new Response("default");
  }

  public function welcome(string $name) : Response{

    $this->add_to_view("name", $name);

    return new Response("");
  }

}
