//
// Created by Astria on 2022/7/22.
//

#ifndef ANALYSIS_CORE_C__17_WRITEFILEDATA_H
#define ANALYSIS_CORE_C__17_WRITEFILEDATA_H

#include <iostream>
#include <utility>

using std::move;

namespace asdu {

    class WriteFileData {
    public:
        const int oie{9};
        size_t _id{0ULL};
        size_t _number{0ULL};
        bool _flag{false};
        string _data;
        int _data_length{0};
        int parity{0};

    public:
        WriteFileData() = default;

        WriteFileData(size_t id, size_t number, bool flag, string data)
                : _id(id), _number(number), _flag(flag), _data(move(data)) {
            for (const auto &val: _data)
                parity += static_cast<int>(val);

            parity = parity % 256;
        }

        WriteFileData(const WriteFileData &rhs)
                : _id(rhs._id),
                  _number(rhs._number),
                  _flag(rhs._flag),
                  _data(rhs._data),
                  _data_length(rhs._data_length),
                  parity(rhs.parity) {}

        ~WriteFileData() = default;

        WriteFileData &operator=(const WriteFileData &rhs) {
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

#endif //ANALYSIS_CORE_C__17_WRITEFILEDATA_H
