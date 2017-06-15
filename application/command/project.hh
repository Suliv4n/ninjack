<?hh
namespace Application\Ninjack\Application\Command;
use Ninjack\Core\Cli\Command as Command;
use Ninjack\Core\Application as Application;
use Ninjack\Core\Helper\TreeNode as TreeNode;
use Ninjack\Core\Helper\File as File;
use Ninjack\Core\Enum\CLIFGColor as CLIFGColor;

class Project extends Command{

  private TreeNode<string> $project_arborescence;

  public function __construct(){

    parent::__construct();

    $this->project_arborescence = new TreeNode<string>();

    $this->project_arborescence->add_child("assets");
    $this->project_arborescence->add_child("command");
    $this->project_arborescence->add_child("conf");
    $this->project_arborescence->add_child("controller");
    $this->project_arborescence->add_child("entity");
    $this->project_arborescence->add_child("form");
    $this->project_arborescence->add_child("public");
    $this->project_arborescence->add_child("theme");
    $this->project_arborescence->add_child("view");

  }

  <<Command>>
  public function create(string $project_path) : void{

    $project_path = File::realpath($project_path);
    $warning = false;
    $error = false;

    if(!file_exists($project_path)){

      if(mkdir($project_path, 0777, true)){

        foreach($this->project_arborescence->get_leaves() as $leaf){
          $folder_path = $project_path.DIRECTORY_SEPARATOR.$leaf->join_parent_nodes(DIRECTORY_SEPARATOR, false);
          if(!mkdir($folder_path, 0777, true))
          {
            $warning = true;
            $this->console->println("Warning : folder '$folder_path' could not be created.", CLIFGColor::YELLOW);
          }
        }

        $ninjack_application_path = NINJACK_PATH."application".DIRECTORY_SEPARATOR;

        $files_to_copy = Vector{
          "bootstrap.hh",
          "ninja",
          "variables_loader.php",
          "public/index.hh",
        };

        foreach($files_to_copy as $file){
          if(!copy(
            $ninjack_application_path.$file,
            $project_path.DIRECTORY_SEPARATOR.$file,
          )){
              $warning = true;
              $this->console->println("Warning : file '$file' could not be created.", CLIFGColor::YELLOW);
          }
        }
      }
      else {
        $error = true;
        $this->console->println("Error : Project folder '$project_path' could not be created.", CLIFGColor::RED);
      }

    }
    else {
      $error = true;
      $this->console->println("Error : Project folder '$project_path' already exists.", CLIFGColor::RED);
    }

    if($error) {
      $this->console->println("Project could not be created.", CLIFGColor::RED);
    }
    else if($warning) {
      $this->console->println("Project created with warnings.", CLIFGColor::YELLOW);
    }
    else {
      $this->console->println("Project was successfully created.", CLIFGColor::GREEN);
    }

  }

}
