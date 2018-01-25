<?hh
namespace Ninjack\Core\Form\Validators;
use Ninjack\Core\Form\FormValidator as FormValidator;


/**
 * Check if an input value is not empty, and if it is string, it's not only blan characters.
 *
 * @author Sulivan
 */
class NotBlankValidator extends FormValidator{



  /**
   * Constructor
   *
   * @param ?string message The custom error message.
   */
  public function __construct(?string $message = null){
    if($message == null){
      $message = "{name} is required.";
    }
    parent::__construct($message);

    $this->maximum = $maximum;
  }

  /**
   * Return false if the value is empty or if the value is composed
   * of blank characters only.
   * 
   * @return true if the value is valid, else false.
   */
  <<__Override>>
  public function is_valid(mixed $value) : bool {
    
    $valid = !empty($value);

    if(is_string($value)){
        $valid &= preg_match('/^\s*$/',$value);
    }

    return $value;
  }

}
