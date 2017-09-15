<?hh
namespace Ninjack\Core\Database;
use Ninjack\Core\Enum\DataBaseColumnOption as DataBaseColumnOption;

/**
 * Represents a table column.
 *
 * @author Sulivan
 */
class Column {

  /**
   * The constructor.
   *
   * @param string $name the column name.
   * @param Ninjack\Core\Enum\DataBaseColumnOption $type The data type of the column.
   * @param ?int $length The data maximum length.
   * @param Vector $options The column options.
   */
  public function __construct(
    private string $name,
    private string $type,
    private ?int $length = null,
    private Vector<string> $options = Vector{}
  ){

  }

  /**
   * Returns a string representation of the column
   * that can be used in sql request (like create table)
   *
   * @return string the string representations of the column.
   */
  public function __toString() : string {
    $string = $this->name . " " . $this->type;

    if($this->length !== null)
    {
      $string .= "(" . $this->length . ")";
    }

    foreach($this->options as $option)
    {
      $string .= " ".$option;
    }

    return $string;
  }

}
