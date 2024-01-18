#!/usr/bin/env python3

import cgi
import os

# Set the path to store uploaded files
upload_dir = "/path/to/upload/directory"

# Create the FieldStorage instance
form = cgi.FieldStorage()

# Get the values from the form
first_name = form.getvalue('first_name')
last_name = form.getvalue('last_name')

# Get the uploaded file details
file_item = form['file']

# Check if the file was uploaded
if file_item.filename:
    # Create a unique filename
    filename = os.path.join(upload_dir, os.path.basename(file_item.filename))

    # Save the file
    with open(filename, 'wb') as file:
        file.write(file_item.file.read())

    message = 'File {} successfully uploaded.'.format(filename)
else:
    message = 'No file uploaded.'

# Print the HTML response
print("Content-type: text/html\n")
print("<html><head><title>CGI Python Form</title></head><body>")
print("<h2>Form Data</h2>")
print("<p>First Name: {}</p>".format(first_name))
print("<p>Last Name: {}</p>".format(last_name))
print("<p>{}</p>".format(message))
print("</body></html>")
