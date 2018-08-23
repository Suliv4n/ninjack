<?hh
namespace Ninjack\Core\Form;
use Ninjack\Core\Application as Application;
use Ninjack\Core\Database\Orm\Field as Field;
use Ninjack\Core\Database\Orm\Field\ForeignKey as ForeignKey;
use Ninjack\Core\Database\Orm\Field\ManyToMany as ManyToMany;
use Ninjack\Core\Database\Orm\Field\ManyToOne as ManyToOne;
use Ninjack\Core\Database\Orm\ORMObject as ORMObject;
use Ninjack\Core\Database\Entity as Entity;
use Ninjack\Core\CSRFToken as CSRFToken;
use Ninjack\Core\Form\FormInput as FormInput;
use Ninjack\Core\Form\FormValidator as FormValidator;
use Ninjack\Core\Exception\InvalidCSRFTokenException as InvalidCSRFTokenException;
use Ninjack\Core\Form\Inputs\TextInput as TextInput;
use Ninjack\Core\Form\Inputs\SelectInput as SelectInput;
use Ninjack\Core\Form\Inputs\HiddenInput as HiddenInput;

use xhp_form;
use xhp_input;

/**
 * An html form genrator and checker.
 *
 * @author Sulivan
 */
class Form{

  /**
   * The FormInput of the Form.
   */
  private Vector<FormInput> $inputs;

  /**
   * The attributes for the generated form element.
   */
  private Map<string, mixed> $attributes;


  /**
   * The errors message generated by inputs validators.
   */
  private Vector<string> $errors;

  /**
   * The name of the form
   */
  private string $name = "";

  /**
   * True if the crsf protection is activate.
   */
  private bool $csrf_protection = true;


  private ?string $entity_bind_class;

  /**
   * Constructor.
   */
  public function __construct(){
    $this->inputs = Vector{};
    $this->attributes = new Map(null);
    $this->errors = Vector{};
  }

  /**
   * Run all the inputs validators and generate errors strings. (can be true only if post request)
   *
   * @return True of the form is valid, else false.
   */
  public function run() : bool{
    $request = Application::get_instance()->get_request();
    $session = Application::get_instance()->session();
    $token = $session->get_value($this->get_csrf_token_input_name());

    if($request->is_post() && $this->csrf_protection){
      if($token instanceof CSRFToken){
        if(
            $token->is_expired() ||
            $request->post($this->get_csrf_token_input_name()) != $token->get_value()
          ){

            throw new InvalidCSRFTokenException();
        }
      }
      else{
        throw new InvalidCSRFTokenException();
      }
    }

    foreach ($this->inputs as $input){
      if(!$input->is_valid()){
        $this->errors->addAll($input->get_errors());
      }
    }

    return $request->is_post() && $this->errors->isEmpty();
  }

  /**
   * Add an input to the form.
   * Attach validators to the input.
   *
   * @param FormInput $input the input to add.
   * @param Vector<FormValidator> $validators Validators to attach to the input.
   */
  public function add_input(FormInput $input, Vector<FormValidator> $validators = Vector{}) : void{
    $input->set_validators($validators);
    $input->set_form_source($this);

    $this->inputs->add($input);
  }

  /**
   * Returns the inputs of the form.
   *
   * @return Vector<FormInput> the inputs of the form.
   */
  public function get_inputs() : Vector<FormInput>{
    return $this->inputs;
  }

  /**
   * Load configuration given by name and returns
   * the configured form.
   *
   * @param The name of the form configuration to load.
   *
   * @return the configred Form.
   */
  public function load_form($name) : Form{
    $path = Application::get_instance()->loader()->get_form_path($name);

    if($path === null){
      throw  new \Exception("No form was found with the name : $name");
    }

    include $path;

    $this->name = $name;
    return $this;
  }

