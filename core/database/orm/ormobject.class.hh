<?hh
namespace Ninjack\Core\Database\Orm;
use Ninjack\Core\TypeHelper as TypeHelper;
use Ninjack\Core\Application as Application;
use Ninjack\Core\Database\Entity as Entity;
use Ninjack\Core\Database\SQLWhere as SQLWhere;
use Ninjack\Core\Database\Orm\Getter as Getter;
use Ninjack\Core\Database\Orm\Setter as Setter;
use Ninjack\Core\Database\Orm\Field\Field as Field;
use Ninjack\Core\Database\Orm\Field\ForeignKey as ForeignKey;
use Ninjack\Core\Database\Orm\Field\ManyToOne as ManyToOne;
use Ninjack\Core\Database\Orm\Field\ManyToMany as ManyToMany;
use Ninjack\Core\Database\DBConnector as DBConnector;


class ORMObject{

  const string DATABASE_ATTRIBUTE = "Database";
  const string TABLE_ATTRIBUTE = "Table";
  const string PRIMARY_ATTRIBUTE = "PrimaryKey";
  const string GETTER_ATTRIBUTE = "Get";
  const string SETTER_ATTRIBUTE = "Set";
  const string FOREIGN_KEY = "ForeignKey";
  const string ONE_TO_MANY = "OneToMany";
  const string MANY_TO_MANY = "ManyToMany";

  private Map<string,Field> $fields;

  private string $table = "";
  private string $database = "main";

  private \ReflectionClass $class;

  private ?\ReflectionMethod $constructor;
  private Map<string, \ReflectionParameter> $construct_parameters;


  public function __construct(string $class){
    $this->fields = Map{};
    $this->construct_parameters = new Map(null);
    $this->class = new \ReflectionClass($class);
  }

