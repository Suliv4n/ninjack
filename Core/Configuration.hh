<?hh
namespace Ninjack\Core;
use Ninjack\Core\Application as Application;


/**
 * Loads configurations file and stores its variables.
 *
 * @author Sulivan
 */
class Configuration{

  /**
   * The coniguration file name.
   */
  private string $file;

  /**
   * The stored variables from the configuation file.
   */
  private Map<string, mixed> $variables;

  /**
   * Constructor of COnfiguration.
   *
   * @param string $file the name of the file to load.
   */
  private function __construct(string $file){
    $this->file = $file;
    $this->variables = new Map(null);
  }

  /**
   * Load the configuration file.
   *
   * @param string $file name of the file to load.
   *
   * @return Ninjack\Core\Configuration the configuration object loaded.
   */
  public static function load(string $file) : Configuration{
    $configuration = new Configuration($file);

    $configuration->include_file();

    return $configuration;
  }

  /**
   * Returns the file name of the configuration.
   *
   * @return string the file name of the configuration.
   */
  public function get_file() : string{
    return $this->file;
  }


  /**
   * Return the path of the configuration from parent applications.
   *
   * @return the path of the configuration from parent applications.
   */
  private function get_parent_file() : ?string{

    $current_filepath = debug_backtrace(DEBUG_BACKTRACE_IGNORE_ARGS, 1)[0]["file"];

    $relative_path = Application::get_instance()->belong_to($current_filepath);

    if($relative_path[0] === null){
      return null;
    }

    $parent_configuration_path = Application::get_instance()->get_file_from_parent_applications(strval($relative_path[1]), $relative_path[0]);

    return $parent_configuration_path;
  }

  /**
   * Include the configuration file and store variables.
   */
  private function include_file(?string $file = null) : void{
    $file = $file === null ? $this->file : $file;
    require($this->file);
    $this->variables = get_defined_vars();
  }

  /**
   * Return the configuration variable by name.
   *
   * @param string $variable name of the variable.
   * @param mixed the default value.
   *
   * @return mixed the requested variable value or the default value if
   * the value was not found.
   */
  public function get(string $variable, mixed $default = null) : mixed{
    if(isset($this->variables[$variable])){
      return $this->variables[$variable];
    }

    return $default;
  }


  /**
   * Return string representation of the configuration variable by name.
   *
   * @param string $variable name of the variable.
   * @param ?string the default value.
   *
   * @return ?string the requested variable value or the default value if
   * the value was not found.
   */
  public function get_string(string $variable, ?string $default = null) : ?string{
    if(isset($this->variables[$variable])){
      return strval($this->variables[$variable]);
    }

    return $default;
  }



}
