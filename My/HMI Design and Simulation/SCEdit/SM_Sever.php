<!DOCTYPE html>
<html>
<body>

<h1>My first PHP page</h1>

<?php

class StateArray {
    function StateArray() {
        $this->model = "VW";
    }
}

$servername = "localhost";
$username = "username";
$password = "password";

// Create connection
$conn = new mysqli($servername, $username, $password);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
echo "Connected successfully";
?>

</body>
</html>