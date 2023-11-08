
<!-- <?php

    // echo "<pre>";
    // //sleep(20000);
    // print_r($_FILES);
    // echo ($_FILES['fileToUpload']['tmp_name']);
    // $uploadedFile = $_FILES['fileToUpload']; // Assuming 'file' is the name of your file input field in a form.

    // if ($uploadedFile['error'] === UPLOAD_ERR_OK) { 
    // $tempFileName = $uploadedFile['tmp_name']; // The temporary file name on the server.

    // $destination = '/Users/eelhafia/Desktop/webServer/' . $uploadedFile['name']; // Set your desired destination path and filename.

    // if (move_uploaded_file($tempFileName, $destination)) {
    //     echo 'File successfully uploaded and moved to ' . $destination;
    // } else {
    //     echo 'Failed to move the uploaded file.';
    // }
    // } else {
    //     echo 'File upload error: ' . $uploadedFile['error'];
    // }


?> -->

 

<!-- <form action="upload.php" method="post" enctype="multipart/form-data">
    Select File: <input type="file" name="fileToUpload">
    <input type="submit" value="Upload File" multiple>
</form>
<img src="../images/i.jpeg" alt=""> -->
 
#!/Users/eelhafia/Desktop/webServer/CGI/php-cgi
<?php
// sleep(100000);
// Check if cookies are set and retrieve their values
if (isset($_COOKIE["name"])) {
    $name = $_COOKIE["name"];
    echo "Hello, $name!";
} else {
    // Set a cookie if it's not already set
    $name = "John";
    setcookie("name", $name, time() + 3600); // Expires in 1 hour
    echo "Welcome! A new cookie has been set.";
}
?>
