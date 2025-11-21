#!/usr/bin/env python3

import cgi
import cgitb
cgitb.enable()  # Enables error display for easier debugging

# Get the data from the query string
form = cgi.FieldStorage()

# Access the parameters passed in the URL
name = form.getvalue("name")
age = form.getvalue("age")

# Output the data in the response
# print("Content-Type: text/html\n")  # Required HTTP header
print("<html><body>")
print("<h1>Received Data</h1>")
print(f"<p>Name: {name}</p>")
print(f"<p>Age: {age}</p>")
print("<p><a href=\"/cgi-bin/index.html\">Back to CGI home</a></p>")
print("</body></html>")


