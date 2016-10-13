<?hh
namespace Application\Entity;
use Ninjack\Core\Database\Entity as Entity;

<<Database("main"), Table("tag")>>
class Tag extends Entity{


  public function __construct(
    private int $id,
    private string $name
  ){

  }

  <<Get("id"), PrimaryKey>>
  public function get_id() : int{
    return $this->id;
  }

  public function __toString() : string{
    return $this->name;
  }
}
