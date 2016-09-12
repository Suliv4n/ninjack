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
    $articles = Entity::get(Article::class);


    $article = $articles[0];
    if($article instanceof Article){
      $article->set_title("Entity saved !!");
      $article->save();
    }
    */


    $article = new Article(null, "Article added ! ");
    $article->save();

    //$article = Entity::get(Article::class)[0];
    //$article->save();
    $this->add_to_view("name", $name);

  }

}
