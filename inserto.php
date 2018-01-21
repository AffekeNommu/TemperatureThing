<?php
$servername = "localhost";
$username = "temperature log user";
$password = "temperature log password";
$dbname = "Temperature";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname, 0, '/path/to/your/database/mysql/socket');
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
$Source=$_GET["Source"]; 
$Temperature=$_GET["Temperature"];
$TimeStamp=$_GET["TimeStamp"];
$sql = "INSERT INTO Readings (Source, Temperature, TimeStamp)
VALUES ('$Source','$Temperature', '$TimeStamp')";

if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
?>
