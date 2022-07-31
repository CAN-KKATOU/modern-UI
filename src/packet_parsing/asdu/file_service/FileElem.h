//
// Created by Astria on 2022/7/22.
//

#ifndef ANALYSIS_CORE_C__17_FILEELEM_H
#define ANALYSIS_CORE_C__17_FILEELEM_H

#include <string>
#include <utility>

using std::string;
using std::move;

namespace asdu {
    class FileElem {
    public:
        size_t name_length{0};   // 文件名称长度
        string name;             // 文件名
        int _property{0};        // 文件属性
        size_t _size{0ULL};            // 文件大小
        size_t _id{0ULL};              // 文件ID
        string _time;            // 文件时间

    public:
        FileElem() = default;

        FileElem(string file_name, size_t size, size_t id = 0, string time = "", int property = 0)
                : name(move(file_name)), _size(size), _time(move(time)), _id(id), _property(property) {
            name_length = name.length();
        }

        FileElem(const FileElem &rhs)
                : name_length(rhs.name_length),
                  name(rhs.name),
                  _property(rhs._property),
                  _size(rhs._size),
                  _id(rhs._id),
                  _time(rhs._time) {}

        ~FileElem() = default;

        FileElem &operator=(const FileElem &rhs) {
            name_length = rhs.name_length;
            name = rhs.name;
            _property = rhs._property;
            _size = rhs._size;
            _id = rhs._id;
            _time = rhs._time;
            return *this;
        }
    };
}


#endif //ANALYSIS_CORE_C__17_FILEELEM_H
