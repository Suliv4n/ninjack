<?hh
/*
@todo mayby put this in core.
*/
use Ninjack\Core\Cli\Command as Command;
use Ninjack\Core\Helper\File as File;
use Ninjack\Core\Enum\CLIFGColor as CLIFGColor;
use Ninjack\Core\Enum\CLIBGColor as CLIBGColor;

class Project extends Command{
  <<Command>>
  public function todo() : void{
    $this->scan_for_todos(ROOT);
  }

  private function scan_for_todos($dir) : void {
    $files = File::scandir($dir)?->filter($file ==> basename($file)[0] != '.');

    if($files != null){
      foreach ($files as $file) {

        if(is_dir($file)){
          $this->scan_for_todos($file);
        }
        else if(pathinfo($file, PATHINFO_EXTENSION) == "hh"){
          $tokens = new Vector(token_get_all(file_get_contents($file)));
          $comments = $tokens->filter($token ==> in_array($token[0], Vector{T_COMMENT,T_DOC_COMMENT}));

          foreach ($comments as $comment) {
              $lines = explode("\n", $comment[1]);
              foreach ($lines as $l => $line) {
                $explode_todo = preg_split("/@todo/i", $line);
                for($i=1;$i<count($explode_todo);$i++){
                  $this->console->print("@todo : ", CLIFGColor::LIGHT_BLUE);
                  $this->console->println(trim($explode_todo[$i]), CLIFGColor::LIGHT_GREEN);
                  $this->console->println("File : " . $file, CLIFGColor::YELLOW);
                  $this->console->println("Line : " . ($comment[2]+$l), CLIFGColor::YELLOW);
                  $this->console->println();
                }
              }
          }
        }
      }
    }



  }


}
