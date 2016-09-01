<?hh
$routes = Map{
  "/?" => "Test:home",
  "/welcome-(.*)?" => "Test:welcome/$1",
};
