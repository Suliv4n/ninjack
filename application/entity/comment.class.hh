<?hh
namespace Application\Entity;
use Ninjack\Core\Database\Entity as Entity;

<<Database("main"), Table("comment")>>
class Comment extends Entity{


  public function __construct(
    private int $id,
    private string $content
  ){

  }
}
