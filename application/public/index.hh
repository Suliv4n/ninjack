<?hh
require "../bootstrap.hh";

Ninjack\Core\Error::get_instance()->set_exception_handler();
Ninjack\Core\Application::get_instance()->run();
