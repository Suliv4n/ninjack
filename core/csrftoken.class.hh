<?hh
namespace Ninjack\Core;

/**
 * The CSRF token class.
 *
 * @author Sulivan
 */
class CSRFToken{

  /**
   * Default validity time in seconds.
   */
  const int DEFAULT_EXPIRATION_TIME = 60*60;

  /**
   * The token values.
   */
  private string $value;

  /**
   * The constructor.
   *
   * @param string $token_name the name of the token.
   * @param int $time_begin the begin timestamp of the token. Default is current tile.
   * @param int $duration the validity duration of the token.
   */
  public function __construct(
  private string $token_name,
  private int $time_begin = time(),
  private int $duration = self::DEFAULT_EXPIRATION_TIME
  ){
    $this->value = uniqid(rand(), true);
  }

  /**
   * Returns true if the token is expired, else false.
   *
   * @return bool true if the token is expired, else false.
   */
  public function is_expired() : bool{
    return (time() - $this->time_begin) > $this->duration;
  }

  /**
   * Returns the taken value.
   *
   * @return string the token value.
   */
  public function get_value() : string{
    return $this->value;
  }
}
