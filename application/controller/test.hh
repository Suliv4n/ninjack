<?hh
namespace Application\Controller;
use Ninjack\Core\Application as Application;
use Ninjack\Core\Controller as Controller;
use Ninjack\Core\Response as Response;
use Ninjack\Core\Database\Entity as Entity;
use Ninjack\Core\Enum\DBOperator as DBOperator;
use Application\Entity\User as User;
use Application\Entity\Article as Article;
use Ninjack\Core\Database\QueryBuilder as QueryBuilder;
use Ninjack\Core\User\UserInterface as UserInterface;
use Ninjack\Core\Form\Form as Form;
use Ninjack\Core\Form\Inputs\TextInput as TextInput;
use Ninjack\Core\Form\Inputs\SubmitInput as SubmitInput;

class Test extends Controller{

  public function home() : void{

  }

  <<Action>>
  public function welcome(string $name) : void{

    //$users = Entity::get(User::class);
    $form = new Form();
    $form->bind_entity_class(Article::class);

    $form->add_input(new SubmitInput("envoyer"), Vector{});
    $form->set_csrf_protection(false);

    if($form->run()){
      $author = $form->get_entity();

      echo "<pre>";
      var_dump($author);
      echo "</pre>";
    }

    $this->add_to_view('form',$form);

  }

}