  /**
   * Set the attributes of the generated xhp form element.
   *
   * @param Map<string,mixed> $attrinutes The attributes of the form xhp element.
   */
  public function set_attributes(Map<string,mixed> $attrinutes){
    $this->attributes = $attrinutes;
  }

  /**
   * Return the generated xhp form element.
   * The form contains all xhp input elements of the form.
   *
   * @return the xhp form element with all inputs.
   */
  public function get_element() : \XHPChild{
    $form = <form method="post"></form>;

    if($this->csrf_protection){
      $form->appendChild($this->get_csrf_token_input());
    }

    foreach ($this->inputs as $input) {

      if($input->is_label_rendered()){
        $form->appendChild($input->get_label_element());
      }
      $form->appendChild($input->get_element());
    }

    return $form;
  }

  /**
   * Returns the csrf token hidden input.
   *
   * @return \XHPChild The xhp hidden input with the csrf token as value.
   */
  private function get_csrf_token_input() : \XHPChild{

    $input = <input type="hidden"></input>;
    $token = new CSRFToken($this->get_csrf_token_input_name());

    $input->setAttribute("value", $token->get_value());
    $name = $this->get_csrf_token_input_name();

    $input->setAttribute("name", $name);

    Application::get_instance()->session()->set_value($name, $token);

    return $input;
  }

  /**
   * Returns the csrf input name.
   *
   * @return string the name of the csrf input.
   */
  private function get_csrf_token_input_name() : string{
    $name = $this->entity_bind_class.$this->name."_csrf_token";
    return $name;
  }

  /**
   * Generate inputs to bind with the fields of a given entity.
   *
   * @param string $entity The class nameof the entity to bind with the form.
   * @param ?Vector<FormInput> $inputs the inputs to add to the form. If null, inputs will
   * be auto generated to bind the entity fields.
   */
  public function bind_entity_class(string $entity, ?Vector<FormInput> $inputs = null) : void{
    $this->entity_bind_class = $entity;

    if($inputs == null){
      $fields = Entity::get_orm($entity)->get_fields();

      foreach ($fields as $name => $field) {
        if(!$field instanceof ManyToOne){
          if($field->is_primary_key()){
            $this->add_input(
              new HiddenInput($name),
              Vector{}
            );
          }
          else if($field instanceof ForeignKey){
            $this->add_input(
              new SelectInput($name, $name, $field->get_choices()),
              Vector{}
            );
          }
          else if($field instanceof ManyToMany){
            $this->add_input(
              new SelectInput($name, $name, $field->get_choices(), Map{"multiple" => true}),
              Vector{}
            );
          }
          else{
            $this->add_input(
              new TextInput($name, $name),
              Vector{}
            );
          }
        }
      }
    }
    else{
        $this->inputs->addAll($inputs);
    }

  }

  /**
   * Enable or disable the csrf protection.
   *
   * @param bool $enable true if the protection should be enable, else false.
   */
  public function set_csrf_protection(bool $enable) : void{
    $this->csrf_protection = $enable;
  }

  /**
   * Return all the inpits values of the form.
   *
   * @return Map<string, value> The values of the input form. Keys are input names and
    * the values the inputs values.
   */
  public function get_values() : Map<string, mixed>{
    $values = Map{};
    foreach($this->inputs as $input){
      $values[$input->get_name()] = $input->get_value();
    }

    return $values;
  }

  /**
   * Return the entity object generated by using the input values.
   * The form has to be binded with a class if no class is given in parameters.
   *
   * @param ?string $class the entity to generate class. If null given then it will
   * use the class given in bind_entity_class method.
   *
   * @return Entity the entity generated.
   *
   * @see Ninjack\Core\Form::bind_entity_class
   */
  public function get_entity(?string $class = null): Entity{
    if($this->entity_bind_class == null && $class == null){
      throw new Exception("No entity class binded.");
    }
    $entity_class = $class == null ? $this->entity_bind_class : $class;

    $entity = Entity::get_from_data($entity_class ?? "", $this->get_values());

    return $entity;
  }

}
