<?hh
define("DS", DIRECTORY_SEPARATOR);
define("ROOT", dirname(__FILE__).DS);
define("APPLICATION_PATH", ROOT."application".DS);
define("CONF_PATH", APPLICATION_PATH."conf".DS);
define("CONTROLLER_PATH", APPLICATION_PATH."controller".DS);

require_once ROOT."core/autoloader.class.hh";
ninjack\core\Autoloader::register();


echo ninjack\core\Application::get_instance()->run();
