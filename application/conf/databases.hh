<?hh
use Ninjack\Core\Enum\DBDriver as DBDriver;

$databases = new Map(null);

$name = "main";
$databases[$name] = new Map(null);
$databases[$name]["dbdriver"] = DBDriver::MARIADB;
$databases[$name]["hostname"] = "127.0.0.1";
$databases[$name]["port"] = 3306;
$databases[$name]["database"] = "ninjack";
$databases[$name]["username"] = "root";
$databases[$name]["password"] = "root";
$databases[$name]["charset"] = "UTF8";
