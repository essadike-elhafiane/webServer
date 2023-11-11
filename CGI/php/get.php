<?php


    $method = $_SERVER['REQUEST_METHOD'];

    
    // echo "The request method is: $method";

?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Check Method</title>
</head>
<body>

    <form action="Get.php" method="get">
        <label for="name">Enter your name:</label>
        <input type="text" id="name" name="name" required>
        <br>
        <button type="submit">Submit</button>
        <p> <?php 
            if (isset($_SERVER["HTTP_QUERY_STRING"])) {
            $colors = $_SERVER["HTTP_QUERY_STRING"];
            list($key, $value) = explode("=", $colors);
            echo $value;
            echo "<hr>";
            // echo "set new session";
            } 
        ?> </p>
    </form>

</body>
</html>