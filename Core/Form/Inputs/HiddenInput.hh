<?hh
namespace Ninjack\Core\Form\Inputs;
use Ninjack\Core\Form\FormInput as FormInput;

use xhp_input;

/**
 * Text input generator.
 *
 * Generate a <input type="text"> tag.
 *
 * @author Sulivan
 */
class HiddenInput extends FormInput{

  /**
   * Constructor.
   *
   * @param string $name The name of the input.
   * @param string $label The label name of the input.
   * @param Map<string, mixed> $attributes Attributes attached to the form.
   * They must be compatible with the input xhp element.
   * @param Map<string, mixed> $label_attributes Attributes of the label.
   * They must be compatible with the label xhp element.
   * @param  bool $render_label True if the label must be rendered, else false (default true).
   */
  public function __construct(
    string $name,
      ?string $label = null,
      Map<string, mixed> $attributes = Map{},
      Map<string, mixed> $label_attributes = Map{},
      bool $render_label = false
  ){
    parent::__construct($name, $label, $attributes, $label_attributes, $render_label);
  }

  /**
   * Return the xhp element of the input (a <input type="text"> element).
   *
   * @return the xhp element of the input.
   */
  <<__Override>>
  public function get_element() : \XHPChild{

    $this->attributes["name"] = $this->get_name();
    $this->attributes["type"] = "hidden";
    $input = <input></input>;

    foreach ($this->attributes as $name => $value) {
      $input->setAttribute($name, $value);
    }

    return $input;
  }

}
