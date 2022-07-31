//
// Created by Astria on 2022/7/22.
//

#ifndef ANALYSIS_CORE_C__17_WRITEFILEDATACONFIRM_H
#define ANALYSIS_CORE_C__17_WRITEFILEDATACONFIRM_H

#include <iostream>

namespace asdu {

    class WriteFileDataConfirm {
    public:
        const int oie{10};
        size_t _id{0ULL};
        size_t _number{0ULL};
        int _res{0};

    public:
        WriteFileDataConfirm() = default;

        WriteFileDataConfirm(size_t id, size_t number, int res) : _id(id), _number(number), _res(res) {}

        WriteFileDataConfirm(const WriteFileDataConfirm &rhs) : _id(rhs._id), _number(rhs._number), _res(rhs._res) {}

        ~WriteFileDataConfirm() = default;

        WriteFileDataConfirm &operator=(const WriteFileDataConfirm &rhs) {
            _id = rhs._id;
            _number = rhs._number;
            _res = rhs._res;
            return *this;
        }
    };

} // asdu

#endif //ANALYSIS_CORE_C__17_WRITEFILEDATACONFIRM_H
