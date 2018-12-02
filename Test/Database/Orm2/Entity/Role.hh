<?hh
namespace Ninjack\Test\Database\Orm2\Entity;

use Ninjack\Core\Database\Orm2\Attributes\Fields\{Id, Field, Foreign};

class Role
{
  public function __construct(
    <<Id>>    private ?int $id,
    <<Field>> private string $name
  )
  {}

  public function get_name() : string
  {
    return $this->name;
  }
}
