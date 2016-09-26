<?hh
namespace Application\Entity;
use Ninjack\Core\Database\Entity as Entity;

<<Database("main"), Table("article")>>
class Article extends Entity{

  private ?Author $author;

  public function __construct(
    private ?int $id,
    private string $title
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

  <<ForeignKey("fk_author","author.id")>>
  public function set_author(Author $author) : void{
    $this->author = $author;
  }



  public function set_title(string $title) : void{
    $this->title = $title;
  }

}
