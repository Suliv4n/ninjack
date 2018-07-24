<?hh
namespace Ninjack\Core\Form;

/**
 * this abstract represents a rule that an input value
 * has to respect to be valid.
 *
 * @author Sulivan
 */
abstract class FormValidator{

  /**
   * The error message if the tested input value is not valid.
   */
  private string $message;

  /**
   * The constructor.
   *
   * @param string $message The error message to genrate.
   */
  public function __construct(string $message = ""){
    $this->message = $message;
  }

  /**
   * Returns the error message of the validator.
   */
  public function get_message() : string{
    return $this->message;
  }

  /**
   * Returns true if the tested value is valid, else returns false.
   *
   * @param mixed $value The tested value.
   *
   * @return true if the value is valid, else false.
   */
  public abstract function is_valid(mixed $value) : bool;

}
