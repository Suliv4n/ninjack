<?hh
namespace Ninjack\Core\Display;

/**
 * Represents a css media.
 *
 * @author Sulivan.
 */
class Media{


  /**
   * Constructor
   *
   * @param ?int $min_width the min width of viewport
   * @param ?int $max_width the max width of viewport
   * @param ?string $media the media of viewport
   * @param Map<string, string> $rules additionnal viewport rules
   */
  public function __construct(
    private ?int $min_width = null,
    private ?int $max_width = null,
    private ?string $media = null,
    private Map<string, string> $rules = Map{}
  ){

  }

  /**
   * Returns the css media query.
   *
   * @return string the css media query.
   */
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

  /**
   * Wraps the content in parameter with the media query.
   *
   * @param string $content the content to wrap.
   *
   * @return string the content wraped.
   */
  public function wrap(string $content) : string{
    return $this->get_query() . "{" . $content ."}";
  }

}
