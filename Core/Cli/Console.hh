<?hh
namespace Ninjack\Core\Cli;
use Ninjack\Core\Application as Application;
use Ninjack\Core\Enum\CLIFGColor as CLIFGColor;
use Ninjack\Core\Enum\CLIBGColor as CLIBGColor;
use Ninjack\Core\Exception\CLIException as CLIException;

/**
 * Class Singleton that represents the console cli.
 *
 * @author Sulivan
 */
class Console{

  /**
   * The uniq instance the class.
   */
  private static ?Console $instance;

  /**
   * Constructor.
   */
  private function __construct(){

  }

  /**
   * Returns the uniq instance of the class.
   *
   * @return Ninjack\Core\Cli\Console the uniq instance of the class.
   */
  public static function get_instance() : Console{
    if(!Application::get_instance()->is_cli()){
      throw new CLIException("Can not load Console on none CLI mode.");
    }
    if(self::$instance == null){
      self::$instance = new Console();
    }

    return self::$instance;
  }

  /**
   * Print a string representation of a mixed value with a specific color and a speicifiq background color when they
   * are given.
   *
   * @param mixed $value The value that will be printed.
   * @param ?Ninjack\Core\Enum\CLIFGColor $color The color of string that will be printed.
   * If null it will be printed with the default cli text color.
   * @param ?Ninjack\Core\Enum\CLIFGColor $bg_color The background color of the string that will be printed.
   * If null, no background will be used.
   */
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

  /**
   * Print a string representation of a mixed value on a new line ith a specific color and a speicifiq background color when they
   * are given.
   *
   * @param mixed $value The value that will be printed.
   * @param ?Ninjack\Core\Enum\CLIFGColor $color The color of string that will be printed.
   * If null it will be printed with the default cli text color.
   * @param ?Ninjack\Core\Enum\CLIFGColor $bg_color The background color of the string that will be printed.
   * If null, no background will be used.
   */
  public function println(mixed $value="", ?CLIFGColor $color = null, ?CLIBGColor $bg_color = null) : void{
    $this->print($value, $color, $bg_color);
    $this->print("\n");
  }
}
