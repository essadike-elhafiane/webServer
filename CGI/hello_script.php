 echo test


<?php 
    print_r($_FILES);
?>

 <form method="POST" enctype="multipart/form-data">
    <input type="file" name="fileToUpload" id="fileToUpload" multiple>
    <input type="submit" value="Upload" name="submit">
</form>
<img src="images/facebook.jpeg" alt="">