<?hh

use Ninjack\Core\Controller as Controller;
use Ninjack\Core\Response as Response;

class Test extends Controller{

  public function home() : void{

  }

  public function welcome(string $name) : void{
    $db = $this->get_database("main");
    if($db != null){
      $res = $db->query("select * from test")->fetchAll();
      var_dump($res);
    }
    $this->add_to_view("name", $name);

  }

}
