# Clop ransomware resource decoder

I wrote a decoder for the obfuscated resources embedded in Clop ransomware (e.g., "SIXSIX1"), specifically for the sample with SHA256 hash: 

    2f29950640d024779134334cad79e2013871afa08c7be94356694db12ee437e2

## Usage

### C++ CLI

:warning: *The C++ tool uses the Windows API and so will only compile on Windows (unless you get creative). It's more or less doing the exact same thing the malware does to decode the resources.* 

1. Extract the resource from the executable.
2. Run the tool:

       decodeResource.exe encryptedResourceFilePath.bin outputFilename.txt

### Python 3 CLI

I also wrote a **Python 3** version that should work on any platform, with the exact same usage &mdash; only Python:

       python3 decodeResource.py encryptedResourceFilePath.bin outputFilename.txt

## Learn more

Read [this blog post.](https://laserkittens.com/ransomware-sucks/)
