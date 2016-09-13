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
    $this->add_to_view("name", $name);

  }

}
