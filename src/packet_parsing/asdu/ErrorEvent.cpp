//
// Created by Astria on 2022/4/25.
//

#include "ErrorEvent.h"

namespace asdu {
    ErrorEvent::ErrorEvent(unique_ptr<vector<int>> data, const ASDULengthSet &als)
            : BaseASDU(move(data), als) {
        size_t cIndex{static_cast<size_t>(asduLength.type +
                                          asduLength.qualifier +
                                          asduLength.cot +
                                          asduLength.commonAd)};

        _iCount = data->at(cIndex); // 故障遥信个数
        cIndex++;

        try {
            _iType = util::vecToLL(data, cIndex, asduLength.type); // 故障遥信类型
            cIndex += asduLength.type;

#if __cplusplus >= 201403L
            _iVec = make_unique<vector<ErrorInfoElem>>();
#elif __cplusplus < 201403L
            _iVec = unique_ptr<vector<ErrorInfoElem >>(new vector<ErrorInfoElem>);
#endif

            for (int index = 0; index < _iCount; index++) {
                try {
                    ErrorInfoElem element;

                    element.address = util::vecToULL(data, cIndex, asduLength.objectAd);
                    element.value = util::vecToLL(data, cIndex + asduLength.objectAd,
                                                  asduLength.describer);
                    element.time = util::parseTime(
                            util::vecMid(data, cIndex + asduLength.objectAd + asduLength.describer, 7));

                    _iVec->push_back(element);

                    cIndex += asduLength.objectAd + asduLength.describer;
                } catch (const std::invalid_argument &e) {
                    throw e;
                }
                catch (const std::out_of_range &e) {
                    throw e;
                }
            }

            _tCount = data->at(cIndex++);
            _tType = util::vecToLL(data, cIndex, asduLength.type);
            cIndex += asduLength.type;

            _isFloat = _tType == 13;

            if (_isFloat) {
#if __cplusplus >= 201403L
                _fVec = std::make_unique<std::vector<ErrorFloatElem>>();
#elif __cplusplus < 201403L
                _fVec = unique_ptr<vector<ErrorFloatElem >>(new vector<ErrorFloatElem>);
#endif
                for (int index = 0; index < _tCount; index++) {
                    try {
                        ErrorFloatElem element;

                        element.address = util::vecToULL(data, cIndex, asduLength.objectAd);
                        element.value = util::parseIeee574(
                                util::vecMid(data, cIndex + asduLength.objectAd, 4));

                        _fVec->push_back(element);
                        cIndex += asduLength.objectAd + 4;
                    } catch (const std::invalid_argument &e) {
                        throw e;
                    }
                    catch (const std::out_of_range &e) {
                        throw e;
                    }
                }
            } else {
#if __cplusplus >= 201403L
                _nVec = make_unique<vector<ErrorNvaElem>>();
#elif __cplusplus < 201403L
                _nVec = unique_ptr<vector<ErrorNvaElem >>(new vector<ErrorNvaElem>);
#endif

                for (int index = 0; index < _tCount; index++) {
                    try {
                        ErrorNvaElem element;

                        element.address = util::vecToULL(data, cIndex, asduLength.objectAd);
                        element.value = data->at(cIndex + asduLength.objectAd + 1) * 256 +
                                        data->at(cIndex + asduLength.objectAd);

                        _nVec->push_back(element);
                        cIndex += asduLength.objectAd + 2;
                    } catch (const std::invalid_argument &e) {
                        throw e;
                    }
                    catch (const std::out_of_range &e) {
                        throw e;
                    }
                }
            }
        }
        catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
        }
        catch (const std::out_of_range &e) {
            std::cerr << e.what() << std::endl;
        }

