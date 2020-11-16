
<?php

include('connection.php');
 
$sql_insert="INSERT INTO records (Ph_level, Water_level, Water_temp, Salinity) VALUES('".$_GET["Ph_level"]."','".$_GET["Water_level"]."','".$_GET["Water_temp"]."','".$_GET["Salinity"]."')";

if(mysqli_query($con ,$sql_insert))
{
echo "Done";
mysqli_close($con);
}
else
{
echo "error is ".mysqli_error($con);
}
 
?>