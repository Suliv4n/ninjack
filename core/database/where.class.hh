<?hh
namespace Ninjack\Core\Database;
use Ninjack\Core\Enum\DBOperator as DBOperator;

class Where{
  public function __construct(
    private string $column,
    private mixed $value,
    private DBOperator $operator = DBOperator::EQUALS
  ){}

    public function get_column() : string {
      return $this->column;
    }

    public function get_value() : mixed {
      return $this->value;
    }

    public function get_operator() : DBOperator {
      return $this->operator;
    }


}
