#!/usr/bin/env python3

import cgi
import http.cookies

# Create a cookie
cookie = http.cookies.SimpleCookie()
cookie["my_cookie"] = "Hello, Cookie!"

# Print the header with the Set-Cookie field
print("Set-Cookie: my_cookie=Hello, Cookie;")
print("Content-type: text/html")
print()

# Print the body of the response
print("<html><body>")
print("<h2>Cookie Set Successfully</h2>")

# Access form data using cgi.FieldStorage()
form = cgi.FieldStorage()

# Check if there's a form field named 'username'
if "username" in form:
    username = form["username"].value
    print(f"<p>Welcome, {username}!</p>")

print("</body></html>")
