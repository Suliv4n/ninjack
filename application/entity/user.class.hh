<?hh
namespace Application\Entity;
use Application\Entity\Role as Role;
use Ninjack\Core\Database\Entity as Entity;
use Ninjack\Core\User\UserInterface as UserInterface;
use Ninjack\Core\User\RoleInterface as RoleInterface;

<<Database("main"), Table("user")>>
class User extends Entity implements UserInterface{

  private Vector<RoleInterface> $roles = Vector{};

  public function __construct(
    private ?int $id,
    private string $username,
    private string $password
  ){

  }

  public function get_roles() : Vector<RoleInterface>{
    return $this->roles;
  }

  <<ManyToMany("id","user_role.id_user","user_role.id_role","role.id")>>
  public function set_roles(Vector<Role> $roles) : void{
    $this->roles->clear();
    foreach ($roles as $role) {
      if($role instanceof RoleInterface){
        $this->roles->add($role);
      }
    }
  }

  public function get_username() : string{
    return $this->username;
  }

}
