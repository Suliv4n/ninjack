<?hh
$routes = HH\Map{
  "/?" => "Test:home",
  "/welcome-(.*)?" => "Test:welcome/$1",
};
