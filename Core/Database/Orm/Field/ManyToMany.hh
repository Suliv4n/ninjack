<?hh
namespace Ninjack\Core\Database\Orm\Field;
use Ninjack\Core\Application as Application;
use Ninjack\Core\Database\Entity as Entity;
use Ninjack\Core\Database\SQLWhere as SQLWhere;
use Ninjack\Core\Database\Orm\Field\Field as Field;
use Ninjack\Core\Enum\DBOperator as DBOperator;
use Ninjack\Core\TypeHelper as TypeHelper;

class ManyToMany extends Field{

  private string $source_column;
  private string $relation_table;
  private string $relation_column;
  private string $target_column;
  private string $target_key;

  public function __construct(string $source_column, string $relation_column, string $target_column, string $target_key){

    $relation_table = explode(".",$relation_column)[0];


    parent::__construct($relation_column);

    $this->relation_table = $relation_table;
    $this->relation_column = $relation_column;
    $this->target_column = $target_column;
    $this->target_key = $target_key;

    $this->source_column = $source_column;


  }

  <<__Override>>
  public function set(Entity $entity, mixed $value) : void{

    if($this->setter != null){
      $type = $this->setter->getParameters()[0]->getType();

      $ids = Vector{};

      if($value instanceof Traversable){
        $ids = $value;
      }
      else{
        $database = Application::get_instance()->load_database(Entity::get_database($entity));

        if($database == null){
          throw new Exception("No database found :  ".Entity::get_database($entity));
        }

        $database->initialize();
        $stmt = $database->query_builder()->select($this->target_column, "fk_id")->from($this->relation_table)->where($this->relation_column, $value)->get();

        //var_dump($database->query_builder()->select($this->target_column)->from($this->relation_table)->where($this->relation_column, $value)->get_query());


        while($row = $stmt->fetch(\PDO::FETCH_ASSOC)){
          $ids[] = $row["fk_id"];
        }
      }

      if($type == null){
        throw new Exception("No type in parameter of setter ".$this->name);
      }

      $generic = TypeHelper::get_generic_type($type);
      $type_str = TypeHelper::get_str_type($type);

      $target_class = new \ReflectionClass($type_str);

      if($generic == null){
        throw new Exception("No generic : ".$this->name);
      }



      if($target_class->implementsInterface("Indexish")){
        $collection = $target_class->newInstanceWithoutConstructor();

        $entities = Vector{};
        if(count($ids) > 0){
          $entities = Entity::get($generic, Vector{
            new SQLWhere($this->target_key, $ids, DBOperator::IN)
          });

          foreach ($entities as $value) {
            $collection[] = $value;
          }
        }

        if($this->setter != null){
          $this->setter->invoke($entity, $entities);
        }
      }

    }

  }

  <<__Override>>
  public function get_source_column() : string{
    return $this->source_column;
  }

  public function get_table() : string{
    return $this->relation_table;
  }

  public function get_left_column() : string{
    return $this->relation_column;
  }

  public function get_right_column() : string{
    return $this->target_column;
  }

}
