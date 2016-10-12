<?hh
namespace Application\Entity;
use Application\Entity\Article as Article;
use Application\Entity\Comment as Comment;
use Application\Entity\Tag as Tag;
use Ninjack\Core\Database\Entity as Entity;

<<Database("main"), Table("article")>>
class Article extends Entity{

  private ?Author $author;

  private Vector<Comment> $comments  = Vector{};

  private Vector<Tag> $tags = Vector{};

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

  <<Get("fk_author")>>
  public function get_author() : ?Author{
    return $this->author;
  }

  <<OneToMany("id", "comment.fk_article")>>
  public function set_comments(Vector<Comment> $comments) : void{
    $this->comments = $comments;
  }

  <<ManyToMany("id","article_tag.id_article","article_tag.id_tag","tag.id")>>
  public function set_tags(Vector<Tag> $tags) : void{
    $this->tags = $tags;
  }

  <<Get("article_tag.id_tag")>>
  public function get_tags() : Vector<Tag>{
    return $this->tags;
  }

  public function set_title(string $title) : void{
    $this->title = $title;
  }

}
