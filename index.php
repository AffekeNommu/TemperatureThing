<?php
$servername = "localhost";
$username = "temperature log user";
$password = "temperature log password";
$dbname = "Temperature";
$data=array();
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname, 0, '/path/to/your/database/mysql/socket');
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT TimeStamp, Temperature FROM Readings where date(TimeStamp)= date(addtime(now(),'11:0:0')) and Source='Deck' order by TimeStamp desc";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    // output data of each row
    echo '<a href="month.php">Month</a><table border=1><tr><th>Time</th><th>Temperature</th></tr>';
    while($row = $result->fetch_assoc()) {
    echo "<tr align=center><td>";
    echo $row['TimeStamp'];
    echo "</td><td>";
    echo $row['Temperature'];
    echo "</td></tr>";
    }
    echo "</table>
    </body>
    </html>";
} else {
    echo "0 results";
}
$conn->close();
?>
