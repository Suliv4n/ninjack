<?hh
$resources = Vector{
  $this->asset('css/main.css'),
};

echo
<htmlpage resources={$resources} title="welcome">
  {$form->get_element()}
</htmlpage>;
