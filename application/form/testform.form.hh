<?hh
namespace Application\Form;
use Ninjack\Core\Form\Inputs\TextInput as TextInput;
use Ninjack\Core\Form\Inputs\SubmitInput as SubmitInput;
use Ninjack\Core\Form\Validators\MaxLengthValidator as MaxLengthValidator;

$this->add_input(
  new TextInput("test","Un test",Map{
    "class" => "input-test",
  }),
  Vector{
    new MaxLengthValidator(5),
  }
);

$this->add_input(
  new SubmitInput("submit","Submit",Map{
    "value" => "submit",
  }),
  Vector{}
);
