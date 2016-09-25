<?hh
namespace Ninjack\Core\Cli;
use Ninjack\Core\Application as Application;
use Ninjack\Core\Enum\CLIFGColor as CLIFGColor;
use Ninjack\Core\Enum\CLIBGColor as CLIBGColor;
use Ninjack\Core\Exception\CLIException as CLIException;

class Console{

  private static ?Console $instance;

  private function __construct(){

  }

  public static function get_instance() : Console{
    if(!Application::get_instance()->is_cli()){
      throw new CLIException("Can not load Console on none CLI mode.");
    }
    if(self::$instance == null){
      self::$instance = new Console();
    }

    return self::$instance;
  }

  public function print(mixed $value, ?CLIFGColor $color = null, ?CLIBGColor $bg_color = null) : void{
    $string = strval($value);
    if($bg_color != null){
      $string = "\033[".$bg_color."m".$string;
    }
    if($color != null){
      $string = "\033[".$color."m".$string;
    }

    if($color != null || $bg_color != null){
      $string = $string."\033[0m";
    }


    echo $string;

  }


  public function println(mixed $value="", ?CLIFGColor $color = null, ?CLIBGColor $bg_color = null) : void{
    $this->print($value, $color, $bg_color);
    $this->print("\n");
  }
}
