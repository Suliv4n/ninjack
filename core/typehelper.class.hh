<?hh
namespace Ninjack\Core;

/**
 * The type helper class.
 *
 * @author Sulivan
 */
class TypeHelper{

  /**
   * Convert a string nullable value type to match a ReflectionParameter type.
   * The parameter type must be one of these :
   * bool, int, float, double, string. Else the method will return the string value.
   *
   * @param \ReflectionParameter $parameter the reflection parameter the
   * value has to match type.
   * @param ?string $value The value to be casted.
   *
   * @param the Casted value type.
   */
  public static function bind_parameter_value_type(\ReflectionParameter $parameter, mixed $value) : mixed{

    if($value == null){
      return null;
    }

    $parameter_type = $parameter->getType();

    if($parameter_type != null){
        $target_type = preg_replace("~^\?~", "", $parameter_type->__toString());
        $target_type = preg_replace("~^HH\\\\~", "", $target_type);

        switch ($target_type) {
          case "boolean":
          case "bool":
            $value = boolval($value);
            break;
          case "int":
          case "integer":
            $value = intval($value);
            break;
          case "float":
            $value = floatval($value);
            break;
          case "double":
            $value = doubleval($value);
            break;
          case "string":
            $value = strval($value);
            break;
        }
      }

      return $value;
  }

  public static function bind_parameters_function(\ReflectionFunctionAbstract $method, $paramaters) : Vector<mixed>{
    foreach ($method->getParameters() as $i => $parameter) {
      $paramaters[$i] = self::bind_parameter_value_type($parameter, $paramaters[$i]);
    }

    return $paramaters;
  }

  public static function get_generic_type(\ReflectionType $type) : ?string{
    $string = $type->__toString();
    $matches = Vector{};
    preg_match('/<([^>]*)>$/', $string, $matches);
    return $matches[1];
  }

  public static function get_str_type(\ReflectionType $type) : ?string{
    $string = $type->__toString();
    $string = preg_replace('/(<[^>]*)>$/',"", $string);
    return $string;
  }

}
