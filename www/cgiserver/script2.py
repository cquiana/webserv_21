#!/usr/local/bin/python3
import os
a = os.environ["HTTP_body"]
# a = os.environ["QUERY_STRING"]
print("""Status: 200
Content-Type: text/plain

emerge system... emerge world... HELLO """ + a.split("=")[1] + """ !!!""")
