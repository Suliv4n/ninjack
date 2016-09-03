<?hh
$routes = Map{
  "/?" => "Test:home",
  "/welcome/(.*)?" => "Tet:welcome/$1",
};
