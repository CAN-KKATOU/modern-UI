//
// Created by Astria on 2022/4/25.
//

#include "RemoteTest.h"

#include <memory>

namespace asdu {
    RemoteTest::RemoteTest(unique_ptr<vector<int>> data, const ASDULengthSet &als)
            : BaseASDU(move(data), als) {
        long long flag = (long) (pow(2, asduLength.qualifier * 8 - 1));
        _sq = vsq >= flag;
        _count = vsq % flag;
        _isFloat = tie == 13;

        size_t cIndex{static_cast<size_t>(asduLength.type +
                                          asduLength.qualifier +
                                          asduLength.cot +
                                          asduLength.commonAd)};

        try {
            size_t addFirst{util::vecToULL(data, cIndex, asduLength.objectAd)}; // 首个元素地址

            int elementLength;
            if (_sq) {
                cIndex += asduLength.objectAd;
                if (_isFloat)
                    elementLength = 4 + asduLength.describer;
                else elementLength = 2 + asduLength.describer;
            } else {
                if (_isFloat)
                    elementLength = asduLength.objectAd + 4 + asduLength.describer;
                else elementLength = asduLength.objectAd + 2 + asduLength.describer;
            }

            if (_isFloat) {
#if __cplusplus >= 201403L
                _fVec = make_unique<vector<FloatElem>>();
#elif  __cplusplus < 201403L
                _fVec = unique_ptr<vector<FloatElem>>(new vector<FloatElem>);
#endif
                for (int index = 0; index < _count; index++) {
                    try {
                        FloatElem element;
                        if (_sq) {
                            element.address = addFirst + index;

                            element.value = util::parseIeee574(util::vecMid(data, cIndex, 4));

                            element.qds = util::vecToLL(data, cIndex + 4, asduLength.describer);
                        } else {
                            element.address = util::vecToULL(data, cIndex, asduLength.objectAd);

                            element.value = util::parseIeee574(
                                    util::vecMid(data, cIndex + asduLength.objectAd, 4));

                            element.qds = util::vecToLL(data, cIndex + asduLength.objectAd + 4,
                                                        asduLength.describer);
                        }

                        _fVec->push_back(element);
                        cIndex += elementLength;
                    } catch (const std::invalid_argument &e) {
                        throw e;
                    }
                    catch (const std::out_of_range &e) {
                        throw e;
                    }
                }
            } else {
#if __cplusplus >= 201403L
                _nVec = make_unique<vector<NvaElem>>();
#elif __cplusplus < 201403L
                _nVec = unique_ptr<vector<NvaElem>>(new vector<NvaElem>);
#endif

                for (int index = 0; index < _count; index++) {
                    try {
                        NvaElem element;
                        if (_sq) {
                            element.address = addFirst + index;

                            element.value = util::vecToInt(data, cIndex, 2);

                            element.qds = util::vecToLL(data, cIndex + 2, asduLength.describer);
                        } else {
                            element.address = util::vecToULL(data, cIndex, asduLength.objectAd);

                            element.value = util::vecToInt(data, cIndex + asduLength.objectAd, 2);

                            element.qds = util::vecToLL(data, cIndex + asduLength.objectAd + 2,
                                                        asduLength.describer);
                        }

                        _nVec->push_back(element);
                        cIndex += elementLength;
                    } catch (const std::invalid_argument &e) {
                        throw e;
                    }
                    catch (const std::out_of_range &e) {
                        throw e;
                    }
                }
            }
        } catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
        }
        catch (const std::out_of_range &e) {
            std::cerr << e.what() << std::endl;
        }

