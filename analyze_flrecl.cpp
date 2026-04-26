#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <cctype>

using namespace std;

// Struct to hold info about problematic lines
struct LineInfo {
    size_t line_num;
    size_t flrecl;
    string full_line;
    string hex_bytes;
    string printable;
    string highlighted;
};

// Utility to format current timestamp
string current_timestamp() {
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm local_tm{};
#ifdef _WIN32
    localtime_s(&local_tm, &now_c);
#else
    localtime_r(&now_c, &local_tm);
#endif
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", &local_tm);
    return string(buffer);
}

// Read and analyze file
size_t analyze_file_flrecl(const string& file_path, size_t limit, const string& log_path) {
    ifstream infile(file_path, ios::binary);
    if (!infile) {
        cerr << "Error: Cannot open file: " << file_path << endl;
        return 0;
    }

    vector<LineInfo> problematic_lines;
    size_t max_len = 0;
    size_t max_line_num = 0;
    string line;
    size_t line_num = 0;

    while (true) {
        string buffer;
        char ch;
        bool eof = false;
        // Read one line manually to preserve binary structure
        while (infile.get(ch)) {
            if (ch == '\n' || ch == '\r') {
                char next = infile.peek();
                if (next == '\n' || next == '\r') infile.get();
                break;
            }
            buffer.push_back(ch);
        }
        if (infile.eof() && buffer.empty()) break;
        line_num++;

        size_t length = buffer.size();
        if (length > max_len) {
            max_len = length;
            max_line_num = line_num;
        }

        if (length > limit) {
            stringstream hex_ss, printable_ss, highlighted_ss;
            for (unsigned char b : buffer) {
                hex_ss << setw(2) << setfill('0') << uppercase << hex << (int)b << " ";
                printable_ss << (isprint(b) ? static_cast<char>(b) : '.');
                if (isprint(b))
                    highlighted_ss << static_cast<char>(b);
                else {
                    stringstream t;
                    t << "[0x" << setw(2) << setfill('0') << uppercase << hex << (int)b << "]";
                    highlighted_ss << t.str();
                }
            }
            LineInfo info {
                line_num,
                length,
                buffer,
                hex_ss.str(),
                printable_ss.str(),
                highlighted_ss.str()
            };
            problematic_lines.push_back(info);
        }

        if (infile.eof()) break;
    }

    // Write log
    ofstream log(log_path, ios::out | ios::trunc);
    if (!log) {
        cerr << "Error: Cannot write log file: " << log_path << endl;
        return max_len;
    }

    log << "File analyzed: " << file_path << "\n";
    log << "FLRECL limit: " << limit << " bytes\n";
    log << "Maximum FLRECL in file: " << max_len << " bytes (line " << max_line_num << ")\n";
    log << "Total lines exceeding limit: " << problematic_lines.size() << "\n\n";

    for (const auto& info : problematic_lines) {
        log << "Line " << info.line_num << ": FLRECL = " << info.flrecl << " bytes\n";
        log << "Full line: " << info.full_line << "\n";
        log << "Hex bytes: " << info.hex_bytes << "\n";
        log << "Printable ASCII: " << info.printable << "\n";
        log << "Highlighted non-ASCII: " << info.highlighted << "\n";
        log << string(80, '-') << "\n";
    }

    log.close();
    cout << "Analysis complete. Log written to: " << filesystem::absolute(log_path) << endl;
    return max_len;
}

// Simple CLI argument parser
// Simple CLI argument parser
int main(int argc, char* argv[]) {
    string file_path;
    string log_file;
    size_t flrecl_limit = 156;

    // Check for arguments
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if ((arg == "-file" || arg == "--file") && i + 1 < argc) {
            file_path = argv[++i];
        } else if ((arg == "-flrecl" || arg == "--flrecl") && i + 1 < argc) {
            flrecl_limit = static_cast<size_t>(stoi(argv[++i]));
        } else if ((arg == "-output" || arg == "--output") && i + 1 < argc) {
            log_file = argv[++i];
        } else if (arg == "-about" || arg == "--about") {
            cout << "====================================================\n";
            cout << " | Program: FLRECL CFT Transfer Line Analyzer\n";
            cout << " | Purpose: Analyze text or binary files to detect lines\n";
            cout << " |          exceeding a specified FLRECL (record length).\n";
            cout << " | Developed by: YAHYA SGHIOURI (FLUX-SI)\n";
            cout << " | Version: 1.0\n";
            cout << " | Language: C++17\n";
            cout << "====================================================\n";
            
            return 0;
        } else if (arg == "-h" || arg == "--help") {
            cout << "Usage: analyze_flrecl.exe -file <inputfile> [-flrecl 156] [-output logfile]\n";
            cout << "Options:\n";
            cout << "  -file <path>       Specify input file to analyze\n";
            cout << "  -flrecl <bytes>    Set FLRECL limit (default 156)\n";
            cout << "  -output <path>     Set output log file name\n";
            cout << "  -about             Display program information\n";
            cout << "  -h, --help         Show this help message\n";
            return 0;
        }
    }

    if (file_path.empty()) {
        cerr << "Error: Missing input file. Use -file <path>.\n";
        return 1;
    }

    if (log_file.empty()) {
        log_file = "FLRECL_log_" + current_timestamp() + ".txt";
    }

    analyze_file_flrecl(file_path, flrecl_limit, log_file);
    return 0;
}
