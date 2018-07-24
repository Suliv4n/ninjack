<?hh
namespace Ninjack\Core\Helper;

/**
 * Reprensents a classic tree.
 */
class TreeNode<T>{


  private ?T $value = null;
  private Vector<TreeNode<T>> $children;
  private ?TreeNode<T> $parent = null;

  private int $current = 0;

  /**
   * The constructor.
   *
   * @param ?T value the value of the Node.
   * @param ?TreeNode<T> $parent The parent of the node
   */
  public function __construct(?T $value = null, ?TreeNode<T> $parent = null){
    $this->value = $value;
    $this->children = Vector{};
    $this->parent = $parent;
  }

  /**
   * Returns the node value.
   *
   * @return ?T The node value.
   */
  public function get_value() : ?T {
    return $this->value;
  }

  /**
   * Returns the node level in the tree.
   *
   * @return ?T The node level.
   */
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

  /**
   * Tests if the node is a leaf.
   *
   * @return bool True if the node is a leaf, else false.
   */
  public function is_leaf() : bool{
    return $this->children->count() === 0;
  }

  /**
   * Returns all the leaves node in the tree.
   *
   * @return Vector<TreeNode<T>> All leaves in the tree.
   */
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

  /**
   * Join the parent nodes from this node to returns a string value containing all string representations
   * of its parent node values.
   *
   * @param string $glue The string between node values.
   * @param bool $include_root True if the root should be included, else false.
   *
   * @return string The joined node values string.
   */
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
