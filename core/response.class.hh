<?hh
namespace Ninjack\Core;

use Ninjack\Core\Loader as Loader;

class Response{

  private string $view_name;
  private string $content_type;
  private Map<string, mixed> $variables;

  public function __construct(string $view_name, Map<string, mixed> $variables = Map{}, string $content_type = "text/html"){
    $this->view_name = $view_name;
    $this->content_type = $content_type;
    $this->variables = $variables;
  }

  public function render(){

  }

  public function get_render() : string{
    $render = Loader::load_view($this->view_name, $this->variables);

    return $render;
  }

}
