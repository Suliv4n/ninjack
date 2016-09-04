<?hh
define("DS", DIRECTORY_SEPARATOR);
define("ROOT", dirname(__FILE__).DS);
define("APPLICATION_PATH", ROOT."application".DS);
define("CONF_PATH", APPLICATION_PATH."conf".DS);
define("CONTROLLER_PATH", APPLICATION_PATH."controller".DS);
define("VIEW_PATH", APPLICATION_PATH."view".DS);
define("CORE_VIEW_PATH", ROOT."core".DS."view".DS);

require_once ROOT."core/autoloader.class.hh";

Ninjack\Core\Autoloader::register();
Ninjack\Core\Error::get_instance()->set_exception_handler();
Ninjack\Core\Application::get_instance()->run();
