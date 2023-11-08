<?php
    $Cook = $_SERVER["HTTP_COOKIE"];
    $values = explode("; ", $Cook);

    foreach ($values as $value) {
        list($key, $val) = explode("=", $value);
        setcookie($key, $val, time() + 3600, '/');
    }
    
    print_r($_COOKIE);
?>
