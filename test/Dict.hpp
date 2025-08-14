#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<unordered_map>
#include<algorithm>
#include<fstream>

const std::string default_dict_path = "./dictionary.txt";

class Dict
{
public:
    Dict(const std::string &path = default_dict_path):_path(path)
    {}
    bool Load(){
        std::ifstream ifs(_path);
        if(!ifs.is_open()){
            return false;
        }
        std::string line;
        while(std::getline(ifs, line)){
            // 忽略空行
            if(line.empty()) continue;
            // 查找分隔符
            std::size_t pos = line.find(':');
            if(pos == std::string::npos) continue;

            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            // 去除首尾空白
            auto trim = [](std::string &s){
                auto not_space = [](int ch){ return !std::isspace(ch); };
                s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
                s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
            };
            trim(key);
            trim(value);

            if(!key.empty() && !value.empty()){
                _dict[key] = value;
            }
        }
        return true;
    }

    std::string Translate(const std::string &word){
        auto it = _dict.find(word);
        if(it != _dict.end()){
            return it->second;
        }
        return "not found";
    }

private:
    std::string _path;
    std::unordered_map<std::string, std::string> _dict;
};