<?hh
namespace Ninjack\Core\Database\Orm;
use Ninjack\Core\TypeHelper as TypeHelper;
use Ninjack\Core\Application as Application;
use Ninjack\Core\Database\Entity as Entity;
use Ninjack\Core\Database\SQLWhere as SQLWhere;
use Ninjack\Core\Database\Orm\Getter as Getter;
use Ninjack\Core\Database\Orm\Setter as Setter;
use Ninjack\Core\Database\DBConnector as DBConnector;

class ClassMap{

  const string DATABASE_ATTRIBUTE = "Database";
  const string TABLE_ATTRIBUTE = "Table";
  const string PRIMARY_ATTRIBUTE = "PrimaryKey";
  const string GETTER_ATTRIBUTE = "Get";
  const string SETTER_ATTRIBUTE = "Set";
  const string FOREIGN_KEY = "ForeignKey";
  const string ONE_TO_MANY = "OneToMany";
  const string MANY_TO_MANY = "ManyToMany";

  private \ReflectionClass $class;

  private ?string $database;
  private Set<string> $columns;
  private string $table = "";

  private Map<string, Getter> $getters;
  private Map<string, Setter> $setters;

  private ?\ReflectionMethod $constructor;
  private Map<string, \ReflectionParameter> $construct_parameters;

  public function __construct(string $class){
    $this->getters = new Map(null);
    $this->setters = new Map(null);
    $this->columns = Set{};
    $this->construct_parameters = new Map(null);
    //@todo throw exception when class does not exists or does not extend Entity
    $this->class = new \ReflectionClass($class);
  }


  public function map() : void{



    $database_attr = $this->class->getAttribute(self::DATABASE_ATTRIBUTE);

    $methods = $this->class->getMethods();

    $this->table = self::get_table(self::class);

    if($database_attr != null && count($database_attr) == 1){
        $this->database = strval($database_attr[0]);
    }


    foreach ($methods as $method) {

      $get_attr = $method->getAttribute(self::GETTER_ATTRIBUTE);
      $set_attr = $method->getAttribute(self::SETTER_ATTRIBUTE);
      $fk_attribute = $method->getAttribute(self::FOREIGN_KEY);
      $fk_manytoone_attribute = $method->getAttribute(self::ONE_TO_MANY);
      $fk_manytomany_attribute = $method->getAttribute(self::MANY_TO_MANY);


      if($get_attr != null && count($get_attr) == 1){
        $column = strval($get_attr[0]);
        $primary_key = isset($method->getAttribute(self::PRIMARY_ATTRIBUTE));
        $this->getters[$column] = new Getter($column, $method, $primary_key);
        $this->columns->add($column);
      }
      else if($set_attr != null && count($set_attr) == 1){
        $column = strval($set_attr[0]);
        $this->setters[$column] = new Setter($column, $method);
        $this->columns->add($column);

      }
      else if($fk_attribute != null && count($fk_attribute) == 2){
        $column = strval($fk_attribute[0]);
        $this->setters[$column] = new Setter($column, $method, strval($fk_attribute[1]));
        $this->columns->add($column);
      }
      else if($fk_manytoone_attribute != null && count($fk_manytoone_attribute) == 2){
        $column = strval($fk_manytoone_attribute[0]);
        $this->setters[$column] = new Setter($column, $method, strval($fk_manytoone_attribute[1]), strval($fk_manytoone_attribute[0]));
        $this->columns->add($column);
      }
      else if($fk_manytomany_attribute != null && count($fk_manytomany_attribute) == 4){
        $column = strval($fk_manytomany_attribute[1]);
        $this->setters[$column] = new Setter($column, $method, strval($fk_manytomany_attribute[2]), strval($fk_manytomany_attribute[0]));
        $this->columns->add($column);
      }

    }
  }

  public function get(Vector<SQLWhere> $filters = Vector{}) : Vector<Entity>{

    //@todo get default database or mapped database
    $entites = Vector{};
    $database = Application::get_instance()->load_database("main");
    if($database != null){
      $database->initialize();
      $stmt = $database->query_builder()->select("*")->from($this->table)->many_where($filters)->get();


      while($row = $stmt->fetch(\PDO::FETCH_ASSOC)){
        $use_setters = Map{};
        $arguments = Map{};
        foreach ($row as $column => $value) {
          if($this->construct_parameters->containsKey($column)){
            $arguments[$column] = TypeHelper::bind_parameter_value_type($this->construct_parameters[$column], $value);
          }
          else if($this->setters->containsKey($column)){
            $use_setters[$column] = $value;
          }
        }

        $entity = $this->class->newInstanceArgs($arguments);
        foreach ($this->setters as $column => $setter) {

          if($use_setters->containsKey($column)){
            $this->setters[$column]->invoke($entity, $use_setters[$column]);
          }
          else if(!$arguments->containsKey($column)){
            //cas one to many
            $this->setters[$column]->invoke($entity, $row[$setter->get_foreign_column()]);
          }
        }

        $entites->add($entity);

      }

    }

    return $entites;
  }

  private function get_primary_keys_getters() : Map{
    return $this->getters->filter( $getter ==> $getter->is_primary_key() );
  }

  public function get_database() : string{
    return $this->database ?? "main";
  }

  public static function get_table(string $entity_class) : string{
    $class = new \ReflectionClass($entity_class);
    $table_attr = $class->getAttribute(self::TABLE_ATTRIBUTE);
    if($table_attr != null && count($table_attr) == 1){
      $table = strval($table_attr[0]);
    }
    else{
        $table = $class->getName();
    }

    return $table;
  }

}
