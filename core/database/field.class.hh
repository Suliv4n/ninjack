<?hh
namespace Ninjack\Core\Database;

abstract class Field{


  public abstract function set_value(?string $value) : void;
}
