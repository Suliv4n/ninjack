<?hh
namespace Ninjack\Core\Display;

interface AssetsCompiler{
  public function initialize() : void;
  public function compile(string $filename) : string;
  public function get_target_extension() : string;
}
