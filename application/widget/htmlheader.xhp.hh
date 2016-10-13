<?hh
final class :htmlheader extends :x:element {
  use XHPHelpers;

  attribute string title = "No Title";
  attribute Vector<string> resources = Vector{};
  attribute string charset = "UTF-8";

  protected function render(): XHPRoot {

    $header = (
      <head>
        <title>{$this->getAttribute("title")}</title>
        <meta charset={$this->:charset} />
      </head>
    );

    foreach ($this->:resources as $css) {
      $link = <link rel="stylesheet" type="text/css" media="screen"/>;
      $link->setAttribute("href", $css);
      $header->appendChild($link);
    }


    return $header;
  }
}
