<?hh
namespace Ninjack\Test\Database\Orm2\Entity;

use Ninjack\Core\Database\Orm2\Attributes\Fields\{Id, Field, ManyToMany};

class UserWithRoles
{

  public function __construct(
    <<Id>>          private ?int $id,
    <<Field>>       private string $username,
    <<ManyToMany>>  private Vector<Role> $roles
  )
  {}

  public function get_roles() : Vector<Role>
  {
    return $this->roles;
  }

}
