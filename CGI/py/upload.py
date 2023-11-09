#!/usr/bin/env python3

import cgi
import os

print('Content-Type: text/html')  # Set the content type to HTML
print()  # Print an empty line to indicate the end of the headers

print('''
<form action="upload.py" method="post" enctype="multipart/form-data">
    Select File: <input type="file" name="fileToUpload">
    <input type="submit" value="Upload File" multiple>
</form>
''')

form = cgi.FieldStorage()
print(form)
if "fileToUpload" in form:
    file_item = form["fileToUpload"]

    if file_item.file:
        # Specify the directory where you want to save the files
        upload_dir = "/Users/edraidry/Desktop/webServer/"

        # Construct the absolute path to the destination file
        destination = os.path.join(upload_dir, os.path.basename(file_item.filename))

        with open(destination, "wb") as f:
            # Write the file content to the destination
            f.write(file_item.file.read())

        print(f"File successfully uploaded and saved to {destination}")
    else:
        print("Error: No file uploaded.")
else:
    print("Error: Form submission did not contain a file.")

