<?hh
namespace Ninjack\Core\Helper;

class TreeNode<T>{


  private ?T $value = null;
  private Vector<TreeNode<T>> $children;
  private ?TreeNode<T> $parent = null;

  private int $current = 0;

  public function __construct(?T $value = null, ?TreeNode<T> $parent = null){
    $this->value = $value;
    $this->children = Vector{};
    $this->parent = $parent;
  }

  public function get_value() : ?T {
    return $this->value;
  }

  public function get_level() : int{
    if($this->parent === null){
      return 0;
    }

    $level = 0;

    $current_node = $this;
    do{
      $current_node = $current_node->parent;
      $level++;
    }while($current_node != null && $current_node->parent !== null);

    return $level;
  }

  public function add_child(T $value) : TreeNode<T>{
    $node = new TreeNode<T>($value, $this);
    $this->children[] = $node;
    return $node;
  }

  //test
  public function print() : void{
    echo str_repeat("\t", $this->get_level())."|_".strval($this->value)."\n";

    foreach($this->children as $child){
      $child->print();
    }
  }

  public function is_leaf() : bool{
    return $this->children->count() === 0;
  }

  public function get_leaves() : Vector<TreeNode<T>>{
    $leaves = Vector{};

    foreach($this->children as $child){
      if($child->is_leaf()){
        $leaves->add($child);
      }
      else{
        $leaves->addAll($child->get_leaves());
      }
    }

    return $leaves;
  }

  public function join_parent_nodes(string $glue = "", bool $include_root = true) : string{
    $parents = Vector{strval($this->get_value())};

    $current_node = $this->parent;

    while($current_node !== null){
      if($include_root || $current_node->parent !== null){
        $parents->add(strval($current_node->get_value()));
      }
      $current_node = $current_node->parent;
    }

    return implode(array_reverse($parents), $glue);
  }

}
