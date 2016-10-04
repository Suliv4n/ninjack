<?hh
namespace Ninjack\Core;
use Ninjack\Core\User\UserInterface as UserInterface;

/**
 * Class storing data session.
 *
 * @author Sulivan
 */
class Session{

  private static ?Session $instance;
  private static ?UserInterface $user;

  const string DATA_INDEX = "ninjack.data";
  const string USER_INDEX = "ninjack.user";

  private function __construct(){
    session_start();

    if(!isset($_SESSION[self::DATA_INDEX]) || !$_SESSION[self::DATA_INDEX] instanceof Map){
      $_SESSION[self::DATA_INDEX] = Map{};
    }
  }

  /**
   * Store a value in the current session.
   *
   * @param string $name the name of the value to store.
   * @param mixed $value the value to store.
   */
  public function set_value(string $name, mixed $value) : void{ //@todo to remove
    $_SESSION[$name] = $value;
  }

  public function set_data(string $name, mixed $value) : void{
    $_SESSION[self::DATA_INDEX][] = $value;
  }

  /**
   * Returns a session value found by name.
   *
   * @param string $name the value name to return.
   *
   * @return the session value.
   */
  public function get_value(string $name) : mixed{ //to remove
    return $_SESSION[$name];
  }

  public function get_data(string $name, mixed $default = null) : mixed{ //to remove
    if( !isset($_SESSION[self::DATA_INDEX][$name])){
      return $default;
    }

    return $_SESSION[self::DATA_INDEX][$name];
  }

  public static function get_instance() : Session{
    if(self::$instance == null){
      self::$instance = new Session();
    }

    return self::$instance;
  }

  public function get_user() : ?UserInterface{
    $user = $_SESSION[self::USER_INDEX];

    if($user instanceof UserInterface){
      return $user;
    }
    return null;
  }

  public function set_user(UserInterface $user) : void{
    $_SESSION[self::USER_INDEX] = $user;
  }

}
