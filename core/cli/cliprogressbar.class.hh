<?hh
namespace Ninjack\Core\Cli;
use  Ninjack\Core\Cli\Console as Console;

class CLIProgressBar{
		private float $total;
		private float $current;
		private Console $console;

		private string $open_bar = "[";
		private string $close_bar = "]";
		private string $end_bar = ">";
		private string $bar_piece = "=";

		private int $size = 20;

		private string $message;

		public function __construct(Console $console, float $total, string $message = ""){
			$this->total = $total;
			$this->current = 0.;
			$this->console = $console;
			$this->message = $message;
		}

		public function set_current(float $current) : void{
			$this->current = $current;
		}

		public function set_currint(int $current) : void{
			$this->current = floatval($current);
		}

		public function print(){
			$this->console->print("\n\r");
			$this->console->print("\x1b[A");

			$ratio = $this->current / $this->total;
			$complete = floor($ratio * $this->size);

			$bar = $this->open_bar;
			$bar .= str_repeat($this->bar_piece, $complete);

			if($complete < $this->size){
				$bar .= ">";
				$bar .= str_repeat(" ", $this->size - $complete);
			}
			else{
				$bar .= $this->bar_piece;
			}

			$bar .= $this->close_bar;


			$bar .= " ".($ratio*100) ."%";
			if($this->is_finished()){
				$bar .= " ".$this->message;
			}


			$this->console->print($bar);

			flush();

			if($this->is_finished()){
				$this->console->print("\n");
			}

		}

		public function is_finished() : bool{
			return $this->current >= $this->total;
		}

		public function set_size(int $size) : void{
			$this->size = $size;
		}
}
