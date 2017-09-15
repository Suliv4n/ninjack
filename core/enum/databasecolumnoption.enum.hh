<?hh
namespace Ninjack\Core\Enum;

/**
 * SQL column options (only mariadb for the moement)
 * (https://mariadb.com/kb/en/library/create-table/)
 */
enum DataBaseColumnOption: string {

  SQL_NULL = "NULL";
  SQL_NOT_NULL = "NOT NULL";
  SQl_AUTO_INCREMENT = "AUTO INCREMENT";
  SQL_UNIQUE = "UNIQUE";
  SQL_PRIMARY_KEY = "PRIMARY KEY";

}
