/*

To use this in-header library you just need to type #include "kapilib.h" in all files that use it
For more convenient use, type using namespace kapi; in those files

*/

#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <thread>
#include <vector>

using namespace std;

namespace kapi {
    // Get username
    string GetUsername();
    // Clear console
    void ClearScreen();
    // Pause execution of the program
    void Pause();
    // Return number of logical cores in CPU
    int GetCPUCoreCount();
    // Convert string to hexadecimal string (2nd argument - lowercase is 0, uppercase (default) is 1, 3rd is a separator)
    string StringToHex(const string& str, bool u = true, char sep = '\0');
    // Convert a file to a vector<unsigned char>, return 0 if file wasn't found
    bool FileToVector(const string& filename, vector<uint8_t>& data);
    // Convert a vector<unsigned char> to a file, create file if it doesn't exist
    void VectorToFile(const string& filename, const vector<uint8_t>& data);
}

#ifndef KAPI_IMPLEMENTED
#define KAPI_IMPLEMENTED

namespace kapi {
    inline string GetUsername() {
        #ifdef _WIN32
        const char* username = getenv("USERNAME");
        #else
        const char* username = getenv("USER");
        #endif
        if (username) return string(username); else return "";
    }
    inline void ClearScreen() {
        #ifdef _WIN32
        system("CLS");
        #else
        system("clear");
        #endif
    }
    inline void Pause() {
        cout << "Press ENTER to continue";
        cin.get();
    }
    inline int GetCPUCoreCount() {
        return thread::hardware_concurrency();
    }
    inline string StringToHex(const string& str, bool u, char sep) {
        stringstream ss;
        ss << hex;
        int len = str.length();
        if (u) ss << uppercase;
        for (int i = 0; i < len; i++) {
            ss << setw(2) << setfill('0') << static_cast<int>(str[i]) << sep;
            if (sep != '\0' && i != len - 1) {
                ss << sep;
            }
        }
        return ss.str();
    }
    inline bool FileToVector(const string& filename, vector<uint8_t>& data) {
        ifstream in(filename, ios::binary);
        if (!in) {
            cerr << filename << "wasn't found\n";
            return 0;
        }
        in.seekg(0, ios::end);
        size_t textfilesize = in.tellg();
        in.seekg(0, ios::beg);
        data.resize(textfilesize);
        in.read(reinterpret_cast<char*>(data.data()), textfilesize);
        in.close();
        return 1;
    }
    inline void VectorToFile(const string& filename, const vector<uint8_t>& data) {
        ifstream checkFile(filename, ios::binary);
        if (!checkFile) {
            #ifdef _WIN32
            system(("echo.>" + filename).c_str());
            #else
            system(("touch " + filename).c_str());
            #endif
        }
        ofstream out(filename, ios::binary);
        out.write(reinterpret_cast<const char*>(data.data()), data.size());
        out.close();
    }
}
#endif