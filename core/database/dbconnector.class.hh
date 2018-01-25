<?hh
namespace Ninjack\Core\Database;

use Ninjack\Core\Enum\DBDriver as DBDriver;
use Ninjack\Core\Database\QueryBuilder as QueryBuilder;

/**
 * Interface between the application and a database using
 * the powerfull PDO.
 *
 * @author Sulivan
 */
class DBConnector{

  private ?\PDO $pdo;

  /**
   * The constructor.
   *
   * @param Ninjack\Core\Enum\DBDriver $dbdriver the driver to use.
   * @param string $hostname the hostname of database server.
   * @param int $port the port of the database server.
   * @param string $username the username to connect to the database.
   * @param string $password the password to connect to the database.
   * @param string $charset the charset to use (default is UTF8).
   */
  public function __construct(
    private DBDriver $dbdriver,
    private string $hostname,
    private int $port,
    private string $username,
    private string $password,
    private string $database,
    private string $charset = "UTF8"
  ){

  }

  /**
   * Initialise the connection to the database.
   */
  public function initialize() : void{
    $this->pdo = new \PDO($this->get_dsn(), $this->username, $this->password);
    //$this->pdo = new \PDO("mysql:host=127.0.0.1;port=3306;dbname=ninjack;charset=UTF8;", "root", "root");
  }

  /**
   * Returns the dsn switch the dbdriver to use.
   *
   * @return the dsn switch the dbdriver to use.
   */
  private function get_dsn() : string{
    $dsn = "";
    switch ($this->dbdriver) {
      case DBDriver::MARIADB:
      case DBDriver::MYSQL:
        $dsn = "mysql:host=".$this->hostname.";port=".$this->port.";dbname=".$this->database.";charset=".$this->charset.";";
        break;
    }

    return $dsn;
  }

  public function query(string $query, Vector<mixed> $variables = Vector{}) : (bool,\PDOStatement){
    if($this->pdo == null){
      throw new Exception();
    }
    $stmt = $this->pdo->prepare($query, $variables);
    $result = $stmt->execute($variables);
    return tuple($result, $stmt);

  }

  public function execute(string $query, ?Vector<mixed> $variables = null) : bool{

    if($this->pdo != null){
      $stmt = $this->pdo->prepare($query);
      $success =  $stmt->execute($variables);

      $stmt->closeCursor();
      return $success;
    }

    return false;
  }

  public function query_builder() : QueryBuilder{
    return new QueryBuilder($this);
  }

  public function get_last_insert_id() : mixed{
    if($this->pdo !== null){
      return $this->pdo->lastInsertId();
    }
  }

  public function get_error() : mixed {
    return $this->pdo->errorInfo();
  }

}
