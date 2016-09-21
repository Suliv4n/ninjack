<?hh
namespace Ninjack\Core;


/**
 * Class storing data session.
 *
 * @author Sulivan
 */
class Session{

  private static ?Session $instance;


  private function __construct(){
    session_start();
  }

  /**
   * Store a value in the current session.
   *
   * @param string $name the name of the value to store.
   * @param mixed $value the value to store.
   */
  public function set_value(string $name, mixed $value) : void{
    $_SESSION[$name] = $value;
  }

  /**
   * Returns a session value found by name.
   *
   * @param string $name the value name to return.
   *
   * @return the session value.
   */
  public function get_value(string $name) : mixed{
    return $_SESSION[$name];
  }

  public static function get_instance() : Session{
    if(self::$instance == null){
      self::$instance = new Session();
    }

    return self::$instance;
  }

}
