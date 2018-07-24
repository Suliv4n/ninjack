<?hh
namespace Ninjack\Core\User;
use Ninjack\Core\User\RoleInterface as RoleInterface;

/**
 * Represents a user.
 *
 * @author Sulivan
 */
interface UserInterface{
    /**
     * Returns the roles of the user.
     *
     * @param Vector<RoleInterface> the roles of the user.
     */
    public function get_roles() : Vector<RoleInterface>;

    /**
     * Returns the username.
     *
     * @return the username.
     */
    public function get_username() : string;
}
