
<?php

include('connection.php');

if (!$con) {
die("Connection failed: " . mysqli_connect_error());}
 
$result="SELECT * FROM records";

$recordss=mysqli_query($con, $result);
?>
<html>
<head>
</head>
<body>
<table width="600" border="1" cellpadding="1" cellspacing="1">
<tr>
<th>PH LEVEL</th>
<th>WATER LEVEL</th>
<th>WATER TEMPERATURE</th>
<th>SALINITY</th>
<th>TIME</th>
</tr>

<?php
while($row = mysqli_fetch_array($recordss)) {

   echo "<tr>";
   echo "<td>".$row['Ph_level']."</td>"; 
   echo "<td>".$row['Water_level']."</td>"; 
   echo "<td>".$row['Water_temp']."</td>"; 
   echo "<td>".$row['Salinity']."</td>"; 
   echo "<td>".$row['Time']."</td>"; 
   echo "</tr>";
}
?>

</table>
</body>
</html>







 