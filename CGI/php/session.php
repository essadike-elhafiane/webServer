<?php
    session_start();
    if (isset($_SERVER["HTTP_QUERY_STRING"])) {
        $colors = $_SERVER["HTTP_QUERY_STRING"];
        print_r($colors);
        list($key, $value) = explode("=", $colors);
        $_SESSION['favcolor'] = $value;
        echo "set new session";
    }

    $favoriteColor = isset($_SESSION['favcolor']) ? $_SESSION['favcolor'] : 'not set';
    echo "<h1 style=\"color:$favoriteColor\"> hhhhhhh </h1>";
    print_r($favoriteColor);
?>