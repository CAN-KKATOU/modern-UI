//
// Created by Astria on 2022/4/25.
//

#include "WriteParameter.h"

#include <memory>

namespace asdu {
    WriteParameter::WriteParameter(unique_ptr<vector<int>> data, const ASDULengthSet &als)
            : BaseASDU(move(data), als) {
        _dir = cot == 6 or cot == 8;
        _preset = vsq != 0;

        size_t cIndex{static_cast<size_t>(asduLength.type +
                                          asduLength.qualifier +
                                          asduLength.cot +
                                          asduLength.commonAd)};

        try {
            _sn = util::vecToULL(data, cIndex, asduLength.sn);
            cIndex += asduLength.sn;
            _features = data->at(cIndex++);

            if (_preset) {
#if __cplusplus >= 201403L
                _oVec = std::make_unique<std::vector<WriteElem>>();
#elif __cplusplus < 201403L
                _oVec = unique_ptr<vector<WriteElem>>(new vector<WriteElem>);
#endif

                _count = vsq % (long) (pow(2, asduLength.qualifier * 8 - 1));
                for (int index = 0; index < _count; index++) {
                    try {
                        WriteElem element;

                        element.address = util::vecToULL(data, cIndex, asduLength.objectAd);
                        element.tag = data->at(cIndex + asduLength.objectAd);
                        element.value_length = data->at(cIndex + asduLength.objectAd + 1);
                        element.value = util::vecToHexStr(data, cIndex + asduLength.objectAd + 2,
                                                          element.value_length);
                        _oVec->push_back(element);
                        cIndex += asduLength.objectAd + 2 + element.value_length;
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
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 44:
            case 45:
            case 46:
            case 47:
                break;
            default:
                errInfo += "传送原因COT错误 ";
                break;
        }
    }

    shared_ptr<json> WriteParameter::toJson() {
        auto object = make_shared<json>();
        (*object)["ASDU报文类型"] = "写参数和定值报文";
        switch (cot) {
            case 6:
                (*object)["传送原因"] = "激活";
                (*object)["报文方向"] = "控制方向";
                break;
            case 8:
                (*object)["传送原因"] = "停止激活";
                (*object)["报文方向"] = "控制方向";
                break;
            case 7:
                (*object)["传送原因"] = "激活确认";
                (*object)["报文方向"] = "监视方向";
                break;
            case 9:
                (*object)["传送原因"] = "停止激活确认";
                (*object)["报文方向"] = "监视方向";
                break;
            case 10:
                (*object)["传送原因"] = "激活终止";
                (*object)["报文方向"] = "监视方向";
                break;
            case 44:
                (*object)["传送原因"] = "未知的类型标识";
                (*object)["报文方向"] = "监视方向";
                break;
            case 45:
                (*object)["传送原因"] = "未知的传动原因";
                (*object)["报文方向"] = "监视方向";
                break;
            case 46:
                (*object)["传送原因"] = "未知的应用服务数据单元公共地址";
                (*object)["报文方向"] = "监视方向";
                break;
            case 47:
                (*object)["传送原因"] = "未知的信息对象地址";
                (*object)["报文方向"] = "监视方向";
                break;
            default:
                (*object)["传送原因"] = "Data Error";
                break;
        }

        (*object)["定值区号"] = _sn;
        (*object)["参数特征标识"] = util::parseFeature(_features);

        if (_preset) {
            (*object)["写入操作"] = "预置";
            auto list1 = new json;
            int index = 1;
            for (const auto &val: *_oVec) {
                json element;
                element["信息体地址"] = val.address;
                element["Tag类型"] = val.tag;
                element["值"] = val.value;
                (*list1)["信息体对象" + std::to_string(index)] = element;
                index++;
            }
            (*object)["信息体对象列表"] = *list1;
            delete list1;
        } else {
            (*object)["写入操作"] = "固化/撤销";
        }
        return object;
    }

    shared_ptr<json> WriteParameter::getPosition() {
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
        (*object)["参数特征标识"] = util::posJson(cIndex, cIndex + 1);
        cIndex++;

        if (_preset) {
            (*object)["写入操作"] = util::posJson(vsqIndex, vsqIndex + asduLength.qualifier);
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

                element["值"] = util::posJson(cIndex, val.value_length);
                cIndex += val.value_length;

                (*list1)["信息体对象" + std::to_string(index1)] = element;
                (*list1)["信息体对象" + std::to_string(index1) + "位置"] = util::posJson(mark2, cIndex);
                index1++;
            }
            (*object)["信息体对象列表"] = *list1;
            (*object)["信息体对象列表位置"] = util::posJson(mark1, cIndex);
            delete list1;
        } else {
            (*object)["写入操作"] = util::posJson(vsqIndex, vsqIndex + asduLength.qualifier);
        }
        return object;
    }

    WriteParameter::WriteParameter(const ASDULengthSet &asdu_length_set)
            : BaseASDU(asdu_length_set) {
        for (int i{0}; i < asduLength.sn + 1; i++)
            data->push_back(0);
#if __cplusplus >= 201403L
        _oVec = make_unique<vector<WriteElem>>();
#elif __cplusplus < 201403L
        _oVec = unique_ptr<vector<WriteElem>>(new vector<WriteElem>);
#endif
    }

    WriteParameter::WriteParameter(const ASDULengthSet &asdu_length_set, long long int tie, long long int vsq,
                                   long long int cot, size_t command_address, long long int sn, int features)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address), _sn(sn), _features(features) {
        util::pushElems(data, asduLength.sn, _sn);
        data->push_back(_features);

        _dir = cot == 6 or cot == 8;
        _preset = vsq != 0;
        if (_preset) {
            _count = vsq % (long) (pow(2, asduLength.qualifier * 8 - 1));
#if __cplusplus >= 201403L
            _oVec = make_unique<vector<WriteElem>>();
#elif __cplusplus < 201403L
            _oVec = unique_ptr<vector<WriteElem>>(new vector<WriteElem>);
#endif
        }
    }

    void WriteParameter::set_sn(long long int sn) {
        _sn = sn;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd)};
        util::replaceElems(data, index, asduLength.sn, _sn);
    }

    void WriteParameter::set_features(int features) {
        _features = features;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd +
                                         asduLength.sn)};
        data->at(index) = _features;
    }

    bool WriteParameter::add_elem(const WriteElem &elem) {
        if (_preset) {
            _oVec->push_back(elem);

            util::pushElems(data, asduLength.objectAd, elem.address);
            data->push_back(elem.tag);
            data->push_back(elem.value_length);
            util::pushString(data, elem.value);

            return true;
        } else
            return false;
    }

    bool WriteParameter::add_elems(const vector<WriteElem> &elems) {
        if (_preset) {
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

    bool WriteParameter::add_elems(const shared_ptr<vector<WriteElem>> &elems) {
        if (_preset) {
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

    void WriteParameter::set_vsq(long long int vsq) {
        BaseASDU::set_vsq(vsq);
        _preset = vsq != 0;
    }

    WriteParameter::WriteParameter(const WriteParameter &rhs)
            : BaseASDU(rhs), _sn(rhs._sn), _dir(rhs._dir), _count(rhs._count), _features(rhs._features),
              _preset(rhs._features) {
#if __cplusplus >= 201403L
        _oVec = make_unique<vector<WriteElem>>();
#elif __cplusplus < 201403L
        _oVec = unique_ptr<vector<WriteElem>>(new vector<WriteElem>);
#endif
        if (rhs._oVec)
            _oVec->assign(rhs._oVec->cbegin(), rhs._oVec->cend());
        else
            std::cerr << "WriteParameter(const WriteParameter&): _oVec is nullptr." << std::endl;
    }

    WriteParameter &WriteParameter::operator=(const WriteParameter &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);
        _sn = rhs._sn;
        _dir = rhs._dir;
        _count = rhs._count;
        _features = rhs._features;
        _preset = rhs._preset;

        if (!_oVec) {
#if __cplusplus >= 201403L
            _oVec = make_unique<vector<WriteElem>>();
#elif __cplusplus < 201403L
            _oVec = unique_ptr<vector<WriteElem>>(new vector<WriteElem>);
#endif
        }
        if (rhs._oVec)
            _oVec->assign(rhs._oVec->cbegin(), rhs._oVec->cend());
        else
            std::cerr << "WriteParameter(const WriteParameter&): _oVec is nullptr." << std::endl;
        return *this;
    }
} // asdu
