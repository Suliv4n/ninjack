<?hh
define("NINJACK_PATH", "/var/www/ninjack/");

require(__DIR__ . "/../vendor/autoload.php");
require(__DIR__ . "/../Core/Autoloader.hh");

Ninjack\Core\Autoloader::add_scope("Ninjack\Test", __DIR__ );
Ninjack\Core\Autoloader::register();
