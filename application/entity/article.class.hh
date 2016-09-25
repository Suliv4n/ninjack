<?hh
namespace Application\Entity;
use Ninjack\Core\Database\Entity as Entity;

<<Database("main"), Table("article")>>
class Article extends Entity{

  private ?Author $author;

  public function __construct(
    private ?int $id,
    private string $title,
    private ?int $fk_author
  ){
  }


  <<Get("id"), PrimaryKey>>
  public function get_id() : ?int{
    return $this->id;
  }

  <<Get("title")>>
  public function get_title() : string{
    return $this->title;
  }

  <<Get("author"),ForeignKey("author.id")>>
  public function get_author() : ?Author{
    return $this->author;
  }

  <<Get("fk_author")>>
  public function get_id_author() : ?int{
    return $this->fk_author;
  }

  <<Set("fk_author")>>
  public function set_id_author(?int $fk_author) : void{
    $this->fk_author = $fk_author;
  }

  public function set_title(string $title) : void{
    $this->title = $title;
  }

}
