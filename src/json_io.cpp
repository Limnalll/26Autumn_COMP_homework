#include "json_io.h"
#include <fstream>
#include <iostream>

using namespace std;

// 读取 JSON 文件
json loadJson(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        throw runtime_error("无法打开文件：" + filename);
    }

    json data;
    file >> data;

    return data;
}

// 写入 JSON 文件
bool saveJson(const string& filename, const json& data) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "无法写入文件：" << filename << '\n';
        return false;
    }

    file << data.dump(4) << '\n';

    if (!file) {
        cerr << "写入失败！\n";
        return false;
    }

    return true;
}