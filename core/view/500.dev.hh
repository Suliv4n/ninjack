<!DOCTYPE html>
<html>
<head>
    <title>Oops an error occured !</title>
    <style>
      body{
        background-color: #e6f7ff;
        font-family: monospace;
        margin:0;
        padding:0;
      }
      .ninjack-error{
        margin: auto;
      }
      .ninjack-error-message{
        width : 90vw;
        padding : 15px;
        background-color: #ffffe6;
        margin: auto;
        border: solid 1px #cc9900;
        border-radius: 5px;
      }

      .ninjack-error-title{
        width : 90vw;
        padding : 15px 15px 30px 15px;
        font-size: 30px;
        font-weight: bold;
        margin: auto;
      }


      .ninjack-error-bloc{
        margin: auto;
        margin-top: 30px;
        width : 90vw;
        background-color: #ffffe6;
        margin: auto;
        border: solid 1px #cc9900;
        border-radius: 5px;
        margin-top : 30px;
        padding : 15px 15px 30px 15px;
      }

      .ninjack-error-bloc-title{
        font-size: 30px;
        font-weight: bold;
        margin-bottom:20px;
      }

    </style>
</head>
<body>
    <div class="ninjack-error">
      <div class="ninjack-error-title" >
        Oops an error occured !
      </div>
      <div class="ninjack-error-bloc ninjack-error-message">
        <?php
          echo $error->getMessage()."<br/>";
          echo "File : ".$error->getFile()."<br/>";
          echo "Line : ".$error->getLine()."<br/>";
        ?>
      </div>
      <div class="ninjack-error-bloc ninjack-error-stack-trace">
        <div class="ninjack-error-bloc-title">
          Trace :
        </div>
        <?php
          foreach ($error->getTrace() as $_ => $trace) {
            ?>
            <div class="ninjack-error-stack-trace-trace" >
            <?php
            echo "File : " . $trace["file"]."<br/>";
            echo "Line : " . $trace["line"]."<br/>";
            if(!empty($trace["class"])){
              echo "Class : " . $trace["class"]."<br/>";
            }
            echo "Funtion : " . $trace["function"]."<br/>";
          }
            ?>
            </div>
      </div>
      <div class="ninjack-error-bloc">
        <div class="ninjack-error-bloc-title">
          Route :
        </div>
        <div>
          <?php
            echo "Rule : \"".$rout_name."\" => \"".$route."\"<br/>";
            echo "Controller : ".$controller."<br/>";
            echo "Action : ".$action."<br/>";
          ?>
        </div>
      </div>
    </div>
</body>
</html>
