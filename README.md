# FLRECL-File-Analyzer
A lightweight C++ tool designed to analyze text or binary files and detect lines that exceed a specified FLRECL (record length) limit.
This program reads files at a low level (byte-by-byte) to preserve their original structure and identifies problematic lines that may cause issues in file transfer systems like Axway CFT. It generates a detailed log including:

Maximum line length in the file
Lines exceeding the FLRECL limit
Hexadecimal representation of bytes
Printable ASCII view
Highlighting of non-printable characters

Useful for debugging file formatting issues, encoding problems, and transfer errors in production environments.
<img width="1485" height="223" alt="image" src="https://github.com/user-attachments/assets/16be3bb1-96ec-4511-9e85-68b0d52b26f2" />