        switch (cot) {
            case 1:
            case 2:
            case 3:
            case 5:
            case 20:
                break;
            default:
                errInfo += "传送原因COT错误 ";
                break;
        }
    }

    shared_ptr<json> RemoteTest::toJson() {
        auto object = make_shared<json>();
        (*object)["ASDU报文类型"] = "遥测命令报文";
        switch (tie) {
            case 9:
                (*object)["标识类型"] = "测量值，归一化值";
                break;
            case 11:
                (*object)["标识类型"] = "测量值，标度化值";
                break;
            case 13:
                (*object)["标识类型"] = "测量值，短浮点数";
                break;
        }

        switch (cot) {
            case 1:
                (*object)["传送原因"] = "周期循环";
                break;
            case 2:
                (*object)["传送原因"] = "背景扫描";
                break;
            case 3:
                (*object)["传送原因"] = "突发（自发）";
                break;
            case 5:
                (*object)["传送原因"] = "被请求";
                break;
            case 20:
                (*object)["传送原因"] = "响应站召唤";
                break;
            default:
                (*object)["传送原因"] = "Data Error";
                break;
        }

        (*object)["公共地址"] = cAddr;

        auto objectList = new json;
        int index{1};
        if (_isFloat) {
            for (const auto &val: *_fVec) {
                json element;
                element["遥测信息对象地址"] = val.address;
                element["短浮点数值"] = val.value;
                element["品质描述词"] = val.qds;
                (*objectList)["对象" + std::to_string(index)] = element;
                index++;
            }
        } else {
            for (const auto &val: *_nVec) {
                json element;
                element["遥测信息对象地址"] = val.address;
                if (tie == 9)
                    element["归一化值"] = val.value;
                else element["标度化值"] = val.value;
                element["品质描述词"] = val.qds;
                (*objectList)["对象" + std::to_string(index)] = element;
                index++;
            }
        }
        (*object)["遥测对象列表"] = *objectList;
        delete objectList;
        return object;
    }

    shared_ptr<json> RemoteTest::getPosition() {
        auto object = make_shared<json>();

        size_t cIndex{static_cast<size_t>(asduLength.startIndex)};
        (*object)["ASDU报文类型"] = util::posJson(cIndex, cIndex + asduLength.type);
        (*object)["标识类型"] = util::posJson(cIndex, cIndex + asduLength.type);

        cIndex += asduLength.type + asduLength.qualifier;
        (*object)["传送原因"] = util::posJson(cIndex, cIndex + asduLength.cot);

        cIndex += asduLength.cot;
        (*object)["公共地址"] = util::posJson(cIndex, cIndex + asduLength.commonAd);

        cIndex += asduLength.commonAd;
        json addSQ = util::posJson(cIndex, cIndex + asduLength.objectAd);

        if (_sq)
            cIndex += asduLength.objectAd;

        size_t mark1{cIndex};
        auto objectList = new json;
        int index{1};
        if (_isFloat) {
            for (const auto &val: *_fVec) {
                size_t mark2{cIndex};
                json element;

                if (!_sq) {
                    element["遥测信息对象地址"] = util::posJson(cIndex, cIndex + asduLength.objectAd);
                    cIndex += asduLength.objectAd;
                } else
                    element["遥测信息对象地址"] = addSQ;

                element["短浮点数值"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;

                element["品质描述词"] = util::posJson(cIndex, cIndex + asduLength.describer);
                cIndex += asduLength.describer;

                (*objectList)["对象" + std::to_string(index)] = element;
                (*objectList)["对象" + std::to_string(index) + "位置"] = util::posJson(mark2, cIndex);

                index++;
            }
        } else {
            for (const auto &val: *_nVec) {
                size_t mark2{cIndex};
                json element;

                if (!_sq) {
                    element["遥测信息对象地址"] = util::posJson(cIndex, cIndex + asduLength.objectAd);
                    cIndex += asduLength.objectAd;
                } else
                    element["遥测信息对象地址"] = addSQ;

                if (tie == 9)
                    element["归一化值"] = util::posJson(cIndex, cIndex + 2);
                else element["标度化值"] = util::posJson(cIndex, cIndex + 2);
                cIndex += 2;

                element["品质描述词"] = util::posJson(cIndex, cIndex + asduLength.describer);
                cIndex += asduLength.describer;

                (*objectList)["对象" + std::to_string(index)] = element;
                (*objectList)["对象" + std::to_string(index) + "位置"] = util::posJson(mark2, cIndex);
                index++;
            }
        }
        (*object)["遥测对象列表"] = *objectList;
        (*object)["遥测对象列表位置"] = util::posJson(mark1, cIndex);
        delete objectList;
        return object;
    }

    RemoteTest::RemoteTest(const ASDULengthSet &asdu_length_set) : BaseASDU(asdu_length_set) {
        tie = 3;
        util::replaceElems(data, 0, asduLength.type, tie);

#if __cplusplus >= 201403L
        _nVec = make_unique<vector<NvaElem>>();
        _fVec = make_unique<vector<FloatElem>>();
#elif __cplusplus < 201403L
        _nVec = unique_ptr<vector<NvaElem>>(new vector<NvaElem>);
        _fVec = unique_ptr<vector<FloatElem>>(new vector<FloatElem>);
#endif
    }

    RemoteTest::RemoteTest(const ASDULengthSet &asdu_length_set, long long int tie, long long int vsq,
                           long long int cot, size_t command_address)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address) {
        long long flag = (long) (pow(2, asduLength.qualifier * 8 - 1));
        _sq = vsq >= flag;
        _count = vsq % flag;
        _isFloat = tie == 13;

        if (_sq)
            util::pushElems(data, asduLength.objectAd, 0);

        if (_isFloat) {
#if __cplusplus >= 201403L
            _fVec = make_unique<vector<FloatElem>>();
#elif __cplusplus < 201403L
            _fVec = unique_ptr<vector<FloatElem>>(new vector<FloatElem>);
#endif
        } else {
#if __cplusplus >= 201403L
            _nVec = make_unique<vector<NvaElem>>();
#elif __cplusplus < 201403L
            _nVec = unique_ptr<vector<NvaElem>>(new vector<NvaElem>);
#endif
        }
    }

    bool RemoteTest::add_fElem(const FloatElem &elem) {
        if (_isFloat) {
            if (_fVec) {
                _fVec->push_back(elem);
                if (!_sq)
                    util::pushElems(data, asduLength.objectAd, elem.address);

                util::pushElems(data, asduLength.describer, elem.qds);
                util::pushIeee574(data, elem.value);
                return true;
            } else
                return false;
        } else
            return false;
    }

    bool RemoteTest::add_fElems(const vector<FloatElem> &elems) {
        if (_isFloat) {
            if (_fVec) {
                for (auto const &elem: elems) {
                    _fVec->push_back(elem);
                    if (!_sq)
                        util::pushElems(data, asduLength.objectAd, elem.address);

                    util::pushElems(data, asduLength.describer, elem.qds);
                    util::pushIeee574(data, elem.value);
                }
                return true;
            } else
                return false;
        } else
            return false;
    }

    bool RemoteTest::add_fElems(const shared_ptr<vector<FloatElem>> &elems) {
        if (_isFloat) {
            if (_fVec) {
                for (auto const &elem: *elems) {
                    _fVec->push_back(elem);
                    if (!_sq)
                        util::pushElems(data, asduLength.objectAd, elem.address);

                    util::pushElems(data, asduLength.describer, elem.qds);
                    util::pushIeee574(data, elem.value);
                }
                return true;
            } else
                return false;
        } else
            return false;
    }

    bool RemoteTest::add_nElem(const NvaElem &elem) {
        if (!_isFloat) {
            if (_nVec) {
                _nVec->push_back(elem);
                if (!_sq)
                    util::pushElems(data, asduLength.objectAd, elem.address);

                util::pushElems(data, asduLength.describer, elem.qds);
                util::pushElems(data, 2, elem.value);
                return true;
            } else
                return false;
        } else
            return false;
    }

    bool RemoteTest::add_nElems(const vector<NvaElem> &elems) {
        if (!_isFloat) {
            if (_nVec) {
                for (const auto &elem: elems) {
                    _nVec->push_back(elem);
                    if (!_sq)
                        util::pushElems(data, asduLength.objectAd, elem.address);

                    util::pushElems(data, asduLength.describer, elem.qds);
                    util::pushElems(data, 2, elem.value);
                }
                return true;
            } else
                return false;
        } else
            return false;
    }

    bool RemoteTest::add_nElems(const shared_ptr<vector<NvaElem>> &elems) {
        if (!_isFloat) {
            if (_nVec) {
                for (const auto &elem: *elems) {
                    _nVec->push_back(elem);
                    if (!_sq)
                        util::pushElems(data, asduLength.objectAd, elem.address);

                    util::pushElems(data, asduLength.describer, elem.qds);
                    util::pushElems(data, 2, elem.value);
                }
                return true;
            } else
                return false;
        } else
            return false;
    }

    void RemoteTest::set_tie(long long int tie) {
        BaseASDU::set_tie(tie);
        _isFloat = tie == 13;
    }

    void RemoteTest::set_vsq(long long int vsq) {
        BaseASDU::set_vsq(vsq);
        long long flag = (long) (pow(2, asduLength.qualifier * 8 - 1));
        _sq = vsq >= flag;
        _count = vsq % flag;
    }

    bool RemoteTest::set_sq_addr(size_t addr) {
        if (_sq) {
            size_t index{static_cast<size_t>(asduLength.type +
                                             asduLength.qualifier +
                                             asduLength.cot +
                                             asduLength.commonAd)};
            if (data->size() < index + asduLength.objectAd)
                util::pushElems(data, asduLength.objectAd, addr);
            else
                util::replaceElems(data, index, asduLength.objectAd, addr);
            return true;
        } else
            return false;
    }

    RemoteTest::RemoteTest(const RemoteTest &rhs)
            : BaseASDU(rhs), _count(rhs._count), _sq(rhs._sq), _isFloat(rhs._isFloat) {
        if (_isFloat) {
#if __cplusplus >= 201403L
            _fVec = make_unique<vector<FloatElem>>();
#elif __cplusplus < 201403L
            _fVec = unique_ptr<vector<FloatElem>>(new vector<FloatElem>);
#endif
            if (rhs._fVec)
                _fVec->assign(rhs._fVec->cbegin(), rhs._fVec->cend());
            else
                std::cerr << "RemoteTest(const RemoteTest&): _fVec is nullptr." << std::endl;
        } else {
#if __cplusplus >= 201403L
            _nVec = make_unique<vector<NvaElem>>();
#elif __cplusplus < 201403L
            _nVec = unique_ptr<vector<NvaElem>>(new vector<NvaElem>);
#endif
            if (rhs._nVec)
                _nVec->assign(rhs._nVec->cbegin(), rhs._nVec->cend());
            else
                std::cerr << "RemoteTest(const RemoteTest&): _nVec is nullptr." << std::endl;
        }
    }

    RemoteTest &RemoteTest::operator=(const RemoteTest &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);
        _count = rhs._count;
        _sq = rhs._sq;
        _isFloat = rhs._isFloat;
        if (_isFloat) {
            if (!_fVec) {
#if __cplusplus >= 201403L
                _fVec = make_unique<vector<FloatElem>>();
#elif __cplusplus < 201403L
                _fVec = unique_ptr<vector<FloatElem>>(new vector<FloatElem>);
#endif
            }
            if (rhs._fVec)
                _fVec->assign(rhs._fVec->cbegin(), rhs._fVec->cend());
            else
                std::cerr << "RemoteTest(const RemoteTest&): _fVec is nullptr." << std::endl;
        } else {
            if (!_nVec) {
#if __cplusplus >= 201403L
                _nVec = make_unique<vector<NvaElem>>();
#elif __cplusplus < 201403L
                _nVec = unique_ptr<vector<NvaElem>>(new vector<NvaElem>);
#endif
            }
            if (rhs._nVec)
                _nVec->assign(rhs._nVec->cbegin(), rhs._nVec->cend());
            else
                std::cerr << "RemoteTest(const RemoteTest&): _nVec is nullptr." << std::endl;
        }
        return *this;
    }
} // asdu
