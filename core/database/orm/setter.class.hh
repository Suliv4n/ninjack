<?hh
namespace Ninjack\Core\Database\Orm;
use Ninjack\Core\TypeHelper as TypeHelper;
use Ninjack\Core\Database\Entity as Entity;
use Ninjack\Core\Database\SQLWhere as SQLWhere;
use Ninjack\Core\Enum\DBOperator as DBOperator;


class Setter{


  public function __construct(
    private string $column,
    private \ReflectionMethod $method,
    private ?string $foreign_key = null,
    private ?string $onetomany = null,
    private ?string $intertable = null
  ){

  }


  public function invoke(Entity $entity, mixed $value) : void{


    if($this->foreign_key != null){
      if(!$this->onetomany){
        $this->load_foreign_key($entity, $value);
      }
      else{
        $this->load_one_to_many($entity, $value);
      }
    }
    else{
      $this->method->invoke($entity, $value);
    }
  }

  private function load_foreign_key(Entity $entity, mixed $value) : void{
    $value = TypeHelper::bind_parameter_value_type($this->method->getParameters()[0], $value);
    $class_target = $this->method->getParameters()[0]->getType();
    //@todo throw exception if null or not entity
    if($class_target == null){
      throw new Exception("Invalid foreign key setter");
    }
    $values = Entity::get($class_target->__toString(), Vector{
      new SQLWhere($this->foreign_key ?? "", $value ), //$this->foreign_key can't be null here
    });
    $fk_entity = null;
    if(count($values) == 1){
      $fk_entity = $values[0];
    }

    if($fk_entity != null || $class_target->allowsNull()){
      $this->method->invoke($entity, $fk_entity);
    }
  }


  private function load_one_to_many(Entity $entity, mixed $value) : void{

    $type = $this->method->getParameters()[0]->getType();
    if($type == null){
      throw new Exception("No type in parameter of setter ".$this->column);
    }
    $generic = TypeHelper::get_generic_type($type);
    $type_str = TypeHelper::get_str_type($type);


    $target_class = new \ReflectionClass($type_str);

    if($target_class->implementsInterface("Indexish")){
      $collection = $target_class->newInstanceWithoutConstructor();
      $entities = Entity::get($generic ?? "", Vector{
        new SQLWhere($this->column, $value)
      });
      foreach ($entities as $value) {
        $collection[] = $value;
      }
      $this->method->invoke($entity, $entities);
    }

  }

  private function load_many_to_many(Entity $entity, mixed $value) : void{

    /*
    $type = $this->method->getParameters()[0]->getType();
    if($type == null){
      throw new Exception("No type in parameter of setter ".$this->column);
    }
    $generic = TypeHelper::get_generic_type($type);
    $type_str = TypeHelper::get_str_type($type);


    $target_class = new \ReflectionClass($type_str);

    if($target_class->implementsInterface("Indexish")){
      $collection = $target_class->newInstanceWithoutConstructor();
      $entities = Entity::get($generic ?? "", Vector{
        new SQLWhere($this->column, $value)
      });
      foreach ($entities as $value) {
        $collection[] = $value;
      }
      $this->method->invoke($entity, $entities);
    }
    */

    $type = $this->method->getParameters()[0]->getType();
    if($type == null){
      throw new Exception("No type in parameter of setter ".$this->column);
    }
    $generic = TypeHelper::get_generic_type($type);
    $type_str = TypeHelper::get_str_type($type);
    if($generic == null){
      throw new Exception("Missing generic : ".$this->column);
    }


    $database = Application::get_instance()->load_database(Entity::get_database($entity));
    $database?->initialize();

    if($database != null){                      //article_tag.tag_id                                         //article_tag             // article_tag.id_tag     //tag.id
      $stmt = $database->query_builder->select($this->column)->from(ClassMap::get_table($generic))->innerjoin($this->intertable, "$this->foreign_key = $this->onetomany")->get();


      $ids = Vector{};

      while($data = $stmt->fetch(\PDO::FETCH_COLUMN)){
        $ids->($data[0]);
      }

      Entity::get($generic, Vector{
        new SQLWhere($this->onetomany ?? "", "(".implode(",",$ids).")", DBOperator::IN ),
      });


    }

  }


  public function is_one_to_many() : bool{
    return $this->onetomany != null && $this->intertable == null;
  }

  public function is_many_to_many() : bool{
    return $this->onetomany != null && $this->intertable != null;
  }

  public function get_foreign_column() : ?string{
    return $this->foreign_key;
  }

}
