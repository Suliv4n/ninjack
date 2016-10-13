<?hh
namespace Ninjack\Core\User;
use Ninjack\Core\User\RoleInterface as RoleInterface;

interface UserInterface{
    public function get_roles() : Vector<RoleInterface>;
    public function get_username() : string;
}
