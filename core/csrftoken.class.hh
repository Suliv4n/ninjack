<?hh
namespace Ninjack\Core;

class CSRFToken{

  /**
   * in seconds
   */
  const int DEFAULT_EXPIRATION_TIME = 60*60;

  private string $value;

  public function __construct(
  private string $token_name,
  private int $time_begin = time(),
  private int $duration = self::DEFAULT_EXPIRATION_TIME
  ){
    $this->value = uniqid(rand(), true);
  }

  public function is_expired() : bool{
    return (time() - $this->time_begin) > $this->duration;
  }

  public function get_value() : string{
    return $this->value;
  }
}