        if (cot != 3)
            errInfo += "传送原因COT错误 ";
    }

    shared_ptr<json> ErrorEvent::toJson() {
        auto object = make_shared<json>();

        (*object)["ASDU报文类型"] = "故障值信息报文";
        if (cot == 3)
            (*object)["传送原因"] = "突发（自发）";
        else (*object)["传送原因"] = "Data Error";

        (*object)["公共地址"] = cAddr;

        auto infoListObject = new json;
        int index{1};
        for (const auto &val: *_iVec) {
            json element;
            element["故障遥信点号"] = val.address;
            element["遥信值"] = val.value;
            element["故障时刻时标"] = val.time;
            (*infoListObject)["遥信对象" + std::to_string(index)] = element;
            index++;
        }
        (*object)["遥信对象列表"] = *infoListObject;
        delete infoListObject;

        auto testListObject = new json;
        index = 1;
        if (_isFloat) {
            for (const auto &val: *_fVec) {
                json element;
                element["遥测信息体地址"] = val.address;
                element["故障时刻数据（短浮点数）"] = val.value;
                (*testListObject)["遥测对象" + std::to_string(index)] = element;
                index++;
            }
        } else {
            for (const auto &val: *_nVec) {
                json element;
                element["遥测信息体地址"] = val.address;
                if (_tType == 9)
                    element["故障时刻数据（归一化值）"] = val.value;
                else element["故障时刻数据（标度化值）"] = val.value;
                (*testListObject)["遥测对象" + std::to_string(index)] = element;
                index++;
            }
        }
        (*object)["遥测对象列表"] = *testListObject;
        delete testListObject;
        return object;
    }

    shared_ptr<json> ErrorEvent::getPosition() {
        auto object = make_shared<json>();

        size_t cIndex{static_cast<size_t>(asduLength.startIndex)};

        (*object)["ASDU报文类型"] = util::posJson(cIndex, cIndex + asduLength.type);

        cIndex += asduLength.type + asduLength.qualifier;
        (*object)["传送原因"] = util::posJson(cIndex, cIndex + asduLength.cot);

        cIndex += asduLength.cot;
        (*object)["公共地址"] = util::posJson(cIndex, cIndex + asduLength.commonAd);

        cIndex += asduLength.commonAd;
        size_t mark1{cIndex};
        auto infoListObject = new json;
        int index{1};
        for (const auto &val: *_iVec) {
            size_t mark2{cIndex};
            json element;

            element["故障遥信点号"] = util::posJson(cIndex, cIndex + asduLength.objectAd);
            cIndex += asduLength.objectAd;

            element["遥信值"] = util::posJson(cIndex, cIndex + asduLength.describer);
            cIndex += asduLength.describer;

            element["故障时刻时标"] = util::posJson(cIndex, cIndex + 7);
            cIndex += 7;

            (*infoListObject)["遥信对象" + std::to_string(index)] = element;
            (*infoListObject)["遥信对象" + std::to_string(index) + "位置"] = util::posJson(mark2, cIndex);

            index++;
        }
        (*object)["遥信对象列表"] = *infoListObject;
        delete infoListObject;
        (*object)["遥信对象列表位置"] = util::posJson(mark1, cIndex);

        size_t mark3{cIndex};
        auto testListObject = new json;
        index = 1;
        if (_isFloat) {
            for (const auto &val: *_fVec) {
                size_t mark4{cIndex};
                json element;
                element["遥测信息体地址"] = util::posJson(cIndex, cIndex + asduLength.objectAd);
                cIndex += asduLength.objectAd;

                element["故障时刻数据（短浮点数）"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;

                (*testListObject)["遥测对象" + std::to_string(index)] = element;
                (*testListObject)["遥测对象" + std::to_string(index) + "位置"] = util::posJson(mark4, cIndex);

                index++;
            }
        } else {
            for (const auto &val: *_nVec) {
                size_t mark4{cIndex};
                json element;
                element["遥测信息体地址"] = util::posJson(cIndex, cIndex + asduLength.objectAd);
                cIndex += asduLength.objectAd;

                if (_tType == 9)
                    element["故障时刻数据（归一化值）"] = util::posJson(cIndex, cIndex + 2);
                else element["故障时刻数据（标度化值）"] = util::posJson(cIndex, cIndex + 2);
                cIndex += 2;

                (*testListObject)["遥测对象" + std::to_string(index)] = element;
                (*testListObject)["遥测对象" + std::to_string(index) + "位置"] = util::posJson(mark4, cIndex);

                index++;
            }
        }
        (*object)["遥测对象列表"] = *testListObject;
        (*object)["遥测对象列表位置"] = util::posJson(mark3, cIndex);
        delete testListObject;
        return object;
    }

    ErrorEvent::ErrorEvent(const ASDULengthSet &asdu_length_set)
            : BaseASDU(asdu_length_set) {
#if __cplusplus >= 201403L
        _iVec = make_unique<vector<ErrorInfoElem>>();
        _fVec = make_unique<vector<ErrorFloatElem>>();
        _nVec = make_unique<vector<ErrorNvaElem>>();
#elif __cplusplus < 201403L
        _iVec = unique_ptr<vector<ErrorInfoElem >>(new vector<ErrorInfoElem>);
        _fVec = unique_ptr<vector<ErrorFloatElem >>(new vector<ErrorFloatElem>);
        _nVec = unique_ptr<vector<ErrorNvaElem >>(new vector<ErrorNvaElem>);
#endif
    }

    ErrorEvent::ErrorEvent(const ASDULengthSet &asdu_length_set, long long int tie, long long int vsq,
                           long long int cot, size_t command_address)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address) {
#if __cplusplus >= 201403L
        _iVec = make_unique<vector<ErrorInfoElem>>();
        _fVec = make_unique<vector<ErrorFloatElem>>();
        _nVec = make_unique<vector<ErrorNvaElem>>();
#elif __cplusplus < 201403L
        _iVec = unique_ptr<vector<ErrorInfoElem >>(new vector<ErrorInfoElem>);
        _fVec = unique_ptr<vector<ErrorFloatElem >>(new vector<ErrorFloatElem>);
        _nVec = unique_ptr<vector<ErrorNvaElem >>(new vector<ErrorNvaElem>);
#endif
    }

    void ErrorEvent::set_info_count_type(int count, long long int type) {
        _iCount = count;
        _iType = type;

        data->push_back(_iCount);
        util::pushElems(data, asduLength.type, _iType);
    }

    void ErrorEvent::set_test_count_type(int count, long long int type) {
        _tCount = count;
        _tType = type;

        data->push_back(_tCount);
        util::pushElems(data, asduLength.type, _tType);
    }

    void ErrorEvent::add_info_elem(const ErrorInfoElem &elem) {
        _iVec->push_back(elem);

        util::pushElems(data, asduLength.type, elem.address);
        util::pushElems(data, asduLength.describer, elem.value);
        util::pushTime(data, elem.time);
    }

    void ErrorEvent::add_info_elems(const vector<ErrorInfoElem> &elems) {
        for (const auto &elem: elems) {
            _iVec->push_back(elem);

            util::pushElems(data, asduLength.type, elem.address);
            util::pushElems(data, asduLength.describer, elem.value);
            util::pushTime(data, elem.time);
        }
    }

    void ErrorEvent::add_info_elems(const shared_ptr<vector<ErrorInfoElem>> &elems) {
        for (const auto &elem: *elems) {
            _iVec->push_back(elem);

            util::pushElems(data, asduLength.type, elem.address);
            util::pushElems(data, asduLength.describer, elem.value);
            util::pushTime(data, elem.time);
        }
    }

    bool ErrorEvent::add_value_elem(const ErrorNvaElem &elem) {
        if (!_isFloat) {
            _nVec->push_back(elem);

            util::pushElems(data, asduLength.type, elem.address);
            data->push_back(elem.value / 256);
            data->push_back(elem.value % 256);

            return true;
        } else
            return false;
    }

    bool ErrorEvent::add_value_elems(const vector<ErrorNvaElem> &elems) {
        if (!_isFloat) {
            for (const auto &elem: elems) {
                _nVec->push_back(elem);

                util::pushElems(data, asduLength.type, elem.address);
                data->push_back(elem.value / 256);
                data->push_back(elem.value % 256);
            }
            return true;
        } else
            return false;
    }

    bool ErrorEvent::add_value_elems(const shared_ptr<vector<ErrorNvaElem>> &elems) {
        if (!_isFloat) {
            for (const auto &elem: *elems) {
                _nVec->push_back(elem);

                util::pushElems(data, asduLength.type, elem.address);
                data->push_back(elem.value / 256);
                data->push_back(elem.value % 256);
            }
            return true;
        } else
            return false;
    }

    bool ErrorEvent::add_float_elem(const ErrorFloatElem &elem) {
        if (_isFloat) {
            _fVec->push_back(elem);

            util::pushElems(data, asduLength.objectAd, elem.address);
            util::pushIeee574(data, elem.value);

            return true;
        } else
            return false;
    }

    bool ErrorEvent::add_float_elems(const vector<ErrorFloatElem> &elems) {
        if (_isFloat) {
            for (auto const &elem: elems) {
                _fVec->push_back(elem);

                util::pushElems(data, asduLength.objectAd, elem.address);
                util::pushIeee574(data, elem.value);
            }
            return true;
        } else
            return false;
    }

    bool ErrorEvent::add_float_elems(const shared_ptr<vector<ErrorFloatElem>> &elems) {
        if (_isFloat) {
            for (auto const &elem: *elems) {
                _fVec->push_back(elem);

                util::pushElems(data, asduLength.objectAd, elem.address);
                util::pushIeee574(data, elem.value);
            }
            return true;
        } else
            return false;
    }

    ErrorEvent::ErrorEvent(const ErrorEvent &rhs)
            : BaseASDU(rhs),
              _iCount(rhs._iCount), _tCount(rhs._tCount),
              _iType(rhs._iType), _tType(rhs._tType), _isFloat(rhs._isFloat) {
#if __cplusplus >= 201436L
        _iVec = make_unique<vector<ErrorInfoElem>>();
#elif __cplusplus < 201403L
        _iVec = unique_ptr<vector<ErrorInfoElem>>(new vector<ErrorInfoElem>);
#endif

        if (rhs._iVec)
            _iVec->assign(rhs._iVec->cbegin(), rhs._iVec->cend());
        else
            std::cerr << "ErrorEvent(const ErrorEvent&): _iVec is nullptr." << std::endl;

        if (_isFloat) {
#if __cplusplus >= 201403L
            _fVec = make_unique<vector<ErrorFloatElem>>();
#elif __cplusplus < 201403L
            _fVec = unique_ptr<vector<ErrorFloatElem>>(new vector<ErrorFloatElem>);
#endif
            if (rhs._fVec)
                _fVec->assign(rhs._fVec->cbegin(), rhs._fVec->cend());
            else
                std::cerr << "ErrorEvent(const ErrorEvent&): _fVec is nullptr." << std::endl;
        } else {
#if __cplusplus >= 201403L
            _nVec = make_unique<vector<ErrorNvaElem>>();
#elif __cplusplus < 201403L
            _nVec = unique_ptr<vector<ErrorNvaElem>>(new vector<ErrorNvaElem>);
#endif
            if (rhs._nVec)
                _nVec->assign(rhs._nVec->cbegin(), rhs._nVec->cend());
            else
                std::cerr << "ErrorEvent(const ErrorEvent&): _nVec is nullptr." << std::endl;
        }
    }

    ErrorEvent &ErrorEvent::operator=(const ErrorEvent &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);

        _iCount = rhs._iCount;
        _tCount = rhs._tCount;
        _iType = rhs._iType;
        _tType = rhs._tType;
        _isFloat = rhs._isFloat;

        if (!_iVec) {
#if __cplusplus >= 201436L
            _iVec = make_unique<vector<ErrorInfoElem>>();
#elif __cplusplus < 201403L
            _iVec = unique_ptr<vector<ErrorInfoElem>>(new vector<ErrorInfoElem>);
#endif
        }

        if (rhs._iVec)
            _iVec->assign(rhs._iVec->cbegin(), rhs._iVec->cend());
        else
            std::cerr << "ErrorEvent::operator=(const ErrorEvent&): _iVec is nullptr." << std::endl;

        if (_isFloat) {
            if (!_fVec) {
#if __cplusplus >= 201403L
                _fVec = make_unique<vector<ErrorFloatElem>>();
#elif __cplusplus < 201403L
                _fVec = unique_ptr<vector<ErrorFloatElem>>(new vector<ErrorFloatElem>);
#endif
            }

            if (rhs._fVec)
                _fVec->assign(rhs._fVec->cbegin(), rhs._fVec->cend());
            else
                std::cerr << "ErrorEvent::operator=(const ErrorEvent&): _fVec is nullptr." << std::endl;
        } else {
            if (!_nVec) {
#if __cplusplus >= 201403L
                _nVec = make_unique<vector<ErrorNvaElem>>();
#elif __cplusplus < 201403L
                _nVec = unique_ptr<vector<ErrorNvaElem>>(new vector<ErrorNvaElem>);
#endif
            }

            if (rhs._nVec)
                _nVec->assign(rhs._nVec->cbegin(), rhs._nVec->cend());
            else
                std::cerr << "ErrorEvent::operator=(const ErrorEvent&): _nVec is nullptr." << std::endl;
        }
        return *this;
    }
} // asdu