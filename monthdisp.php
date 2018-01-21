<?php
$servername = "localhost";
$username = "templog";
$password = "templog";
$dbname = "Temperature";
$year=$_GET['year'];
$month=$_GET['month'];
$data=array();
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname, 0, '/media/zds/klvn8r/private/mysql/socket');
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
$sql = "SELECT date_format(TimeStamp,'%Y/%m/%d') daystr, MIN(Temperature) mintmp, MAX(Temperature) maxtmp FROM Readings WHERE Source='Deck'and date_format(TimeStamp,'%Y')=".$year." and date_format(TimeStamp,'%m')=".$month." GROUP by date_format(TimeStamp,'%Y/%m/%d') order by TimeStamp asc";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    // output data of each row
    echo '<html>
    <head></head>
    <body><a href="./month.php">Month</a>
    <table border=1>
    <tr><th>Day</th><th>Min</th><th>Max</th></tr>';
    while($row = $result->fetch_assoc()) {
    echo "<tr align=center><td>";
    echo $row["daystr"];
    echo "</td><td>";
    echo $row["mintmp"];
    echo "</td><td>";
    echo $row["maxtmp"];
    echo "</td></tr>";
    }
    echo "</table>
    </body>
    </html>";
} else {
    echo "0 results";
}?>
