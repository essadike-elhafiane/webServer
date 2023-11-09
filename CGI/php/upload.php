<?php
echo "<pre>";

print_r($_FILES);

if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_FILES['fileToUpload'])) {

    $uploadedFile = $_FILES['fileToUpload'];

    if ($uploadedFile['error'] === UPLOAD_ERR_OK) {
        $tempFileName = $uploadedFile['tmp_name'];

        $destinationDirectory = '/Users/edraidry/Desktop/webServer/';
        $destination = $destinationDirectory . $uploadedFile['name'];

        // Check if the destination directory exists, if not, create it.
        if (!is_dir($destinationDirectory) && !mkdir($destinationDirectory, 0755, true)) {
            echo 'Failed to create destination directory.';
        } else {
            if (move_uploaded_file($tempFileName, $destination)) {
                echo 'File successfully uploaded and moved to ' . $destination;
            } else {
                echo 'Failed to move the uploaded file.';
            }
        }
    } else {
        echo 'File upload error: ' . $uploadedFile['error'];
    }
}
?>

<form action="upload.php" method="post" enctype="multipart/form-data">
    Select File: <input type="file" name="fileToUpload">
    <input type="submit" value="Upload File" multiple>
</form>
