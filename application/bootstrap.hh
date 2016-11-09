<?hh
define("DS", DIRECTORY_SEPARATOR);
define("ROOT", dirname(__FILE__).DS);
define("NINJACK_PATH", "/var/www/ninjack".DS);
define("CORE_PATH", NINJACK_PATH."core".DS);


define("APPLICATION_PATH", ROOT."application".DS);
define("WIDGET_PATH", APPLICATION_PATH."widget".DS);

define("CONTROLLER_PATH", APPLICATION_PATH."controller".DS);
define("VIEW_PATH", APPLICATION_PATH."view".DS);
define("FORM_PATH", APPLICATION_PATH."form".DS);
define("COMMAND_PATH", APPLICATION_PATH."command".DS);
define("CORE_VIEW_PATH", ROOT."core".DS."view".DS);
define("THEME_PATH", APPLICATION_PATH."theme".DS);

require_once CORE_PATH."autoloader.class.hh";
Ninjack\Core\AutoLoader::register();

//composer autoload
require __DIR__ . '/vendor/autoload.php';
