//
// Created by Astria on 2022/4/25.
//

#include "ReadParameter.h"

namespace asdu {
    ReadParameter::ReadParameter(unique_ptr<vector<int>> data, const ASDULengthSet &als)
            : BaseASDU(move(data), als) {
        _dir = cot == 6;
        size_t cIndex{static_cast<size_t>(asduLength.type +
                                          asduLength.qualifier +
                                          asduLength.cot +
                                          asduLength.commonAd)};

        try {
            _sn = util::vecToLL(data, cIndex, asduLength.sn);
        } catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
        }
        catch (const std::out_of_range &e) {
            std::cerr << e.what() << std::endl;
        }

        cIndex += asduLength.sn;

        if (_dir) {
            is_read_all = vsq == 0;
            if (!is_read_all) {
#if __cplusplus >= 201403L
                _aVec = make_unique<vector<long long>>();
#elif  __cplusplus < 201403L
                _aVec = unique_ptr<vector<long long>>(new vector<long long>);
#endif
                _count = vsq % (long) (pow(2, asduLength.qualifier * 8 - 1));
                for (int index = 0; index < _count; index++) {
                    try {
                        _aVec->push_back(util::vecToLL(data, cIndex, asduLength.objectAd));
                        cIndex += asduLength.objectAd;
                    } catch (const std::out_of_range &e) {
                        std::cerr << e.what() << std::endl;
                    }
                }
            }
        } else {
            _features = data->at(cIndex++);

            _count = vsq % (long) (pow(2, asduLength.qualifier * 8 - 1));

#if __cplusplus >= 201403L
            _oVec = make_unique<vector<ReadElem>>();
#elif __cplusplus < 201403L
            _oVec = unique_ptr<vector<ReadElem>>(new vector<ReadElem>);
#endif

            for (int index{0}; index < _count; index++) {
                try {
                    ReadElem element;

                    element.address = util::vecToULL(data, cIndex, asduLength.objectAd);
                    element.tag = data->at(cIndex + asduLength.objectAd);
                    element.value_length = data->at(cIndex + asduLength.objectAd + 1);
                    element.value = util::vecToHexStr(data, cIndex + asduLength.objectAd + 2,
                                                      element.value_length);

                    _oVec->push_back(element);
                    cIndex += asduLength.objectAd + 2 + element.value_length;
                } catch (const std::invalid_argument &e) {
                    std::cerr << e.what() << std::endl;
                }
                catch (const std::out_of_range &e) {
                    std::cerr << e.what() << std::endl;
                }
            }
        }