  public function map() : void{

    $methods = $this->class->getMethods();

    $this->table = self::get_table($this->class->getName());

    $database_attr = $this->class->getAttribute(self::DATABASE_ATTRIBUTE);
    if($database_attr != null && count($database_attr) == 1){
        $this->database = strval($database_attr[0]);
    }

    foreach ($methods as $method) {

      $get_attr = $method->getAttribute(self::GETTER_ATTRIBUTE);
      $set_attr = $method->getAttribute(self::SETTER_ATTRIBUTE);
      $fk_attribute = $method->getAttribute(self::FOREIGN_KEY);
      $fk_manytoone_attribute = $method->getAttribute(self::ONE_TO_MANY);
      $fk_manytomany_attribute = $method->getAttribute(self::MANY_TO_MANY);
      $pk_attribute = $method->getAttribute(self::PRIMARY_ATTRIBUTE);

      $is_primary_key = $pk_attribute !== null;

      if($get_attr != null && count($get_attr) == 1){
        $column = strval($get_attr[0]);
        if(!isset($this->fields[$column])){
          $this->fields[$column] = new Field($column);
        }

        if($is_primary_key){
          $this->fields[$column]->set_primary_key(true);
        }

        $this->fields[$column]->set_getter($method);
      }
      else if($set_attr != null && count($set_attr) == 1){
        $column = strval($set_attr[0]);
        if(!isset($this->fields[$column])){
          $this->fields[$column] = new Field($column);
        }

        if($is_primary_key){
          $this->fields[$column]->set_primary_key(true);
        }

        $this->fields[$column]->set_setter($method);
      }
      else if($fk_attribute != null && count($fk_attribute) == 2){
        $column = strval($fk_attribute[0]);
        if(!isset($this->fields[$column])){
          $this->fields[$column] = new ForeignKey($column, strval($fk_attribute[1]), $is_primary_key);
        }

        $this->fields[$column]->set_setter($method);
      }
      else if($fk_manytoone_attribute != null && count($fk_manytoone_attribute) == 2){
        $column = strval($fk_manytoone_attribute[1]);
        if(!isset($this->fields[$column])){
          $this->fields[$column] = new ManyToOne(strval($fk_manytoone_attribute[0]), $column);
        }

        $this->fields[$column]->set_setter($method);
      }
      else if($fk_manytomany_attribute != null && count($fk_manytomany_attribute) == 4){
        $column = strval($fk_manytomany_attribute[2]);
        if(!isset($this->fields[$column])){
          $this->fields[$column] = new ManyToMany(
            strval($fk_manytomany_attribute[0]),
            strval($fk_manytomany_attribute[1]),
            strval($fk_manytomany_attribute[2]),
            strval($fk_manytomany_attribute[3])
          );
        }
        $this->fields[$column]->set_setter($method);
      }
    }

    $this->constructor = $this->class->getConstructor();
    if($this->constructor != null){
      foreach ($this->constructor->getParameters() as $parameter) {
        $this->construct_parameters[$parameter->getName()] = $parameter;
      }
    }


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


  public function get(Vector<SQLWhere> $filters = Vector{}) : Vector<Entity>{

    //@todo get default database or mapped database
    $entites = Vector{};
    $database = Application::get_instance()->load_database($this->get_database());
    if($database != null){
      $database->initialize();
      //var_dump($database->query_builder()->select("*")->from($this->table)->many_where($filters)->get_query());
      $stmt = $database->query_builder()->select("*")->from($this->table)->many_where($filters)->get();



      while($row = $stmt->fetch(\PDO::FETCH_ASSOC)){

        $entity = $this->get_from_data(new Map($row));

        $entites->add($entity);

      }

    }

    return $entites;
  }


  public function get_from_data(Map<string, mixed> $data) : Entity{
    $already_set = Vector{};
    $arguments = Map{};

    foreach ($data as $column => $value) {

      if($this->construct_parameters->containsKey($column)){
        $already_set->add($column);
        $order = $this->construct_parameters->keys()->linearSearch($column);
        $arguments[$order] = TypeHelper::bind_parameter_value_type($this->construct_parameters[$column], $value);
      }

    }

    ksort($arguments);


    $entity = $this->class->newInstanceArgs($arguments);

    foreach ($this->fields as $column => $field) {

      if($already_set->linearSearch($column) == -1){
        if(isset($data[$column])){
          $field->set($entity, $data[$column]);
        }
        else if(isset($data[$field->get_source_column()])){
          $field->set($entity, $data[$field->get_source_column()]);
        }
      }

    }

    return $entity;
  }


  public function get_database() : string{
    return $this->database ?? "main";
  }

  public function get_fields() : Map<string,Field>{
    return $this->fields;
  }

  public function get_primary_key_value(Entity $entity) : mixed{
    foreach ($this->fields as $field) {
      if($field->is_primary_key()){
        return $field->get($entity);
      }
    }

    return null;
  }

  public function get_primary_key_field() : ?Field{
    foreach ($this->fields as $field) {
      if($field->is_primary_key()){
        return $field;
      }
    }

    return null;
  }

  public function save(Entity $entity) : void{
    $pk_value = $this->get_primary_key_value($entity);
    if($pk_value === null){
      $this->insert($entity);
    }
    else{
      $this->update($entity);
    }
  }

  public function insert(Entity $entity) : bool{
    $database = Application::get_instance()->load_database($this->get_database());

    if($database === null){
      throw new Exception("No database");
    }
    $database->initialize();

    $values = $this->get_column_values($entity);
    $many_to_many = $this->fields->filter($f ==> $f instanceof ManyToMany);


    $res = $database->query_builder()->insert($this->table, $values)->execute();
    if($res){
      $id = $database->get_last_insert_id();

      foreach ($many_to_many as $f) {
        //$this->clean_manytomany($database, $f, $id);
        $values = $f->get($entity);
        if($values instanceof Traversable){
          foreach ($values as $value) {
            if($f instanceof ManyToMany){
              $res = $res && $database->query_builder()->insert(
                $f->get_table(),
                Map{
                  $f->get_left_column() => $id,
                  $f->get_right_column() =>  Entity::get_orm(get_class($value))->get_primary_key_value($value),
                }
              )->execute();
            }
          }
        }
      }

    }

    return $res;
  }

  public function clean_manytomany(DBConnector $database, ManyToMany $field, mixed $id) : bool{
    return $database->query_builder()
      ->delete($field->get_table())
      ->where($field->get_name(), $id)
      ->execute();
  }

  public function update(Entity $entity) : bool{
    $database = Application::get_instance()->load_database($this->get_database());

    if($database === null){
      throw new Exception("No database");
    }
    $database->initialize();

    $values = $this->get_column_values($entity);
    $many_to_many = $this->fields->filter($f ==> $f instanceof ManyToMany);

    $primary = $this->get_primary_key_field();

    $res = false;
    if($primary !== null){
      $res = $database->query_builder()->update($this->table, $values)->where($primary->get_source_column(), $this->get_primary_key_value($entity))->execute();
      if($res){
        $id = $entity->get_primary_key_value();

        foreach ($many_to_many as $f) {
          $values = $f->get($entity);
          if($f instanceof ManyToMany){
              $res = $res && $this->clean_manytomany($database, $f, $id);
          }
          if($values instanceof Traversable){
            foreach ($values as $value) {
              if($f instanceof ManyToMany){
                $res = $res && $database->query_builder()->insert(
                  $f->get_table(),
                  Map{
                    $f->get_left_column() => $id,
                    $f->get_right_column() =>  Entity::get_orm(get_class($value))->get_primary_key_value($value),
                  }
                )->execute();
              }
            }
          }
        }

      }
    }
    return $res;
  }

  private function get_column_values(Entity $entity) : Map<string, mixed>{
    $values = Map{};
    foreach ($this->fields as $field) {
      if($field instanceof ForeignKey){
        $entity_fk = $field->get($entity);

        if($entity_fk instanceof Entity){
          $fk = Entity::get_orm(get_class($entity_fk))->get_primary_key_value($entity_fk);
          $values[$field->get_name()] = $fk;
        }
      }
      else if(get_class($field) == Field::class){
        $values[$field->get_name()] = $field->get($entity);
      }
    }

    return $values;
  }


}
