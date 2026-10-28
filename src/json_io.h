#ifndef JSON_IO_H
#define JSON_IO_H

#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// 从文件读取 JSON
json loadJson(const std::string& filename);

// 将 JSON 写入文件
bool saveJson(const std::string& filename, const json& data);

#endif