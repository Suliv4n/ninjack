<?hh
namespace ninjack\core;

class Configuration{
  private string $file;
  private array<string, object> $variables;

  protected function __construct(string $file){
    $this->file = $file;
    $this->variables = array();
  }

  public static function load(string $file) : Configuration{
    $configuration = new Configuration(CONF_PATH.$file);

    $configuration->include_file();

    return $configuration;
  }

  public function get_file() : string{
    return $this->file;
  }

  private function include_file(){
    require($this->file);
    $this->variables = get_defined_vars();
  }

  public function get($variable) : mixed{
    return $this->variables[$variable];
  }

}
