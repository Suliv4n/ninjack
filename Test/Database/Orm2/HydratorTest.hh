<?hh
require(__DIR__ . "/../../bootstrap.hh");

use function Facebook\FBExpect\expect;
use namespace Facebook\CLILib\TestLib;
use type Facebook\HackTest\HackTest;

use Ninjack\Core\Database\Orm2\Hydrator;

use Ninjack\Test\Database\Orm2\Entity\User;

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

  public function test_composite_hydrate()
  {
    
  }

}
