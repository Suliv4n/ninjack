<?hh
namespace Ninjack\Core\User;

/**
 * Interface that represents a Role.
 *
 * @author Sulivan
 */
interface RoleInterface{

    /**
     * Returns the name of the role.
     *
     * @return string the name of the role.
     */
    public function get_name() : string;
}
