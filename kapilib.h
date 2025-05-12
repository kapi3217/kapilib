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
#include <sstream>
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
    // Convert vector to hexadecimal string (2nd argument - lowercase is 0, uppercase (default) is 1, 3rd is a separator)
    string ToHex(const vector<uint8_t> str, bool u = true, char sep = '\0');
    // Convert a file to a vector<unsigned char>, return 0 if file wasn't found
    bool FileToVector(const string& filename, vector<uint8_t>& data);
    // Convert a vector<unsigned char> to a file, create file if it doesn't exist
    void VectorToFile(const string& filename, const vector<uint8_t>& data);
    // Returns true if 2 files are different
    inline bool AreFilesDifferent(string f1, string f2);
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
    inline string ToHex(const vector<uint8_t> str, bool u, char sep) {
        stringstream ss;
        ss << hex;
        int len = str.size();
        if (u) ss << uppercase;
        for (int i = 0; i < len; i++) {
            ss << setw(2) << setfill('0') << static_cast<int>(str[i]);
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
    inline bool AreFilesDifferent(string f1, string f2) {
        bool flag = false;
        vector<uint8_t> data1, data2;
        if (!FileToVector(f1, data1)) return true;
        if (!FileToVector(f2, data2)) return true;
        size_t len1 = data1.size(), len2 = data2.size();
        if (len1 != len2) return 1;
        for (size_t i = 0; i < len1; i++) {
            if (data1[i] != data2[i]) {
                flag = 1;
                break;
            }
        }
        return flag;
    }
}
#endif
