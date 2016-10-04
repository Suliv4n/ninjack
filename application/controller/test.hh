<?hh
namespace Application\Controller;
use Ninjack\Core\Application as Application;
use Ninjack\Core\Controller as Controller;
use Ninjack\Core\Response as Response;
use Ninjack\Core\Database\Entity as Entity;
use Ninjack\Core\Enum\DBOperator as DBOperator;
use Application\Entity\User as User;
use Ninjack\Core\Database\QueryBuilder as QueryBuilder;
use Ninjack\Core\User\UserInterface as UserInterface;

class Test extends Controller{

  public function home() : void{

  }

  <<Action>>
  public function welcome(string $name) : void{

    $users = Entity::get(User::class);
    $user = $users[0];

    if($user instanceof UserInterface){
      //Application::get_instance()->session()->set_user($user);
    }

    $user = Application::get_instance()->session()->get_user();

    var_dump($user);die();

  }

}
