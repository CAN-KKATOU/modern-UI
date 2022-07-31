//
// Created by Astria on 2022/7/22.
//

#ifndef ANALYSIS_CORE_C__17_DIRCALLCONFIRM_H
#define ANALYSIS_CORE_C__17_DIRCALLCONFIRM_H

#include "FileElem.h"
#include <iostream>
#include <memory>

using std::string;
using std::unique_ptr;
using std::move;

namespace asdu {

    class DirCallConfirm {
    public:
        const int oie{2};
        bool _res{true};                       // 结果
        size_t _id{0ULL};                      // 目录ID
        bool _flag{false};                     // false为无后续
        size_t _count{0ULL};                   // 文件数
        unique_ptr<vector<FileElem>> _files;   // 文件列表(长度、名称、属性、大小、时间)

    public:
        DirCallConfirm() = default;

        DirCallConfirm(bool res, size_t id, bool flag) : _res(res), _id(id), _flag(flag) {
            _files = unique_ptr<vector<FileElem>>(new vector<FileElem>);
        }

        DirCallConfirm(const DirCallConfirm &rhs) : _res(rhs._res), _id(rhs._id), _flag(rhs._flag), _count(rhs._count) {
            _files = unique_ptr<vector<FileElem>>(new vector<FileElem>);
            *_files = (*rhs._files);
        }

        ~DirCallConfirm() = default;

        void add_file(const FileElem &elem) {
            _files->push_back(elem);
            _count = _files->size();
        }

        void add_files(const vector<FileElem> &elems) {
            _files->insert(_files->end(), elems.begin(), elems.end());
            _count = _files->size();
        }

        void add_files(const shared_ptr<vector<FileElem>> &elems) {
            _files->insert(_files->end(), elems->begin(), elems->end());
            _count = _files->size();
        }

        DirCallConfirm &operator=(const DirCallConfirm &rhs) {
            _res = rhs._res;
            _id = rhs._id;
            _flag = rhs._flag;
            _count = rhs._count;
            if (!_files)
                _files = unique_ptr<vector<FileElem>>(new vector<FileElem>);

            if (rhs._files)
                _files->assign(rhs._files->cbegin(), rhs._files->cend());
            else
                std::cerr << "DirCallConfirm operator=(const DirCallConfirm&): _files is nullptr." << std::endl;

            return *this;
        }
    };

} // asdu

#endif //ANALYSIS_CORE_C__17_DIRCALLCONFIRM_H
