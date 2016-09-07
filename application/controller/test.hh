<?hh

use Ninjack\Core\Controller as Controller;
use Ninjack\Core\Response as Response;
use Ninjack\Core\Database\Entity as Entity;
use Application\Entity\Article as Article;

class Test extends Controller{

  public function home() : void{

  }

  <<Action>>
  public function welcome(string $name) : void{
    /*
    $db = $this->get_database("main");
    if($db != null){
      $res = $db->query("select * from test")->fetchAll();
      var_dump($res);
    }
    */
    var_dump(Entity::get(Article::class));
    $this->add_to_view("name", $name);

  }

}
