<?hh
namespace Application\Entity;
use Application\Entity\Article as Article;
use Application\Entity\Comment as Comment;
use Ninjack\Core\Database\Entity as Entity;

<<Database("main"), Table("article")>>
class Article extends Entity{

  private ?Author $author;

  private ?Vector $comments;

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

  <<OneToMany("Application\\Entity\\Comment", "comment.fk_article", "id")>>
  public function set_comments(Vector<Comment> $comments) : void{
    $this->comments = $comments;
  }

  public function set_title(string $title) : void{
    $this->title = $title;
  }

}
