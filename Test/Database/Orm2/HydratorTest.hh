<?hh
require(__DIR__ . "/../../bootstrap.hh");

use function Facebook\FBExpect\expect;
use namespace Facebook\CLILib\TestLib;
use type Facebook\HackTest\HackTest;

use Ninjack\Core\Database\Orm2\Hydrator;

use Ninjack\Test\Database\Orm2\Entity\User;
use Ninjack\Test\Database\Orm2\Entity\Role;
use Ninjack\Test\Database\Orm2\Entity\UserWithRole;
use Ninjack\Test\Database\Orm2\Entity\UserWithRoles;

class HydratorTest extends HackTest
{

  public function test_simple_hydrate() : void
  {
    $hydrator = new Hydrator();

    $user_data = Map {
      "username" => "Henry"
    };

    $user = $hydrator->hydrate(User::class, $user_data);

    expect($user)->toBeInstanceOf(User::class);
    expect($user->get_username())->toBeSame($user_data["username"]);
  }

  public function test_composite_hydrate() : void
  {
    $hydrator = new Hydrator();

    $user_data = Map {
      "username" => "Henry",
      "role" => Map{
        "name" => "admin",
      },
    };

    $user = $hydrator->hydrate(UserWithRole::class, $user_data);

    expect($user->get_role())->toBeInstanceOf(Role::class);
    expect($user->get_role()->get_name())->toBeSame($user_data["role"]["name"]);
  }

  public function test_many_to_many_hydrate()
  {
    $hydrator = new Hydrator();

    $user_data = Map {
      "username" => "Henry",
      "roles" => Vector {
        Map {
          "name" => "admin",
        },
        Map {
          "name" => "author",
        }
      },
    };

    $user= $hydrator->hydrate(UserWithRoles::class, $user_data);

    expect($user)->toBeInstanceOf(UserWithRoles::class);
    expect($user->get_roles()->count())->toBeSame(2);
    expect($user->get_roles()[0]->get_name())->toBeSame($user_data["roles"][0]["name"]);
    expect($user->get_roles()[1]->get_name())->toBeSame($user_data["roles"][1]["name"]);
  }

}
