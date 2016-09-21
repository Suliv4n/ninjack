<?hh

use Ninjack\Core\Cli\Command as Command;
use Ninjack\Core\Enum\CLIFGColor as CLIFGColor;
use Ninjack\Core\Enum\CLIBGColor as CLIBGColor;

class TestCommand extends Command{

  <<Command>>
  public function test() : void{
    $this->console->println("Test with Color", CLIFGColor::LIGHT_BLUE, CLIBGColor::LIGHT_GRAY);
    $this->console->println("Test with Color");
  }

}
