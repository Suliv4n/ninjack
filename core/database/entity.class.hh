<?hh
namespace Ninjack\Core\Database;
use Ninjack\Core\Application as Application;
use Ninjack\Core\TypeHelper as TypeHelper;

abstract class Entity{

  const string DATABASE_ATTRIBUTE = "Database";
  const string TABLE_ATTRIBUTE = "Table";
  const string PRIMARY_ATTRIBUTE = "PrimaryKey";
  const string GETTER_ATTRIBUTE = "Get";

  public static function get(string $class) : Vector<Entity>{

    $entities = Vector{};

    $class_reflection = new \ReflectionClass($class);

    $database_name_attribute = $class_reflection->getAttribute(self::DATABASE_ATTRIBUTE);


    $table_attribute = $class_reflection->getAttribute(self::TABLE_ATTRIBUTE);


    if($table_attribute !== null && count($table_attribute) == 1){
      $table = strval($table_attribute[0]);
    }
    else{
      $table = $class_reflection->getName();
    }

    if($database_name_attribute !== null && count($database_name_attribute) == 1){
      $database_name = strval($database_name_attribute[0]);
    }
    else{
      $database_name = "main";
    }

    $constructor = $class_reflection->getConstructor();
    $attributes = $class_reflection->getProperties();

    $database = Application::get_instance()->load_database($database_name);

    if($database !== null){
      $database->initialize();

      $query = "select * from ".$table;
      $result = $database->query($query);

      if($constructor !== null){
        while($row = $result->fetch()){
          $constructor_args = new Map(null);
          foreach($constructor->getParameters() as $parameter){
            $constructor_args[$parameter->getName()] = TypeHelper::bind_parameter_value_type($parameter, $row[$parameter->getName()]);
          }
          $entities->add($class_reflection->newInstanceArgs($constructor_args));
        }
      }
    }

    return $entities;
  }


  private function __get_table() : string{
    $class = new \ReflectionClass($this);
    $table_attribute = $class->getAttribute(self::TABLE_ATTRIBUTE);

    if($table_attribute !== null && count($table_attribute) == 1){
      $table = strval($table_attribute[0]);
    }
    else{
      $table = $class->getName();
    }

    return $table;
  }

  private function __get_getters() : Map<string, \ReflectionMethod>{

    $getters = new Map(null);

    $class = new \ReflectionClass($this);
    $methods = $class->getMethods();

    foreach($methods as $method){
      $attribute = $method->getAttribute(self::GETTER_ATTRIBUTE);

      if($attribute != null && count($attribute) == 1){

        $getters[strval($attribute[0])] = $method;
      }

    }

    return $getters;
  }


  private function __get_primary_keys_getters() : Map<string, \ReflectionMethod>{
    $primary_keys = new Map(null);

    foreach ($this->__get_getters() as $column => $method) {
        if($method->getAttribute(self::PRIMARY_ATTRIBUTE) !== null){
          $primary_keys[$column] = $method;
        }
    }

    return $primary_keys;
  }

  private function __get_database() : ?DBConnector{
    $class_reflection = new \ReflectionClass($this);

    $database_name_attribute = $class_reflection->getAttribute(self::DATABASE_ATTRIBUTE);


    if($database_name_attribute !== null && count($database_name_attribute) == 1){
      $database_name = strval($database_name_attribute[0]);
    }
    else{
      $database_name = "main";
    }

    return Application::get_instance()->load_database($database_name);
  }

  public final function save() : void{
    $primary_keys = new Map(null);
    $columns = Map{};

    $primary_keys_getters = $this->__get_primary_keys_getters();

    foreach ($this->__get_getters() as $column => $method) {
      $value = $method->invoke($this);
      if($primary_keys_getters->containsKey($column) && $value != null){
        $primary_keys[$column] = $value;
      }
      $columns[$column] = $value;
    }

    $db = $this->__get_database();
    $table = $this->__get_table();

    if($db == null){
      throw new Exception("No db");
    }

    $db->initialize();

    if(!$primary_keys->isEmpty()){
      $query = $db->query_builder()->update($table, $columns);

      foreach ($primary_keys as $column => $value) {
        $query->where($column, $value);
      }

      $query->execute();
    }
    else{
      $db->query_builder()->insert($table, $columns)->execute();
    }

    var_dump($primary_keys);
  }


}
