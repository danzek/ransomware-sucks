/*
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
*/

#include <iostream>
#include <string>
#include <Windows.h>

int main(int argc, char* argv[])
{
	std::cout << "Beginning to decode file" << std::endl;

	if (argc < 3)
	{
		std::cout << "Missing file name parameters. Usage:\n    decodeResource.exe encryptedResourceFilePath.bin outputFilename.txt" << std::endl;
		return 1;
	}

	std::string fileName(argv[1]);
	std::string outputFileName(argv[2]);

	std::cout << "Opening " << fileName << std::endl;

	// decode resource
	CHAR clopDecodeResourceSIXSIX1_MagicString[] = "Clopfdwsjkjr23LKhuifdhwui73826ygGKUJFHGdwsieflkdsj324765tZPKQWLjwNVBFHewiuhryui32JKG";
	HANDLE hResource = CreateFileA(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	std::cout << "created file handle" << std::endl;
	if (hResource == INVALID_HANDLE_VALUE)
	{
		std::cout << "Unable to open " << fileName << std::endl;
		return 1;
	}

	LARGE_INTEGER sz = { 0 };
	if (GetFileSizeEx(hResource, &sz) == 0)
	{
		std::cout << "Failed to get size of " << fileName << std::endl;
		CloseHandle(hResource);
		return 1;
	}
	std::cout << "File size is " << sz.QuadPart << std::endl;
	
	HGLOBAL hResourceMemory = GlobalAlloc(GMEM_ZEROINIT, sz.QuadPart);
	DWORD bytesRead = 0;
	if (ReadFile(hResource, hResourceMemory, sz.QuadPart, &bytesRead, 0) == 0)
	{
		std::cout << "Unable to read contents of " << fileName << std::endl;
		CloseHandle(hResource);
		GlobalFree(hResourceMemory);
		return 1;
	}
	std::cout << "Read " << bytesRead << " from " << fileName << " into memory." << std::endl;

	/* Decompiled malware code (annotated), where byte_4141C0 is magic string for resources:

	for ( i = 0; i < cbResourceSIXSIX1; ++i )
	    *((_BYTE *)hDecryptedResourceMemory + i) ^= byte_4141C0[i % 0x42];

		GetCurrentDirectoryA(260u, &Buffer);
		wsprintfA(&FileName, "%s\\clearsystems-10-1.bat", &Buffer);
		NumberOfBytesWritten = 0;
		hDecryptedFile = CreateFileA(&FileName, 0x40000000u, 2u, 0, 4u, 0x80u, 0);
		if ( hDecryptedFile != (HANDLE)-1 )
		{
			WriteFile(hDecryptedFile, hDecryptedResourceMemory, cbResourceSIXSIX1, &NumberOfBytesWritten, 0);
			CloseHandle(hDecryptedFile);
		}
		GlobalFree(hDecryptedResourceMemory);
		return ShellExecuteA(0, "open", &FileName, 0, 0, 0);
	*/
	for (int i = 0; i < sz.QuadPart; ++i)
	{
		*((BYTE *)hResourceMemory + i) ^= clopDecodeResourceSIXSIX1_MagicString[i % 66];
	}

	HANDLE hDecryptedResourceFile = CreateFileA(
		outputFileName.c_str(), 
		GENERIC_ALL,
		FILE_SHARE_WRITE,
		0,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		0
	);
	if (hDecryptedResourceFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "Unable to create file for decrypted contents!" << std::endl;
		CloseHandle(hResource);
		GlobalFree(hResourceMemory);
		return 1;
	}

	// write file to disk
	DWORD bytesWritten = 0;
	WriteFile(hDecryptedResourceFile, hResourceMemory, sz.QuadPart, &bytesWritten, 0);
	std::cout << "Wrote " << bytesWritten << " to " << fileName << " file." << std::endl;

	// cleanup
	CloseHandle(hResource);
	GlobalFree(hResourceMemory);

	std::cout << "Resource should now be decoded" << std::endl;
}

