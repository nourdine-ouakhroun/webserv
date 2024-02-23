#!/usr/bin/python3.9
import cgi
import os
from http import cookies
import cgitb; cgitb.enable()  # Enable CGI traceback for debugging

def print_header(title):
    # print("Content-Type: text/html")
    # print()  # End of headers
    print(f"<html><head><title>{title}</title></head><body>")

def print_footer():
    print("</body></html>")

# def save_uploaded_file(form_field, upload_dir):
#     form = cgi.FieldStorage()

#     if form_field not in form:
#         return False, "No file was uploaded1"
#     fileitem = form[form_field]
#     if not fileitem.file:
#         return False, "No file was uploaded2"
    
#     file_path = os.path.join(upload_dir, os.path.basename(fileitem.filename))
#     with open(file_path, 'wb') as f:
#         while True:
#             chunk = fileitem.file.read(100000)
#             if not chunk:
#                 break
#             f.write(chunk)
#     return True, f"The file was uploaded successfully to {file_path}"


def save_uploaded_file(form_field, upload_dir):
    form = cgi.FieldStorage()

    if form_field not in form:
        return False, "No file was uploaded1"
    fileitem = form[form_field]
    if not fileitem.file:
        return False, "No file was uploaded2"
    
    file_path = os.path.join(upload_dir, os.path.basename(fileitem.filename))
    with open(file_path, 'wb') as f:  # Open in binary mode
        while True:
            chunk = fileitem.file.read(100000)
            if not chunk:
                break
            if not isinstance(chunk, bytes):
                return False, "Chunk is not bytes"  # Add this line for debugging
            f.write(chunk)
    return True, f"The file was uploaded successfully to {file_path}"

def main():

    print_header("File Upload")
    upload_dir = './'  # Change this to the correct path
    success, message = save_uploaded_file('filename', upload_dir)
    print(f"<p>{message}</p>")
    print_footer()

if __name__ == "__main__":
    main()