<?hh
namespace Ninjack\Core\Database;
use Ninjack\Core\Database\Column as Column;

/**
 * Represents a database table
 *
 * @author Sulivan
 */
class Table {

  /**
   * The contructor.
   *
   * @param string $name The table name.
   * @param Vector<Ninjack\Core\Database\Column>  $columns The table columns
   */
  public function __construct(
    private string $name,
    private Vector<Column> $columns = Vector {}
  )
  {
  }


  /**
   * Returns the table name.
   *
   * @return string The table name.
   */
  public function get_name() : string
  {
    return $this->name;
  }

  /**
   * Returns the columns table.
   *
   * @return string The table columns.
   */
  public function get_columns() : Vector<Column>
  {
    return $this->columns;
  }

}
