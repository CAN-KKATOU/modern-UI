//
// Created by Astria on 2022/7/22.
//

#ifndef ANALYSIS_CORE_C__17_DIRCALL_H
#define ANALYSIS_CORE_C__17_DIRCALL_H

#include <iostream>

using std::string;
using std::move;

namespace asdu {

    class DirCall {
    public:
        const int oie{1};            // 操作标识
        size_t _id{0ULL};            // 目录ID
        size_t name_length{0};       // 目录名称长度
        string _name;                // 目录名称
        bool _flag{false};           // 召唤标志,false为所有文件
        string _sTime;               // 查询起始时间
        string _eTime;               // 查询终止时间

    public:
        DirCall() = default;

        DirCall(size_t id, string dir_name, bool flag, string start_time, string end_time)
                : _id(id), _name(move(dir_name)), _flag(flag), _sTime(move(start_time)), _eTime(move(end_time)) {
            name_length = _name.length();
        }

        DirCall(const DirCall &rhs)
                : _id(rhs._id),
                  name_length(rhs.name_length),
                  _name(rhs._name),
                  _flag(rhs._flag),
                  _sTime(rhs._sTime),
                  _eTime(rhs._eTime) {}

        ~DirCall() = default;

        DirCall &operator=(const DirCall &rhs) {
            _id = rhs._id;
            name_length = rhs.name_length;
            _name = rhs._name;
            _flag = rhs._flag;
            _sTime = rhs._sTime;
            _eTime = rhs._eTime;
            return *this;
        }
    };

} // asdu

#endif //ANALYSIS_CORE_C__17_DIRCALL_H
