<?hh
namespace Ninjack\Core\Display;

/**
 * Interface for assets compiler.
 *
 * @author Sulivan.
 */
interface AssetsCompiler{
  /**
   * Initialize the compiler.
   */
  public function initialize() : void;

  /**
   * Compiles the content of the file which the filename is passed in argument.
   *
   * @param string $filename the path of the file to compile.
   *
   * @return string the file compiled content.
   */
  public function compile(string $filename) : string;

  /**
   * Returns the target extension for generated file.
   * (for example ".css", ".js")
   *
   * @return string the target extension for generated file.
   */
  public function get_target_extension() : string;
}
