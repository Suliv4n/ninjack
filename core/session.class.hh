<?hh
namespace Ninjack\Core;
use Ninjack\Core\User\UserInterface as UserInterface;

/**
 * Class storing data session.
 *
 * @author Sulivan
 */
class Session{

  /**
   * The uniq instance of Session.
   */
  private static ?Session $instance;
  /**
   * The user attached to the session.
   */
  private static ?UserInterface $user;

  /**
   * The index of the session where miscellaneous data are stored.
   */
  const string DATA_INDEX = "ninjack.data";
  /**
   * The index of session where user is stored.
   */
  const string USER_INDEX = "ninjack.user";

  /**
   * The constructor.
   */
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

  /**
   * Returns the uniq of instance of the Session class.
   *
   * @return Ninjack\Core\Session The uniq Session instance.
   */
  public static function get_instance() : Session{
    if(self::$instance == null){
      self::$instance = new Session();
    }

    return self::$instance;
  }

  /**
   * Returns the user in the session. If no user was added to the session,
   * returns the null.
   *
   * @return ?UserInterface The user in the session or null if there is no user.
   */
  public function get_user() : ?UserInterface{
    $user = $_SESSION[self::USER_INDEX];

    if($user instanceof UserInterface){
      return $user;
    }
    return null;
  }

  /**
   * Set the user of the session.
   *
   * @param ?Ninjack\Core\User\UserInterface $user the user to attach to the session.
   */
  public function set_user(?UserInterface $user) : void{
    $_SESSION[self::USER_INDEX] = $user;
  }

}
