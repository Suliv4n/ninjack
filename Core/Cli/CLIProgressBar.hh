<?hh
namespace Ninjack\Core\Cli;
use  Ninjack\Core\Cli\Console as Console;

/**
 * Console Progress Bar generator.
 *
 * @author Sulivan
 */
class CLIProgressBar{

		/**
		 * The maximum value of the progress bar.
		 */
		private float $total;

		/**
		 * The current value of the progress bar.
		 */
		private float $current;

		/**
		 * Console where the progress bar will be printed.
		 */
		private Console $console;

		/**
		 * String that represents the opening bar.
		 */
		private string $open_bar = "[";

		/**
		 * String that represents the closing bar.
		 */
		private string $close_bar = "]";

		/**
		 * String that represents the end of the bar.
		 */
		private string $end_bar = ">";

		/**
		 * STring that represents the "done work".
		 */
		private string $bar_piece = "=";

		/**
		 * Size of the the progress bar.
		 */
		private int $size = 20;

		/**
		 * The message that will be printed when the progress bar is complete.
		 */
		private string $message;

		/**
		 * The constructor.
		 *
		 * @param Console $console The console where the progress bar will be printed.
		 * @param float $total the total of the progress bar.
		 * @param string $message The message that will be printed when the progress bar is completed.
		 */
		public function __construct(Console $console, float $total, string $message = ""){
			$this->total = $total;
			$this->current = 0.;
			$this->console = $console;
			$this->message = $message;
		}

		/**
		 * Set the current value of the progress bar.
		 *
		 * @param float $current The current value of the progress bar.
		 */
		public function set_current(float $current) : void{
			$this->current = $current;
		}

		/**
		 * Set the current value of the progress barwith an interger value.
		 *
		 * @param int $current The int current value of the progress bar.
		 */
		public function set_currint(int $current) : void{
			$this->current = floatval($current);
		}

		/**
		 * Print the progress bar in the console.
		 */
		public function print() : void{
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

		/**
		 * Returns true when the progress bar is finished (when the current value is
		 * greater or equal than the total value).
		 *
		 * @return bool true when the progress bar is finished, else false.
		 */
		public function is_finished() : bool{
			return $this->current >= $this->total;
		}

		/**
		 * Set the size of the progress bar.
		 *
		 * @param int $size The size of the progress bar.
		 */
		public function set_size(int $size) : void{
			$this->size = $size;
		}
}
