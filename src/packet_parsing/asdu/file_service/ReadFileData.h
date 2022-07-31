//
// Created by Astria on 2022/7/22.
//

#ifndef ANALYSIS_CORE_C__17_READFILEDATA_H
#define ANALYSIS_CORE_C__17_READFILEDATA_H

#include <iostream>
#include <utility>
#include <cmath>

using std::string;
using std::move;

namespace asdu {

    class ReadFileData {
    public:
        const int oie{5};
        size_t _id{0ULL};
        size_t _number{0LL};
        bool _flag{false};
        string _data;
        int _data_length{0};
        int parity{0};

    public:
        ReadFileData() = default;

        ReadFileData(size_t id, size_t number, bool flag, string data)
                : _id(id), _number(number), _flag(flag), _data(move(data)) {
            for (const auto &val: _data)
                parity += static_cast<int>(val);

            parity = parity % 256;
        }

        ReadFileData(const ReadFileData &rhs)
                : _id(rhs._id),
                  _number(rhs._number),
                  _flag(rhs._number),
                  _data(rhs._data),
                  _data_length(rhs._data_length),
                  parity(rhs.parity) {}

        ~ReadFileData() = default;

        ReadFileData &operator=(const ReadFileData &rhs) {
            _id = rhs._id;
            _number = rhs._number;
            _flag = rhs._flag;
            _data = rhs._data;
            _data_length = rhs._data_length;
            parity = rhs.parity;
            return *this;
        }
    };

} // asdu

#endif //ANALYSIS_CORE_C__17_READFILEDATA_H
