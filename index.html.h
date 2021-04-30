const char *indexHtml = R"====( 
<!DOCTYPE html>
<html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <script src="https://code.jquery.com/jquery-3.6.0.min.js" integrity="sha256-/xUj+3OJU5yExlq6GSYGSHk7tPXikynS7ogEvDej/m4=" crossorigin="anonymous"></script>
        <title>Remote Polarization Switch</title>
        <style>
        td {
          height:2em;
          font-size:2em;
        }
        a,a:hover {
          color:black;
          text-decoration:none;
        }
        </style>
    </head>
    <body>
        <center>
        <h1>Remote Polarization Switch</h1>
        <table width='600' style='text-align:center;'>
        <tr style="background-color:yellow;"><td>2m</td><td>70cm</td></tr>
        <tr><td id="2mHORI"><a onclick="sendrelay('2m','HORI');" href="#">HORI</a></td><td id="70cmHORI"><a onclick="sendrelay('70cm','HORI');" href="#">HORI</a></td></tr>
        <tr><td id="2mVERT"><a onclick="sendrelay('2m','VERT');" href="#">VERT</a></td><td id="70cmVERT"><a onclick="sendrelay('70cm','VERT');" href="#">VERT</a></td></tr>
        <tr><td id="2mCIRC"><a onclick="sendrelay('2m','CIRC');" href="#">CIRC</a></td><td id="70cmCIRC"><a onclick="sendrelay('70cm','CIRC');" href="#">CIRC</a></td></tr>
        </table><p><a href="/reset">Reset Wi-Fi Settings</a></p>
        </center>
        <script type="text/javascript">
     $(document).ready(function () {
          setInterval(function(){
              getstate();
        }, 3000);
     });
    getstate();
    var old2m='';
    var old70cm='';
    function sendrelay(band,polar) {
      $.getJSON('/json?band='+band+'&polar='+polar, function(data) {
      });
      getstate();
    }
    function getstate() {
      $.getJSON('/json', function(data) {
        var v2m=data.v2m;
        var v70cm=data.v70cm;
        if (v2m!=old2m) {
          old2m=v2m;
          $("#2mHORI").css("background", "grey");
          $("#2mVERT").css("background", "grey");
          $("#2mCIRC").css("background", "grey");
          $("#2m"+v2m).css("background", "green");
        }
        if (v70cm!=old70cm) {
          old70cm=v70cm;
          $("#70cmHORI").css("background-color", "grey");
          $("#70cmVERT").css("background-color", "grey");
          $("#70cmCIRC").css("background-color", "grey");
          $("#70cm"+v70cm).css("background-color", "green");
        }
      });
        }
    </script>
    </body>
</html>
<!-- end .html -->
)===="; 
