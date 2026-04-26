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

## Compile the program

```bash
g++ -std=c++17 analyze_flrecl.cpp -o analyze_flrecl
```

## Show help

```bash
./analyze_flrecl --help
```

## Show program information

```bash
./analyze_flrecl --about
```

## Analyze a file with the default FLRECL limit

By default, the FLRECL limit is `156` bytes.

```bash
./analyze_flrecl -file input.txt
```

This creates a log file automatically, for example:

```bash
FLRECL_log_20260426_083000.txt
```

## Analyze a file with a custom FLRECL limit

```bash
./analyze_flrecl -file input.txt -flrecl 200
```

## Specify the output log file

```bash
./analyze_flrecl -file input.txt -flrecl 156 -output result_log.txt
```

## Windows example

```powershell
.analyze_flrecl.exe -file C:\files\input.txt -flrecl 156 -output C:\files\flrecl_log.txt
```

## Linux example

```bash
./analyze_flrecl -file /home/user/files/input.txt -flrecl 156 -output /home/user/flrecl_log.txt
```

## Output

The generated log file contains:

- The analyzed file path
- The configured FLRECL limit
- The maximum FLRECL found in the file
- The line number of the longest record
- The number of lines exceeding the limit
- Full content of problematic lines
- Hexadecimal byte representation
- Printable ASCII view
- Highlighted non-ASCII or non-printable characters

### Example output

```txt
File analyzed: input.txt
FLRECL limit: 156 bytes
Maximum FLRECL in file: 184 bytes (line 12)
Total lines exceeding limit: 1

Line 12: FLRECL = 184 bytes
Full line: ...
Hex bytes: 41 42 43 C3 A9 ...
Printable ASCII: ABC...
Highlighted non-ASCII: ABC[0xC3][0xA9]
--------------------------------------------------------------------------------
```

## Command Options

| Option | Description |
|---|---|
| `-file` or `--file` | Input file to analyze |
| `-flrecl` or `--flrecl` | FLRECL limit in bytes (default: 156) |
| `-output` or `--output` | Output log file path |
| `-about` or `--about` | Display program information |
| `-h` or `--help` | Show help message |

