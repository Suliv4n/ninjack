<?hh
namespace Ninjack\Core;

use Ninjack\Core\Loader as Loader;
use Ninjack\Core\View as View;

/**
 * Represents a Http response.
 *
 * @author Sulivan
 */
class Response{

  /**
   * The view (body) of the response.
   */
  private View $view;

  /**
   * The content type of the response.
   */
  private string $content_type;

  /**
   * The constructor.
   * @param Ninjack\Core\View the view of the response.
   * @param the constent type of the response. By default "text/html".
   */
  public function __construct(View $view, string $content_type = "text/html"){
    $this->view = $view;
    $this->content_type = $content_type;
  }

  /*
   * Print the view.
   */
  public function render() : void{
    $this->view->render();
  }

}
