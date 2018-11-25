<?hh
namespace Ninjack\Test\Database\Orm2\Entity;

use Ninjack\Core\Database\Orm2\Attributes\Fields\{Id, Field};

class User
{
  public function __construct(
    <<Id>>    private ?int $id,
    <<Field>> private string $username
  )
  {}

  public function set_username(string $username) : void
  {
    $this->username = $username;
  }

  public function get_username() : string
  {
    return $this->username;
  }
}
