#!/usr/bin/python3

# Import the CGI module for handling CGI data
import cgi

# Create an instance of the FieldStorage class to handle the form data
form = cgi.FieldStorage()

# Get the user's name from the form data
user_name = form.getvalue('name')

# Send the Content-Type header
print("Content-Type: text/html\n")

# Start the HTML content
print("<html>")
print("<head><title>Python CGI Form</title></head>")
print("<body>")
print("<h1>Python CGI Form Example</h1>")

# Check if a name was submitted
if user_name:
    print("<p>Hello, {}!</p>".format(user_name))
else:
    print("<p>Please enter your name in the form below.</p>")

# Create an HTML form
print("<form method='post' action='your_script.py'>")
print("<label for='name'>Name:</label>")
print("<input type='text' id='name' name='name'>")
print("<input type='submit' value='Submit'>")
print("</form>")

# Close the HTML content
print("</body>")
print("</html>")
