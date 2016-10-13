<?hh
namespace Application\Entity;
use Ninjack\Core\Database\Entity as Entity;

<<Database("main"), Table("author")>>
class Author extends Entity{


  public function __construct(
    private string $nom,
    private ?int $id = null
  ){

  }


  <<Get("id"), PrimaryKey>>
  public function get_id() : ?int{
    return $this->id;
  }

  <<Get("nom")>>
  public function get_nom() : string{
    return $this->nom;
  }

  <<__Override>>
  public function __toString() : string{
    return $this->nom;
  }

}
