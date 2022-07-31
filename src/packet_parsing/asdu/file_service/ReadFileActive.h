//
// Created by Astria on 2022/7/22.
//

#ifndef ANALYSIS_CORE_C__17_READFILEACTIVE_H
#define ANALYSIS_CORE_C__17_READFILEACTIVE_H

#include <iostream>

using std::string;
using std::move;

namespace asdu {

    class ReadFileActive {
    public:
        const int oie{3};
        size_t file_length{0ULL};
        string _file;

    public:
        ReadFileActive() = default;

        explicit ReadFileActive(string file_name) : _file(move(file_name)) {
            file_length = _file.length();
        }

        ReadFileActive(const ReadFileActive &rhs) : file_length(rhs.file_length), _file(rhs._file) {}

        ~ReadFileActive() = default;

        ReadFileActive &operator=(const ReadFileActive &rhs) {
            file_length = rhs.file_length;
            _file = rhs._file;
            return *this;
        }
    };

} // asdu

#endif //ANALYSIS_CORE_C__17_READFILEACTIVE_H
