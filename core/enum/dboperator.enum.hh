<?hh
namespace Ninjack\Core\Enum;

/**
 * Enum of data base where operator
 */
 enum DBOperator : string {
   EQUALS = "=";
   NOT_EQUALS = "<>";
   GREATER = ">";
   GREATER_OR_EQUALS = ">=";
   LOWER = "<";
   LOWER_OR_EQUALS = "<=";
   IS_NULL = "IS NULL";
   IS_NOT_NULL = "IS NOT NULL";
   IN = "IN";
   NOT_IN = "NOT IN";
   LIKE = "LIKE";
   NOT_LIKE = "NOT LIKE";
 }
