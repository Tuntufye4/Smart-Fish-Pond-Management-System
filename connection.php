<?php

$servername = "localhost";
$username = "root";
$password = "";
$db="pondrecords";
 
$con = mysqli_connect($servername, $username, $password);

$db= mysqli_select_db ($con, $db);

?>