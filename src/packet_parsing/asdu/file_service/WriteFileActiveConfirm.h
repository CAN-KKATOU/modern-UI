//
// Created by Astria on 2022/7/22.
//

#ifndef ANALYSIS_CORE_C__17_WRITEFILEACTIVECONFIRM_H
#define ANALYSIS_CORE_C__17_WRITEFILEACTIVECONFIRM_H

#include "FileElem.h"

namespace asdu {

    class WriteFileActiveConfirm {
    public:
        const int oie{8};
        int _res{0};
        FileElem _file; // 长度、名称、ID、大小

    public:
        WriteFileActiveConfirm() = default;

        explicit WriteFileActiveConfirm(int res, const FileElem &file) : _res(res), _file(file) {}

        WriteFileActiveConfirm(const WriteFileActiveConfirm &rhs) : _res(rhs._res), _file(rhs._file) {}

        ~WriteFileActiveConfirm() = default;

        WriteFileActiveConfirm &operator=(const WriteFileActiveConfirm &rhs) {
            _res = rhs._res;
            _file = rhs._file;
            return *this;
        }
    };

} // asdu

#endif //ANALYSIS_CORE_C__17_WRITEFILEACTIVECONFIRM_H
