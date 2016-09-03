<?hh

use Ninjack\Core\Controller as Controller;
use Ninjack\Core\Response as Response;

class Test extends Controller{

  public function home() : void{

  }

  public function welcome(string $name) : void{

    $this->add_to_view("name", $name);

  }

}
