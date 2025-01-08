#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <algorithm>
#include <cctype> // for std::toupper and std::tolower

class Util {
public:
    // ��̬���������ַ���ת��Ϊ��д
    static std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::toupper(c); });
        return result;
    }

    // ��̬���������ַ���ת��ΪСд
    static std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;
    }
};

#endif // UTIL_H
