<?hh
final class :htmlheader extends :x:element {
  use XHPHelpers;

  attribute string title = "No Title";
  attribute Vector<string> style = Vector{};

  protected function render(): XHPRoot {

    $header = (
      <head>
        <title>{$this->getAttribute("title")}</title>
      </head>
    );

    foreach ($this->:style as $css) {
      $link = <link rel="stylesheet" type="text/css" media="screen"/>;
      $link->setAttribute("href", $css);
      $header->appendChild($link);
    }


    return $header;
  }
}
