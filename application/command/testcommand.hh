<?hh

use Ninjack\Core\Cli\Command as Command;
use Ninjack\Core\Cli\CLIProgressBar as CLIProgressBar;
use Ninjack\Core\Enum\CLIFGColor as CLIFGColor;
use Ninjack\Core\Enum\CLIBGColor as CLIBGColor;

class TestCommand extends Command{

  <<Command>>
  public function test() : void{

    $progress = new CLIProgressBar($this->console, 10., "Done");
    $progress->set_size(30);

    for($i=1;$i<=10;$i++){
      sleep(1);
      $progress->set_currint($i);
      $progress->print();
    }

  }

}
