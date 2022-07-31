//
// Created by Astria on 2022/7/22.
//

#ifndef ANALYSIS_CORE_C__17_READFILEDATACONFIRM_H
#define ANALYSIS_CORE_C__17_READFILEDATACONFIRM_H

#include <iostream>

using std::string;
using std::move;

namespace asdu {

    class ReadFileDataConfirm {
    public:
        const int oie{6};
        size_t _id{0ULL};
        size_t _number{0ULL};
        bool _flag{false};

    public:
        ReadFileDataConfirm() = default;

        ReadFileDataConfirm(size_t id, size_t number, bool flag)
                : _id(id), _number(number), _flag(flag) {}

        ReadFileDataConfirm(const ReadFileDataConfirm &rhs) : _id(rhs._id), _number(rhs._number), _flag(rhs._flag) {}

        ~ReadFileDataConfirm() = default;

        ReadFileDataConfirm &operator=(const ReadFileDataConfirm &rhs) {
            _id = rhs._id;
            _number = rhs._number;
            _flag = rhs._flag;
            return *this;
        }
    };

} // asdu

#endif //ANALYSIS_CORE_C__17_READFILEDATACONFIRM_H
