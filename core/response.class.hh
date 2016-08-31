<?hh
namespace ninjack\core;

use ninjack\core\Loader as Loader;

class Response{

  private string $view_name;
  private string $content_type;

  public function __construct(string $view_name, Map<string, string> $variables = Map{}, string $content_type = "text/html"){
    $this->view_name = $view_name;
    $this->content_type = $content_type;
  }

  public function render(){

  }

  public function get_render() : string{
    $render = Loader::load_view($this->view_name);

    return $render;
  }

}
