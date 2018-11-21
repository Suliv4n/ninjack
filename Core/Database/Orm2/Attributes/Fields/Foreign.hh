<?hh
namespace Ninjack\Core\Database\Orm2\Attributes\Fields;

class Foreign extends Field
{
  <<__LateInit>>
  private string $target_classname;

  public function set_target_classname(string $classname) : void
  {
    $this->target_classname = $classname;
  }

  public function get_target_classname() : string
  {
    return $this->target_classname;
  }
}
