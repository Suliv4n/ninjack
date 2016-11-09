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

}
