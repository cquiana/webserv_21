#!/usr/bin/php
<?php

echo "Status: 200 OK\r\n";
echo "Content-Type: text/html\r\n\r\n";

?>






<html>
<body>
<?php


parse_str($_ENV["QUERY_STRING"], $out_res);

echo "<h2>Name: ", $out_res['name'], "</h2></br>";
// echo "<h2>Age: ", $out_res['age'], "</h2></br>";


?>
</html>
</body>name=123%D1%8B%D0%BF%D0%B0%D1%8B%D0%BF%D0%B0