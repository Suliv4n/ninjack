<?hh
define("DS", DIRECTORY_SEPARATOR);
define("ROOT", dirname(__FILE__).DS);
define("APPLICATION_PATH", ROOT."application".DS);
define("WIDGET_PATH", APPLICATION_PATH."widget".DS);
define("CONF_PATH", APPLICATION_PATH."conf".DS);
define("CONTROLLER_PATH", APPLICATION_PATH."controller".DS);
define("VIEW_PATH", APPLICATION_PATH."view".DS);
define("FORM_PATH", APPLICATION_PATH."form".DS);
define("COMMAND_PATH", APPLICATION_PATH."command".DS);
define("CORE_VIEW_PATH", ROOT."core".DS."view".DS);

require_once ROOT."core/autoloader.class.hh";
Ninjack\Core\AutoLoader::register();

//composer auto load
require __DIR__ . '/vendor/autoload.php';