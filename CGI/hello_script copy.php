
<?php
setcookie("name", "essadike", time() + 3600, '/');
print_r($_COOKIE);
?>


<!-- <form action="upload.php" method="post" enctype="multipart/form-data">
    Select File: <input type="file" name="fileToUpload">
    <input type="submit" value="Upload File" multiple>
</form>
<img src="../images/i.jpeg" alt=""> -->
 

<!-- <?php
// Check if cookies are set and retrieve their values
// if (isset($_COOKIE["name"])) {
//     $name = $_COOKIE["name"];
//     echo "Hello, $name!";
// } else {
//     // Set a cookie if it's not already set
//     $name = "mahdi";
//     setcookie("name", $name, time() + 3600, '/'); // Expires in 1 hour
//     echo "Welcome! A new cookie has been set.";
// }
?> -->

<!-- <?php
// set the cookies
// setcookie("cookie[three]", "cookiethree");
// setcookie("cookie[two]", "cookietwo");
// setcookie("cookie[one]", "cookieone");

// // after the page reloads, print them out
// if (isset($_COOKIE['cookie'])) {
//     foreach ($_COOKIE['cookie'] as $name => $value) {
//         $name = htmlspecialchars($name);
//         $value = htmlspecialchars($value);
//         echo "$name : $value <br />\n";
//     }
// } -->
// ?>

<!-- <?php

    // echo "<pre>";
    // //sleep(20000);
    // print_r($_FILES);
    // echo ($_FILES['fileToUpload']['tmp_name']);
    // $uploadedFile = $_FILES['fileToUpload']; // Assuming 'file' is the name of your file input field in a form.

    // if ($uploadedFile['error'] === UPLOAD_ERR_OK) { 
    // $tempFileName = $uploadedFile['tmp_name']; // The temporary file name on the server.

    // $destination = '/Users/edraidry/Desktop/webserver/' . $uploadedFile['name']; // Set your desired destination path and filename.

    // if (move_uploaded_file($tempFileName, $destination)) {
    //     echo 'File successfully uploaded and moved to ' . $destination;
    // } else {
    //     echo 'Failed to move the uploaded file.';
    // }
    // } else {
    //     echo 'File upload error: ' . $uploadedFile['error'];
    // }


    ?> -->
    #!/Users/edraidry/Desktop/webserver/CGI/php-cgi