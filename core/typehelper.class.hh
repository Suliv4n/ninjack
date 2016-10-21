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

  /**
   * Cast all parameters given to match the type of a function parameters.
   *
   * @param \ReflectionFunctionAbstract the function that parameters types have to match.
   * @param Vector<string> $paramaters parameters to cast.
   */
  public static function bind_parameters_function(\ReflectionFunctionAbstract $method, Vector<string> $paramaters) : Vector<mixed>{
    $parsed_parameters = Vector{};
    foreach ($method->getParameters() as $i => $parameter) {
      $parsed_parameters[$i] = self::bind_parameter_value_type($parameter, $paramaters[$i]);
    }

    return $parsed_parameters;
  }

  /**
   * Returns the generic types of a type.
   *
   * @param \ReflectionType $type the type to parse.
   *
   * @return ?string the genric types of a class or null
   * the class has no generic class.
   */
  public static function get_generic_type(\ReflectionType $type) : ?string{
    $string = $type->__toString();
    $matches = Vector{};
    preg_match('/<([^>]*)>$/', $string, $matches);
    return $matches[1];
  }

  /**
   * Returns the class of a type without Generic types representation.
   *
   * @return string The class of a type without Generic types representation.
   */
  public static function get_str_type(\ReflectionType $type) : ?string{
    $string = $type->__toString();
    $string = preg_replace('/(<[^>]*)>$/',"", $string);
    return $string;
  }

}
