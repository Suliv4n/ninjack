<?hh
namespace Ninjack\Core\Database;
use Ninjack\Core\Enum\DBOperator as DBOperator;
use Ninjack\Core\Database\DBConnector as DBConnector;


type Where = shape(
  "column" => string,
  "value" => mixed,
  "operator" => DBOperator,
);

class QueryBuilder{

  private Map<string,string> $selected_colomuns;
  private Map<string,string> $from;
  private Map<string,Map<string,mixed>> $update;
  private Map<string,Map<string,mixed>> $insert;
  private Vector<Where> $where;

  private Vector<mixed> $variables;

  private DBConnector $connector;

  public function __construct(DBConnector $connector){
    $this->selected_colomuns = new Map(null);
    $this->from = new Map(null);
    $this->update = new Map(null);
    $this->insert = new Map(null);
    $this->where = new Vector(null);
    $this->variables = new Vector(null);

    $this->connector = $connector;
  }

  public function select(string $column, ?string $alias = null) : QueryBuilder{

    $this->selected_colomuns[$column] = $alias ?? "";

    return $this;
  }

  public function from(string $table, ?string $alias = null) : QueryBuilder{

    $this->from[$table] = $alias ?? "";

    return $this;
  }

  public function update(string $table, Map<string,mixed> $values) : QueryBuilder{
    $this->update[$table] = $values;

    return $this;
  }

  public function insert(string $table, Map<string,mixed> $values) : QueryBuilder{
    $this->insert[$table] = $values;

    return $this;
  }


  public function where(string $column, mixed $value, DBOperator $operator = DBOperator::EQUALS) : QueryBuilder{

    $this->where->add(
      shape(
        "column" => $column,
        "operator" => $operator,
        "value" => $value,
      )
    );

    return $this;
  }

  public function get_query() : string{
    $query  = "";
    $query .= $this->__generate_select_query();
    $query .= $this->__generate_from_query();
    $query .= $this->__generate_update_query();
    $query .= $this->__generate_where_query();
    $query .= $this->__generate_insert_query();

    return $query;
  }

  private function __generate_select_query() : string{
    $query = "";

    if(!$this->selected_colomuns->isEmpty()){
      $query = "select\n";
      foreach($this->selected_colomuns as $column => $alias){
        if(!empty($alias)){
          $query .= $column." as ".$alias.",\n";
        }
        else{
          $query .= $column."\n";
        }
      }
      $query = preg_replace("-,$-", "", $query);
    }

    return $query;
  }


  private function __generate_from_query() : string{
    $query = "";

    if(!$this->from->isEmpty()){
      $query = "from\n";
      foreach($this->from as $table => $alias){
        if(!empty($alias)){
          $query .= $table." as ".$alias.",\n";
        }
        else{
          $query .= $table."\n";
        }
      }
      $query = preg_replace("-,$-", "", $query);
    }

    return $query;
  }

  public function __generate_update_query() : string{
    $query = "";

    foreach ($this->update as $table => $columns){
      $query = "update ".$table." set \n";
      foreach($columns as $column => $value){
        $query .= " `".$column."` = ?,\n";
        $this->variables->add($value);
      }
      $query = preg_replace("-,\n$-", "", $query);
    }

    return $query;
  }

  public function __generate_insert_query() : string{
    $query = "";

    foreach ($this->insert as $table => $columns){
      $values = str_repeat("?,",$columns->count());
      $values = preg_replace("-,$-", "", $values);

      $query = "insert into ".$table."(".implode(",", $columns->keys()).") VALUES( ".$values." )";
      $this->variables->addAll($columns->values());
    }

    return $query;
  }

  public function __generate_where_query() : string{
    $query = "";

    $first = true;

    foreach ($this->where as $where){
      if($first){
        $query = "\nwhere \n";
      }
      else{
        $query .= "\nAND ";
      }

      $query .= $where["column"]." ".$where["operator"]. " ? ";
      $first = false;
      $this->variables->add($where["value"]);
    }

    return $query;
  }



  public function execute() : bool{
    return $this->connector->execute($this->get_query(), $this->variables);
  }

  public function get() : \PDOStatement{
    return $this->connector->query($this->get_query(), $this->variables)[1];
  }


}
