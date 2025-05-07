#!/bin/python3

import cgi
import html
import os

import cgitb
cgitb.enable()

print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
print()

form = cgi.FieldStorage()
name = html.escape(form.getfirst("name", "Unknown"))
email = html.escape(form.getfirst("email", "Unknown"))

print(f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Form Submission Received</title>
</head>
<body>
    <h1>Data Received</h1>
    <p><strong>Name:</strong> {name}</p>
    <p><strong>Email:</strong> {email}</p><p>""")

for k, v in os.environ.items():
    print(f"{k}={v}")

print("""
</body>
</html>
""")
