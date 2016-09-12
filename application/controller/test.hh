<?hh

use Ninjack\Core\Controller as Controller;
use Ninjack\Core\Response as Response;
use Ninjack\Core\Database\Entity as Entity;
use Ninjack\Core\Enum\DBOperator as DBOperator;
use Application\Entity\Article as Article;
use Ninjack\Core\Database\QueryBuilder as QueryBuilder;

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



    $conn = $this->get_database("main");
    if($conn != null){
      echo "<pre>";
      var_dump(
        $conn->query_builder()->update("article", Map{
            "title" => "Title modified B",
        })->where("id", 2, DBOperator::NOT_EQUALS)->execute()
      );
      echo "</pre>";
    }

    //$article = Entity::get(Article::class)[0];
    //$article->save();
    $this->add_to_view("name", $name);

  }

}
