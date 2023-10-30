
# import cgi
# import cgitb

# cgitb.enable()  # Enable detailed error messages

# print("Content-Type: text/html")  # Set the content type
# print()  # Print a blank line to separate headers from the body
# print("<html>")
# print("<head><title>Python CGI Test</title></head>")
# print("<body>")
# print("<h1>Python CGI Test</h1>")
# print("<p>This is a test of Python CGI.</p>")

# # Print server environment variables
# print("<h2>Server Environment Variables</h2>")
# print("<ul>")
# for key, value in sorted(os.environ.items()):
#     print(f"<li><strong>{key}:</strong> {value}</li>")
# print("</ul>")

# # Print form data
# form = cgi.FieldStorage()
# if form:
#     print("<h2>Form Data</h2>")
#     print("<ul>")
#     for field in form.keys():
#         value = form[field].value
#         print(f"<li><strong>{field}:</strong> {value}</li>")
#     print("</ul>")

# print("</body>")
# print("</html>")



#############################################################################################
#!/usr/bin/env python
# import cgi

# # Create an instance of the FieldStorage class to parse the form data.
# form = cgi.FieldStorage()

# # Get the user's name from the form.
# user_name = form.getvalue("name")

# # Get the selected programming languages as a list.
# selected_languages = form.getlist("languages")

# # Process the data.
# print("Content-Type: text/html")  # Set the content type for the response
# print()  # Print an empty line to indicate the start of the response body

# print("<html>")
# print("<head>")
# print("<title>Form Processing Results</title>")
# print("</head>")
# print("<body>")
# print("<h1>Form Processing Results</h1>")
# print("<p>Name: " + user_name + "</p>")
# print("<p>Favorite Programming Languages:</p>")
# print("<ul>")
# for language in selected_languages:
#     print("<li>" + language + "</li>")
# print("</ul>")
# print("</body>")
# print("</html>")

######################################################

#!/usr/bin/env python


# import cgi
# import os

# # Create an instance of the FieldStorage class to parse the form data.
# form = cgi.FieldStorage()

# # Get the uploaded file information.
# uploaded_file = form['fileToUpload']

# # Check if the file was uploaded successfully and there are no errors.
# if uploaded_file.filename:
#     # Set your desired destination directory.
#     destination_directory = '/Users/edraidry/Desktop/webserver/'

#     # Create a unique filename to avoid overwriting existing files.
#     destination_filename = os.path.join(destination_directory, uploaded_file.filename)

#     # Open a new file in binary write mode to save the uploaded file.
#     with open(destination_filename, 'wb') as file:
#         file.write(uploaded_file.file.read())

    
#     print("Content-Type: text/html")
#     print()
#     print("File successfully uploaded and moved to: " + destination_filename)
# else:
#     print("Content-Type: text/html")
#     print()
#     print("File upload error")




import cgi
import os

# Create an instance of the FieldStorage class to parse the form data.
form = cgi.FieldStorage()

# Get the uploaded file information.
uploaded_file = form['fileToUpload']

# Check if the file was uploaded successfully and there are no errors.
if uploaded_file.filename:
    # Set your desired destination directory.
    destination_directory = '/Users/edraidry/Desktop/webserver/'

    # Create a unique filename to avoid overwriting existing files.
    destination_filename = os.path.join(destination_directory, uploaded_file.filename)

    # Open a new file in binary write mode to save the uploaded file.
    with open(destination_filename, 'wb') as file:
        file.write(uploaded_file.file.read())

    # Get the dynamic value you want to set for the cookie
    dynamic_cookie_value = form.getvalue('dynamic_value')

    # Set a cookie with the dynamic value in the response
    print(f"Set-Cookie: mycookie={dynamic_cookie_value}")

    print("Content-Type: text/html")
    print()
    print("File successfully uploaded and moved to: " + destination_filename)
else:
    # Print the content-type header
    print("Content-Type: text/html")
    print()
    print("File upload error")


#console.log(document.cookie);