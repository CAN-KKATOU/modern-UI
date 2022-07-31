//
// Created by Astria on 2022/7/22.
//

#ifndef ANALYSIS_CORE_C__17_WRITEFILEACTIVE_H
#define ANALYSIS_CORE_C__17_WRITEFILEACTIVE_H

#include "FileElem.h"

namespace asdu {

    class WriteFileActive {
    public:
        const int oie{7};
        FileElem _file; // 长度、名称、ID、大小

    public:
        WriteFileActive() = default;

        explicit WriteFileActive(const FileElem &file) : _file(file) {}

        WriteFileActive(const WriteFileActive &rhs) : _file(rhs._file) {}

        ~WriteFileActive() = default;

        WriteFileActive &operator=(const WriteFileActive &rhs) {
            _file = rhs._file;
            return *this;
        }
    };

} // asdu

#endif //ANALYSIS_CORE_C__17_WRITEFILEACTIVE_H
