#!/usr/bin/env python3
"""
decodeResource: Decodes Clop ransomware obfuscated resource for sample with hash
2f29950640d024779134334cad79e2013871afa08c7be94356694db12ee437e2 (SHA256)

MIT License

Copyright (c) 2019 Dan O'Day (d@4n68r.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import os
import sys

__author__ = "Dan O'Day"
__copyright__ = "Copyright 2019, Dan O'Day"
__credits__ = "Andrea Sancho"
__license__ = "MIT"
__version__ = "0.9"
__maintainer__ = "Dan O'Day"
__email__ = "d@4n68r.com"
__status__ = "Development"


clop_magic_string = "Clopfdwsjkjr23LKhuifdhwui73826ygGKUJFHGdwsieflkdsj324765tZPKQWLjwNVBFHewiuhryui32JKG"
magic_bytes = bytearray()
magic_bytes.extend(map(ord, clop_magic_string))
decoded_data = ""

# check for required args
if len(sys.argv) < 3:
    print("Missing file name parameters. Usage:\n"
          "    decodeResource.exe encryptedResourceFilePath.bin outputFilename.txt")
    sys.exit(1)

# place args in vars
resource_filename = sys.argv[1]
output_filename = sys.argv[2]

# validate resource file exists
if not os.path.isfile(resource_filename):
    print("Invalid file path: {}".format(resource_filename))
    sys.exit(1)

print("Beginning to decode file")

# read resource into buffer
with open(resource_filename, "rb") as resource:
    resource.seek(0, 2)  # seek EOF
    num_bytes = resource.tell()  # get file size in bytes

    # iterate over file byte by byte
    for i in range(num_bytes):
        resource.seek(i)
        b = resource.read(1)  # read a single byte
        c = int.from_bytes(b, sys.byteorder) ^ magic_bytes[i % 66]  # decode byte

        # append decoded byte (c) to buffer with exception-handling for unicode decode errors
        # todo -- just write bytes and forget about decoding to text
        try:
            decoded_data += c.to_bytes(1, sys.byteorder).decode("utf8")
        except UnicodeDecodeError:
            decoded_data += c.to_bytes(1, sys.byteorder).decode("latin1")

with open(output_filename, 'w') as out:
    out.write(decoded_data)

print("Finished! Output written to {}".format(output_filename))

