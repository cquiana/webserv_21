#!/usr/bin/php
<?php

echo "Status: 200 OK\r\n";
echo "Content-Type: text/html\r\n\r\n";

?>






<html>
<body>
<?php

parse_str($_ENV["QUERY_STRING"], $out_res);
echo "<h2>emerge system... emerge world... HELLO " . $out_res['name'] . " !!!</h2></br>";

// echo "<h2>Name: ". $out_res['name']. "</h2></br>";
// echo "<h2>Age: ", $out_res['age'], "</h2></br>";

// $name = "не определено";
// if(isset($_POST["name"])){
//
//     $name = $_POST["name"];
// }
// echo "<br> Привет, $name ! <br>";


// echo 'Привет ' . htmlspecialchars($_POST["name"]) . '!';

?>
</html>
</body>