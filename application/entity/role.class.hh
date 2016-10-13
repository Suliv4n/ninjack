<?hh
namespace Application\Entity;
use Ninjack\Core\Database\Entity as Entity;
use Ninjack\Core\User\RoleInterface as RoleInterface;

<<Database("main"), Table("role")>>
class Role extends Entity implements RoleInterface{


  public function __construct(
    private int $id,
    private string $name
  ){

  }

  public function get_name() : string{
    return $this->name;
  }
}
