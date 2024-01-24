# !/usr/bin/env python

import cgi

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
first_name = form.getvalue('first_name')
last_name = form.getvalue('last_name')

# Print the HTTP header with content type
print("Content-type: text/html\n")

# Generate HTML response
print("<html>")
print("<head>")
print("<title>CGI Python Script</title>")
print("</head>")
print("<body>")
print("<h1>CGI Python Script Example</h1>")

# Display the submitted data
if first_name and last_name:
    print("<p>Thank you for submitting the form, {} {}!</p>".format(first_name, last_name))
else:
    print("<p>Please submit the form with your first and last name.</p>")

# Display the form
print("<form method='post' action='myscript.py'>")
print("<label for='first_name'>First Name:</label>")
print("<input type='text' name='first_name'>")
print("<br>")
print("<label for='last_name'>Last Name:</label>")
print("<input type='text' name='last_name'>")
print("<br>")
print("<input type='submit' value='Submit'>")
print("</form>")

print("</body>")
print("</html>")
