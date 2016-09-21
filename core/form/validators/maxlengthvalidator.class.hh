<?hh
namespace Ninjack\Core\Form\Validators;
use Ninjack\Core\Form\FormValidator as FormValidator;


/**
 * Input validator checkin the maximum length.
 *
 * @author Sulivan
 */
class MaxLengthValidator extends FormValidator{

  /**
   * Maximum number of values characters
   */
  private int $maximum;

  /**
   * Constructor
   *
   * @param int $maximum Maximum number of values characters
   * @param ?string message The custom error message.
   */
  public function __construct(int $maximum, ?string $message = null){
    if($message == null){
      $message = "{name} must have ".$maximum." character(s) or less.";
    }
    parent::__construct($message);

    $this->maximum = $maximum;
  }

  /**
   * Returns false if the number of characters of the value is greater
   * than the maximum number of characters authorise, else returns true.
   * If the value is a Traversable, then the method checks all values in the value and
   * returns false when a value isn ot valid.
   *
   * @return true if the value is valid, else false.
   */
  <<__Override>>
  public function is_valid(mixed $value) : bool{

    if(!($value instanceof Traversable)){
      $value = Vector{$value};
    }
    if($value instanceof Traversable){
      foreach ($value as $val) {

        if(strlen($val) > $this->maximum){
          return false;
        }
      }
      return true;
    }

    return false;
  }

}
