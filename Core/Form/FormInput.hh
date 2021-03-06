<?hh
namespace Ninjack\Core\Form;
use Ninjack\Core\Application as Application;
use Ninjack\Core\Form\Form as Form;
use Ninjack\Core\Form\FormValidator as FormValidator;

use xhp_label;

/**
 * Abstract class for Fom input.
 *
 * @author
 */
abstract class FormInput{

  /**
   * The name of the input.
   */
  private string $name;

  /**
   * The label name of the input.
   */
  private string $label;

  /**
   * The validators of the input.
   */
  private Vector<FormValidator> $validators;

  /**
   * Attributes of the input.
   */
  protected Map<string, mixed> $attributes;

  /**
   * Attributes of the label.
   */
  protected Map<string, mixed> $label_attributes;

  /**
   * True if the label should be renderer, else false.
   */
  protected bool $render_label;

  /**
   * Errors messages generated by the validators.
   * Filled by the is_valid method.
   */
  private Vector<string> $errors;

  /**
   * The Form whic the input belong.
   * If it is null the input is not assigned to a Form.
   */
  private ?Form $form;

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
    bool $render_label = true
  ){
    $this->validators = Vector{};
    $this->name = $name;

    $this->label = $label != null ? $label : $name;
    $this->attributes = $attributes;
    $this->label_attributes = $label_attributes;

    $this->render_label = $render_label;

    $this->errors = Vector{};
  }


  /**
   * Checks if the value in the user request is valid.
   * If not returns false, else returns true.
   *
   *  @return true if the value is valid, else false.
   */
  public function is_valid(){
    $valid = true;
    foreach ($this->validators as $validator) {
      $validate = $validator->is_valid($this->get_value());
      if(!$validate){
        $valid = false;
        $this->errors->add($validator->get_message());
      }
    }

    return $valid;
  }


  /**
   * Returns the error messages generated by the validators assigned
   * to the input. An empty Vector is returned if the input value is
   * valid, or if the is_valid was not called.
   *
   * @return The Vector containing the error messages.
   */
  public function get_errors() : Vector<string>{
    return $this->errors;
  }

  /**
   * Returns the error messages generated by the validators assigned
   * to the input. An empty Vector is returned if the input value is
   * valid, or if the is_valid was not called.
   */
  public function get_name() : string{
    return $this->name;
  }

  /**
   * Returns the label name of the input.
   *
   * @return the label name of the input.
   */
  public function get_label() : string{
    return $this->label;
  }

  /**
   * Returns the xhp element of the input. It can be a custom xhp elememt.
   *
   * @return the xhp element of the input.
   */
  abstract public function get_element() : \XHPChild;

  /**
   * Returns the xhp element of the label.
   * This method can be overrided to have a custom render.
   *
   * @return the xhp element of the input.
   */
  public function get_label_element() : \XHPChild{

    $this->label_attributes["for"] = $this->name;

    $label = <label>{$this->label}</label>;
    foreach ($this->label_attributes as $name => $value) {
      $label->setAttribute($name, $value);
    }

    return $label;
  }

  /**
   * Set the validators of the input.
   *
   * @param Vector<FormValidator> $validators validators to attach to the input.
   */
  public final function set_validators(Vector<FormValidator> $validators) : void{
    $this->validators = $validators;
  }

  /**
   * Returns the attributes of the input for the xhp element.
   *
   * @return the attributes of the input.
   */
  public function get_attributes() : Map<string, mixed>{
    return $this->attributes;
  }

  /**
   * Returns the attributes of the label for the xhp label element.
   *
   * @return  the attributes of the label.
   */
  public function get_label_attributes() : Map<string, mixed>{
    return $this->label_attributes;
  }

  /**
   * Returns true if the label should be rendered, else returns false.
   *
   * @return  true if the label should be rendered, else returns false.
   */
  public function is_label_rendered() : bool{
    return $this->render_label;
  }

  /**
   * Set the Form wich the input belong.
   *
   * @param Form $form the Form source.
   */
  public function set_form_source(Form $form) : void{
    $this->form = $form;
  }

  /**
   * Returns the value of the input. For now the value
   * searched in the post data by the name of the input.
   *
   * @return the value of the data. Can be a string or an array.
   */
  public function get_value() : mixed{
    $request = Application::get_instance()->get_request();
    $name = str_replace(".", "_", $this->name);
    return $request->post($name);
  }

}
