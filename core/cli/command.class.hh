<?hh
namespace Ninjack\Core\Cli;
use Ninjack\Core\Exception\CLIException as CLIException;
use Ninjack\Core\Cli\Console as Console;
use Ninjack\Core\TypeHelper as TypeHelper;

/**
 * Command called via CLI.
 *
 * @author Sulivan.
 */
abstract class Command{

  /**
   * The console where the command is loaded.
   */
  protected Console $console;

  /**
   * Constructor.
   */
  public function __construct(){
    $this->console = Console::get_instance();
  }


  /**
   * Executes the command passed in parameter. It will run the method
   * named $command of thi class.
   *
   * @param string $command the command name.
   * @param Vector<string> $parameters the paramters of the command.
   */
  final public function go(string $command, Vector<string> $parameters) : void{

    $exists = method_exists($this, $command);
    $is_command = false;
    if($exists){
      $method = new \ReflectionMethod($this, $command);
      $is_command = isset($method->getAttributes()["Command"]);

      if($is_command){
        $parsed_param = TypeHelper::bind_parameters_function($method, $parameters);
        $method->invokeArgs($this, $parsed_param->toArray());
      }

    }

    if(!$exists || !$is_command){
        throw new CLIException("Command ".$command." not found.");
    }

  }

}
