<?hh
namespace Ninjack\Core\Display;

class Media{


  public function __construct(
    private ?int $min_width = null,
    private ?int $max_width = null,
    private ?string $media = null,
    private Map<string, string> $rules = Map{}
  ){

  }

  public function get_query() : string{

    $media = $this->media;

    if($media === null){
      $media = "screen ";
    }
    if($this->min_width !== null){
      $media .= (!empty($media) ? "and " : "") . "(min-width: ".$this->min_width."px) ";
    }
    if($this->max_width !== null){
      $media .= (!empty($media) ? "and " : "") . "(max-width: ".$this->max_width."px) ";
    }

    foreach ($this->rules as $rule => $value) {
      $media .= (!empty($media) ? "and " : "") . "(".$rule.": ".$value.") ";
    }

    return "@media ".$media;

  }

  public function wrap(string $content) : string{
    return $this->get_query() . "{" . $content ."}";
  }

}
