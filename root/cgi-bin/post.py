#!/usr/bin/env python3

import os
import sys

def main():
    
    content_length = os.environ.get("CONTENT_LENGTH")
    
    
    if content_length:
        try:
            content_length = int(content_length)
            body = sys.stdin.read(content_length)
        except ValueError:
            body = "Invalid CONTENT_LENGTH"
    else:
        body = "No body received"

  
    print("Content-Type: text/plain")
    print("") 
    print("Received POST data:")
    print(body)

if __name__ == "__main__":
    main()