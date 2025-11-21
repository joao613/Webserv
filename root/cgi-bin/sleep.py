#!/usr/bin/python3

import time
import cgi

# Define the HTTP response header
# print("Content-Type: text/html")    # Response header
# print()                             # Blank line separating header from body

# Sleep time in seconds
sleep_time = 5  # Change this value as needed

# Wait for the specified time
time.sleep(sleep_time)

# Response body
print("<html>")
print("<head><title>CGI Sleep Example</title></head>")
print("<body>")
print(f"<h1>The script slept for {sleep_time} seconds.</h1>")
print("<p><a href=\"/cgi-bin/index.html\">Back to CGI home</a></p>")
print("</body>")
print("</html>")
