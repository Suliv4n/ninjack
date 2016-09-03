<?hh
namespace Ninjack\Core;

use Ninjack\Core\Loader as Loader;
use Ninjack\Core\View as View;

class Response{

  private View $view;
  private string $content_type;

  public function __construct(View $view, string $content_type = "text/html"){
    $this->view = $view;
    $this->content_type = $content_type;
  }

  public function render() : void{
    $this->view->render();
  }

}
