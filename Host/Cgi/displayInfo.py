#!/usr/bin/env python3
import cgi
import cgitb; cgitb.enable()  # Enable for debugging

def main():
    while(1):
        print()
    print("HTTP/1.1 200 OK\r")
    print("Content-Length: 50000\r")
    print("Content-Type: text/html\r")
    print("\r")

    print("<html><head><title>CGI Query String Values</title></head><body>")
    print("<h1>Query String Values</h1>")
    
    # Create instance of FieldStorage
    form = cgi.FieldStorage()

    # Check if the form has any fields
    if form.keys():
        print("<ul>")
        for key in form.keys():
            value = form.getvalue(key)
            print(f"<li>{key}: {value}</li>")
        print("</ul>")
    else:
        print("<p>No query string values found.</p>")
    print("</body></html>")

if __name__ == "__main__":
    main()
