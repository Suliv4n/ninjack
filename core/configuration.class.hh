<?hh
namespace Ninjack\Core;

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
  private Map<string, object> $variables;

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
   * Include the configuration file and store variables.
   */
  private function include_file() : void{
    require($this->file);
    $this->variables = get_defined_vars();
  }

  /**
   * Return the configuration variable by name.
   *
   * @param string $variable name of the variable.
   *
   * @return mixed the requested variable value.
   */
  public function get(string $variable) : mixed{
    return $this->variables[$variable];
  }

  public function get_string(string $variable, ?string $default = null) : ?string{
    if($this->variables[$variable] == null){
      return $default;
    }

    return strval($this->variables[$variable]);
  }

}
