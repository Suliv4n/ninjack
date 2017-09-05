<?hh
namespace Ninjack\Core;
use Ninjack\Core\User\UserInterface as UserInterface;

/**
 * Class to access server data.
 *
 * @author Sulivan
 */
class Server{

  /**
   * Returns the server root path.
   *
   * @return string the server root path.
   */
  public function get_root_path() : string{
    return $_SERVER["DOCUMENT_ROOT"];
  }

  /**
   * Returns the server root url.
   *
   * @return string the server root url.
   */
  public function get_root_url() : string{

    $ssl = $this->get_item("HTTPS");
    $ssl_enabled = !empty($ssl) && $ssl == "on";

    $protocol = "http".($ssl_enabled ? "s" : "");

    $port = $this->get_item("SERVER_PORT");
    $port = (!$ssl_enabled && $port=="80") || ($ssl_enabled && $port =="443") ? "" : ":".$port;

    $host = $protocol."://".$this->get_item("SERVER_NAME").$port;

    return $host;
  }

  /**
   * Returns a server item from its key.
   *
   * @param string $key the item key.
   * @param ?string $default the default value.
   *
   * @return ?string The server item or default value if it doesn't exist.
   */
  public function get_item(string $key, ?string $default = null) : ?string{
    return isset($_SERVER[$key]) ? $_SERVER[$key] : $default;
  }


}
