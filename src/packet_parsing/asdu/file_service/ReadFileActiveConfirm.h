//
// Created by Astria on 2022/7/22.
//

#ifndef ANALYSIS_CORE_C__17_READFILEACTIVECONFIRM_H
#define ANALYSIS_CORE_C__17_READFILEACTIVECONFIRM_H

#include "FileElem.h"
#include <iostream>

using std::string;
using std::move;

namespace asdu {

    class ReadFileActiveConfirm {
    public:
        const int oie{4};
        bool _res{true};
        FileElem _file;  // 长度、名称、ID、大小

    public:
        ReadFileActiveConfirm() = default;

        explicit ReadFileActiveConfirm(bool res, const FileElem &file) : _res(res), _file(file) {}

        ReadFileActiveConfirm(const ReadFileActiveConfirm &rhs) : _res(rhs._res), _file(rhs._file) {}

        ~ReadFileActiveConfirm() = default;

        ReadFileActiveConfirm &operator=(const ReadFileActiveConfirm &rhs) {
            _res = rhs._res;
            _file = rhs._file;
            return *this;
        }
    };

} // asdu

#endif //ANALYSIS_CORE_C__17_READFILEACTIVECONFIRM_H
