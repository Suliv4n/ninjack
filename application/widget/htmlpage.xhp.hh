<?hh
final class :htmlpage extends :x:element {
  use XHPHelpers;

  attribute string title = "";
  attribute Vector<string> resources = Vector{};
  attribute string charset = "UTF-8";

  protected function render(): XHPRoot {

    $head =
    <htmlheader
      title={$this->:title}
      resources={$this->:resources}
      charset={$this->:charset}
    />;

    $body = <body>{$this->getChildren()}</body>;

    $page = <x:doctype>
      <html>
        {$head}
        {$body}
      </html>
    </x:doctype>;

    return $page;
  }
}
