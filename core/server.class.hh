<?hh
namespace Ninjack\Core;
use Ninjack\Core\User\UserInterface as UserInterface;

/**
 * Class to access server data.
 *
 * @author Sulivan
 */
class Server{

  public function get_root_path() : string{
    return $_SERVER["DOCUMENT_ROOT"];
  }

  public function get_root_url() : string{

    $ssl = $this->get_item("HTTPS");
    $ssl_enabled = !empty($ssl) && $ssl == "on";

    $protocol = "http".($ssl_enabled ? "s" : "");

    $port = $this->get_item("SERVER_PORT");
    $port = (!$ssl_enabled && $port=="80") || ($ssl_enabled && $port =="443") ? "" : ":".$port;

    $host = $protocol."://".$this->get_item("SERVER_NAME").$port;

    return $host;
  }

  public function get_item(string $key, ?string $default = null) : ?string{
    return isset($_SERVER[$key]) ? $_SERVER[$key] : $default;
  }


}
