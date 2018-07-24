<?hh
define("DS", DIRECTORY_SEPARATOR);
define("ROOT", dirname(__FILE__).DS);
define("NINJACK_PATH", "/var/www/ninjack".DS);
define("CORE_PATH", NINJACK_PATH."Core".DS);


//@todo faudrait faire un truc ici pour savoir quel path utilisé pour l'application
//define("APPLICATION_PATH", ROOT."application".DS);
define("APPLICATION_PATH", ROOT.DS);
define("WIDGET_PATH", APPLICATION_PATH."widget".DS);

define("CONTROLLER_PATH", APPLICATION_PATH."controller".DS);
define("VIEW_PATH", APPLICATION_PATH."view".DS);
define("FORM_PATH", APPLICATION_PATH."form".DS);
define("COMMAND_PATH", APPLICATION_PATH."command".DS);
define("CORE_VIEW_PATH", ROOT."core".DS."view".DS);
define("THEME_PATH", APPLICATION_PATH."theme".DS);

require_once CORE_PATH . "Autoloader.hh";
Ninjack\Core\Autoloader::register();

//composer autoload
$vendor_autoload_path = __DIR__ . '/vendor/autoload.php';
if(file_exists($vendor_autoload_path)){
    require $vendor_autoload_path;
}
