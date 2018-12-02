<?hh
namespace Ninjack\Test\Database\Orm2\Entity;

use Ninjack\Core\Database\Orm2\Attributes\Fields\{Id, Field, Foreign};

class UserWithRole
{

  public function __construct(
    <<Id>>      private ?int $id,
    <<Field>>   private string $username,
    <<Foreign>> private Role $role
  )
  {}

  public function get_role() : Role
  {
    return $this->role;
  }

}
