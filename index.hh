<?hh
define("DS", DIRECTORY_SEPARATOR);
define("ROOT", dirname(__FILE__).DS);
define("APPLICATION_PATH", ROOT."application".DS);
define("CONF_PATH", APPLICATION_PATH."conf".DS);
define("CONTROLLER_PATH", APPLICATION_PATH."controller".DS);
define("VIEW_PATH", APPLICATION_PATH."view".DS);

require_once ROOT."core/autoloader.class.hh";
ninjack\core\Autoloader::register();


ninjack\core\Application::get_instance()->run();