        switch (cot) {
            case 6:
            case 7:
            case 47:
                break;
            default:
                errInfo += "传送原因COT错误 ";
                break;
        }
    }

    shared_ptr<json> ReadParameter::toJson() {
        auto object = make_shared<json>();
        (*object)["ASDU报文类型"] = "读参数和定值报文";
        switch (cot) {
            case 6:
                (*object)["传送原因"] = "激活";
                (*object)["传送方向"] = "控制方向";
                break;
            case 7:
                (*object)["传送原因"] = "激活确认";
                (*object)["传送方向"] = "监视方向";
                break;
            case 47:
                (*object)["传送原因"] = "激活终止";
                (*object)["传送方向"] = "监视方向";
                break;
            default:
                (*object)["传送原因"] = "Data Error";
                break;
        }

        (*object)["公共地址"] = cAddr;
        (*object)["定值区号"] = _sn;

        if (_dir) {
            if (is_read_all)
                (*object)["读取操作"] = "读取全部";
            else {
                (*object)["读取操作"] = "读取多个";
                auto list1 = new json;
                int index = 1;
                for (const auto &val: *_aVec) {
                    (*list1)["信息体地址" + std::to_string(index)] = val;
                    index++;
                }
                (*object)["信息体地址列表"] = *list1;
                delete list1;
            }
        } else {
            (*object)["参数特征标识"] = util::parseFeature(_features);
            auto list1 = new json;
            int index = 1;
            for (const auto &val: *_oVec) {
                json element;
                element["信息体地址"] = val.address;
                element["Tag类型"] = val.tag;
                element["值"] = val.value;
                (*list1)["信息体" + std::to_string(index)] = element;
                index++;
            }
            (*object)["信息体列表"] = *list1;
            delete list1;
        }
        return object;
    }

    shared_ptr<json> ReadParameter::getPosition() {
        auto object = make_shared<json>();

        size_t cIndex{static_cast<size_t>(asduLength.startIndex)};
        (*object)["ASDU报文类型"] = util::posJson(cIndex, cIndex + asduLength.type);

        size_t vsqIndex{cIndex + asduLength.type};

        cIndex += asduLength.type + asduLength.qualifier;
        (*object)["传送原因"] = util::posJson(cIndex, cIndex + asduLength.cot);
        (*object)["传送方向"] = util::posJson(cIndex, cIndex + asduLength.cot);

        cIndex += asduLength.cot;
        (*object)["公共地址"] = util::posJson(cIndex, cIndex + asduLength.commonAd);

        cIndex += asduLength.commonAd;
        (*object)["定值区号"] = util::posJson(cIndex, cIndex + asduLength.sn);

        cIndex += asduLength.sn;

        if (_dir) {
            if (is_read_all)
                (*object)["读取操作"] = util::posJson(vsqIndex, vsqIndex + asduLength.qualifier);
            else {
                (*object)["读取操作"] = util::posJson(vsqIndex, vsqIndex + asduLength.qualifier);
                auto list1 = new json;
                size_t mark{cIndex};
                int index1{1};
                for (const auto &val: *_aVec) {
                    (*list1)["信息体地址" + std::to_string(index1)] = util::posJson(cIndex, cIndex +
                                                                                               asduLength.objectAd);
                    index1++;
                    cIndex += asduLength.objectAd;
                }
                (*object)["信息体地址列表"] = *list1;
                (*object)["信息体地址列表位置"] = util::posJson(mark, cIndex);
                delete list1;
            }
        } else {
            (*object)["参数特征标识"] = util::posJson(cIndex, cIndex + 1);
            cIndex++;

            auto list1 = new json;
            size_t mark1{cIndex};
            int index1{1};
            for (const auto &val: *_oVec) {
                size_t mark2{cIndex};
                json element;
                element["信息体地址"] = util::posJson(cIndex, cIndex + asduLength.objectAd);
                cIndex += asduLength.objectAd;

                element["Tag类型"] = util::posJson(cIndex, cIndex + 1);
                cIndex += 2;

                element["值"] = util::posJson(cIndex, cIndex + val.value_length);
                cIndex += val.value_length;

                (*list1)["信息体" + std::to_string(index1)] = element;
                (*list1)["信息体" + std::to_string(index1) + "位置"] = util::posJson(mark2, cIndex);
                index1++;
            }
            (*object)["信息体列表"] = *list1;
            (*object)["信息体列表位置"] = util::posJson(mark1, cIndex);
            delete list1;
        }
        return object;
    }

    ReadParameter::ReadParameter(const ASDULengthSet &asdu_length_set)
            : BaseASDU(asdu_length_set) {
        for (int i{0}; i < asduLength.sn; i++)
            data->push_back(0);
#if __cplusplus >= 201403L
        _oVec = make_unique<vector<ReadElem>>();
        _aVec = make_unique<vector<long long>>();
#elif __cplusplus < 201403L
        _oVec = unique_ptr<vector<ReadElem>>(new vector<ReadElem>);
        _aVec = unique_ptr<vector<long long>>(new vector<long long>);
#endif
    }

    ReadParameter::ReadParameter(const ASDULengthSet &asdu_length_set, long long int tie, long long int vsq,
                                 long long int cot, size_t command_address, long long int sn)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address), _sn(sn) {
        util::pushElems(data, asduLength.sn, _sn);
        _dir = cot == 6;
        if (_dir) {
            is_read_all = vsq == 0;
            if (is_read_all) {
#if __cplusplus >= 201403L
                _aVec = make_unique<vector<long long>>();
#elif __cplusplus < 201403L
                _aVec = unique_ptr<vector<long long>>(new vector<long long>);
#endif
            }
        } else {
            data->push_back(0); // 参数特征标识
#if __cplusplus >= 201403L
            _oVec = make_unique<vector<ReadElem>>();
#elif __cplusplus < 201403L
            _oVec = unique_ptr<vector<ReadElem>>(new vector<ReadElem>);
#endif
        }
    }

    void ReadParameter::set_sn(long long int sn) {
        _sn = sn;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd)};
        util::replaceElems(data, index, asduLength.sn, _sn);
    }

    bool ReadParameter::set_features(int features) {
        if (!_dir) {
            _features = features;
            size_t index{static_cast<size_t>(asduLength.type +
                                             asduLength.qualifier +
                                             asduLength.cot +
                                             asduLength.commonAd +
                                             asduLength.sn)};
            data->at(index) = _features;
            return true;
        } else
            return false;
    }

    bool ReadParameter::add_addr(long long int addr) {
        if (_dir and !is_read_all) {
            _aVec->push_back(addr);
            util::pushElems(data, asduLength.objectAd, addr);
            return true;
        } else
            return false;
    }

    bool ReadParameter::add_addrs(const vector<long long int> &addrs) {
        if (_dir and !is_read_all) {
            for (const auto &addr: addrs) {
                _aVec->push_back(addr);
                util::pushElems(data, asduLength.objectAd, addr);
            }
            return true;
        } else
            return false;
    }

    bool ReadParameter::add_addrs(const shared_ptr<vector<long long int>> &addrs) {
        if (_dir and !is_read_all) {
            for (const auto &addr: *addrs) {
                _aVec->push_back(addr);
                util::pushElems(data, asduLength.objectAd, addr);
            }
            return true;
        } else
            return false;
    }

    bool ReadParameter::add_elem(const ReadElem &elem) {
        if (!_dir) {
            _oVec->push_back(elem);
            util::pushElems(data, asduLength.objectAd, elem.address);
            data->push_back(elem.tag);
            data->push_back(elem.value_length);
            util::pushString(data, elem.value);
            return true;
        } else
            return false;
    }

    bool ReadParameter::add_elems(const vector<ReadElem> &elems) {
        if (!_dir) {
            for (const auto &elem: elems) {
                _oVec->push_back(elem);

                util::pushElems(data, asduLength.objectAd, elem.address);
                data->push_back(elem.tag);
                data->push_back(elem.value_length);
                util::pushString(data, elem.value);
            }
            return true;
        } else
            return false;
    }

    bool ReadParameter::add_elems(const shared_ptr<vector<ReadElem>> &elems) {
        if (!_dir) {
            for (const auto &elem: *elems) {
                _oVec->push_back(elem);

                util::pushElems(data, asduLength.objectAd, elem.address);
                data->push_back(elem.tag);
                data->push_back(elem.value_length);
                util::pushString(data, elem.value);
            }
            return true;
        } else
            return false;
    }

    void ReadParameter::set_vsq(long long int vsq) {
        BaseASDU::set_vsq(vsq);
        if (_dir)
            is_read_all = vsq == 0;

        if (_dir and !is_read_all)
            _count = vsq % (long) (pow(2, asduLength.qualifier * 8 - 1));
        else
            _count = vsq % (long) (pow(2, asduLength.qualifier * 8 - 1));
    }

    void ReadParameter::set_cot(long long int cot) {
        BaseASDU::set_cot(cot);
        _dir = cot == 6;
    }

    ReadParameter::ReadParameter(const ReadParameter &rhs)
            : BaseASDU(rhs), _sn(rhs._sn), _count(rhs._count), _dir(rhs._dir), is_read_all(rhs.is_read_all),
              _features(rhs._features) {
        if (_dir) {
            if (is_read_all) {
#if __cplusplus >= 201403L
                _aVec = make_unique<vector<long long>>();
#elif __cplusplus < 201403L
                _aVec = unique_ptr<vector<long long>>(new vector<long long>);
#endif

                if (rhs._aVec)
                    _aVec->assign(rhs._aVec->cbegin(), rhs._aVec->cend());
                else
                    std::cerr << "ReadParameter(const ReadParameter&): _aVec is nullptr." << std::endl;
            }
        } else {
#if __cplusplus >= 201403L
            _oVec = make_unique<vector<ReadElem>>();
#elif __cplusplus < 201403L
            _oVec = unique_ptr<vector<ReadElem>>(new vector<ReadElem>);
#endif

            if (rhs._oVec)
                _oVec->assign(rhs._oVec->cbegin(), rhs._oVec->cend());
            else
                std::cerr << "ReadParameter(const ReadParameter&): _oVec is nullptr." << std::endl;
        }
    }

    ReadParameter &ReadParameter::operator=(const ReadParameter &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);
        _sn = rhs._sn;
        _count = rhs._count;
        _dir = rhs._dir;
        is_read_all = rhs.is_read_all;
        _features = rhs._features;

        if (_dir) {
            if (is_read_all) {
                if (!_aVec) {
#if __cplusplus >= 201403L
                    _aVec = make_unique<vector<long long>>();
#elif __cplusplus < 201403L
                    _aVec = unique_ptr<vector<long long>>(new vector<long long>);
#endif
                }

                if (rhs._aVec)
                    _aVec->assign(rhs._aVec->cbegin(), rhs._aVec->cend());
                else
                    std::cerr << "ReadParameter(const ReadParameter&): _aVec is nullptr." << std::endl;
            }
        } else {
            if (!_oVec) {
#if __cplusplus >= 201403L
                _oVec = make_unique<vector<ReadElem>>();
#elif __cplusplus < 201403L
                _oVec = unique_ptr<vector<ReadElem>>(new vector<ReadElem>);
#endif
            }

            if (rhs._oVec)
                _oVec->assign(rhs._oVec->cbegin(), rhs._oVec->cend());
            else
                std::cerr << "ReadParameter(const ReadParameter&): _oVec is nullptr." << std::endl;
        }
        return *this;
    }
} // asdu