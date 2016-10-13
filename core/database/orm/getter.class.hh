<?hh
namespace Ninjack\Core\Database\Orm;

class Getter{


  public function __construct(
    private string $column,
    private \ReflectionMethod $method,
    private bool $primary_key
  ){

  }

  public function is_primary_key() : bool{
    return $this->primary_key;
  }

  public function invoke(Entity $entity) : mixed{
    return $this->method->invoke($entity);
  }

}
