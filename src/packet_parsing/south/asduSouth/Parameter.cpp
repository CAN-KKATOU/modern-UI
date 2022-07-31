//
// Created by Astria on 2022/4/25.
//

#include "Parameter.h"

#include <memory>

namespace ASDUSouth {
    Parameter::Parameter(std::unique_ptr<std::vector<int>> data, const asdu::ASDULengthSet &als)
            : BaseASDU(std::move(data), als) {
        long long flag = (long) (pow(2, asduLength.qualifier * 8 - 1));
        _sq = vsq >= flag;
        _count = vsq % flag;
        _read = tie == 108;

        size_t cIndex{static_cast<size_t >(asduLength.type +
                                           asduLength.qualifier +
                                           asduLength.cot +
                                           asduLength.commonAd)};

        try {
            size_t addFirst = util::vecToULL(data, cIndex, asduLength.objectAd); // 首个元素地址

            int element_length; // 单个元素长度
            if (_sq) {
                cIndex += asduLength.objectAd; // 将标识往后到带品质描述词的单/双点信息位置
                if (_read)
                    element_length = 4;
                else element_length = 4 + asduLength.qualifier;
            } else {
                if (_read)
                    element_length = asduLength.objectAd + 4;
                else element_length = asduLength.objectAd + asduLength.qualifier + 4;
            }

#if __cplusplus >= 201403L
            _eVec = std::make_unique<std::vector<ParaElem>>();
#elif __cplusplus < 201403L
            elemVec = std::unique_ptr<std::vector<ParaElem>>(new std::vector<ParaElem>);
#endif

            for (int index = 0; index < _count; index++) {
                try {
                    ParaElem element;
                    if (_sq) {
                        element.address = addFirst + index;
                        element.value = util::parseIeee574(util::vecMid(*data, cIndex, 4));
                        if (!_read)
                            element.QOS = util::vecToInt(data, cIndex + 4, asduLength.qualifier);
                    } else {
                        element.address = util::vecToULL(data, cIndex, asduLength.objectAd);
                        element.value = util::parseIeee574(
                                util::vecMid(data, cIndex + asduLength.objectAd, 4));
                        if (!_read) {
                            element.QOS = util::vecToInt(data, cIndex + asduLength.objectAd + 4,
                                                         asduLength.qualifier);
                        }
                    }

                    _eVec->push_back(element);
                    cIndex += element_length;
                } catch (const std::invalid_argument &e) {
                    throw e;
                }
                catch (const std::out_of_range &e) {
                    throw e;
                }
            }
        } catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
        }
        catch (const std::out_of_range &e) {
            std::cerr << e.what() << std::endl;
        }

        switch (cot) {
            case 5:
            case 6:
            case 7:
            case 8:
            case 44:
                break;
            default:
                errInfo += "传送原因COT错误 ";
                break;
        }
    }

    shared_ptr<json> Parameter::toJson() {
        auto object = make_shared<json>();
        switch (tie) {
            case 55:
                (*object)["ASDU报文类型"] = "预置/激活参数报文";
                break;
            case 108:
                (*object)["ASDU报文类型"] = "读参数报文";
                break;
        }

        switch (cot) {
            case 6:
                (*object)["传送原因"] = "激活";
                (*object)["报文方向"] = "控制方向";
                break;
            case 8:
                (*object)["传送原因"] = "停止激活";
                (*object)["报文方向"] = "控制方向";
                break;
            case 5:
                (*object)["传送原因"] = "被请求";
                (*object)["报文方向"] = "监视方向";
                break;
            case 7:
                (*object)["传送原因"] = "激活确认";
                (*object)["报文方向"] = "监视方向";
                break;
            case 9:
                (*object)["传送原因"] = "停止激活确认";
                (*object)["报文方向"] = "监视方向";
                break;
            case 44:
                (*object)["传送原因"] = "未知的类型标识";
                (*object)["报文方向"] = "监视方向";
                break;
            default:
                (*object)["传送原因"] = "Data Error";
                break;
        }

        (*object)["公共地址"] = cAddr;

        auto objectList = new nlohmann::json;
        int index{1};
        for (const auto &val: *_eVec) {
            nlohmann::json element;
            element["参数信息对象地址"] = val.address;
            element["参数对象短浮点数值"] = val.value;
            if (!_read)
                element["设定命令限定词"] = util::parseQOS(val.QOS);
            (*objectList)["对象" + std::to_string(index)] = element;
            index++;
        }
        (*object)["参数信息对象列表"] = *objectList;
        delete objectList;
        return object;
    }

    shared_ptr<json> Parameter::getPosition() {
        auto object = make_shared<json>();

        size_t cIndex{static_cast<size_t>(asduLength.startIndex)};
        (*object)["ASDU报文类型"] = util::posJson(cIndex, cIndex + asduLength.type);
        (*object)["标识类型"] = util::posJson(cIndex, cIndex + asduLength.type);

        cIndex += asduLength.type + asduLength.qualifier;
        (*object)["传送原因"] = util::posJson(cIndex, cIndex + asduLength.cot);

        cIndex += asduLength.cot;
        (*object)["公共地址"] = util::posJson(cIndex, cIndex + asduLength.commonAd);

        cIndex += asduLength.commonAd;
        nlohmann::json address_SQ = util::posJson(cIndex, cIndex + asduLength.objectAd);

        if (_sq)
            cIndex += asduLength.objectAd;

        size_t mark1{cIndex};
        auto objectList = new nlohmann::json;
        int index = 1;
        for (const auto &val: *_eVec) {
            size_t mark2{cIndex};
            nlohmann::json element;
            if (!_sq) {
                element["参数信息对象地址"] = util::posJson(cIndex, cIndex + asduLength.objectAd);
                cIndex += asduLength.objectAd;
            } else
                element["参数信息对象地址"] = address_SQ;

            element["参数对象短浮点数值"] = util::posJson(cIndex, cIndex + 4);
            cIndex += 4;

            if (!_read) {
                element["设定命令限定词"] = util::posJson(cIndex, cIndex + asduLength.qualifier);
                cIndex += asduLength.qualifier;
            }
            (*objectList)["对象" + std::to_string(index)] = element;
            (*objectList)["对象" + std::to_string(index) + "位置"] = util::posJson(mark2, cIndex);
            index++;
        }
        (*object)["参数信息对象列表"] = *objectList;
        (*object)["参数信息对象列表位置"] = util::posJson(mark1, cIndex);
        delete objectList;
        return object;
    }

    Parameter::Parameter(const asdu::ASDULengthSet &asdu_length_set)
            : BaseASDU(asdu_length_set) {
#if __cplusplus >= 201403L
        _eVec = make_unique<vector<ParaElem>>();
#elif __cplusplus < 201403L
        elemVec = unique_ptr<vector<ParaElem>>(new vector<ParaElem>());
#endif

        tie = 55;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    Parameter::Parameter(const asdu::ASDULengthSet &asdu_length_set, long long int tie, long long int vsq,
                         long long int cot, size_t command_address)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address) {
#if __cplusplus >= 201403L
        _eVec = make_unique<vector<ParaElem>>();
#elif __cplusplus < 201403L
        elemVec = unique_ptr<vector<ParaElem>>(new vector<ParaElem>());
#endif

        long long flag = (long) (pow(2, asduLength.qualifier * 8 - 1));
        _sq = vsq >= flag;
        _count = vsq % flag;
        _read = tie == 108;

        if (_sq)
            util::pushElems(data, asduLength.objectAd, 0);
    }

    void Parameter::add_elem(const ParaElem &elem) {
        _eVec->push_back(elem);
        if (!_sq)
            util::pushElems(data, asduLength.objectAd, elem.address);

        util::pushIeee574(data, elem.value);

        if (!_read)
            util::pushElems(data, asduLength.qualifier, elem.QOS);
    }

    void Parameter::add_elems(const std::vector<ParaElem> &elems) {
        for (const auto &elem: elems) {
            _eVec->push_back(elem);
            if (!_sq)
                util::pushElems(data, asduLength.objectAd, elem.address);

            util::pushIeee574(data, elem.value);

            if (!_read)
                util::pushElems(data, asduLength.qualifier, elem.QOS);
        }
    }

    void Parameter::add_elems(const std::shared_ptr<std::vector<ParaElem>> &elems) {
        for (const auto &elem: *elems) {
            _eVec->push_back(elem);
            if (!_sq)
                util::pushElems(data, asduLength.objectAd, elem.address);

            util::pushIeee574(data, elem.value);

            if (!_read)
                util::pushElems(data, asduLength.qualifier, elem.QOS);
        }
    }

    void Parameter::set_tie(long long int tie) {
        BaseASDU::set_tie(tie);
        _read = tie == 108;
    }

    void Parameter::set_vsq(long long int vsq) {
        BaseASDU::set_vsq(vsq);
        long long flag = (long) (pow(2, asduLength.qualifier * 8 - 1));
        _sq = vsq >= flag;
        _count = vsq % flag;
    }

    Parameter::Parameter(const Parameter &rhs)
            : asdu::BaseASDU(rhs), _count(rhs._count), _sq(rhs._sq), _read(rhs._read) {
#if __cplusplus >= 201403L
        _eVec = make_unique<vector<ParaElem>>();
#elif __cplusplus < 201403L
        elemVec = unique_ptr<vector<ParaElem>>(new vector<ParaElem>());
#endif
        if (rhs._eVec)
            _eVec->assign(rhs._eVec->cbegin(), rhs._eVec->cend());
        else
            std::cerr << "Parameter(const Parameter&): elemVec is nullptr." << std::endl;
    }

    Parameter &Parameter::operator=(const Parameter &rhs) {
        if (this == &rhs)
            return *this;

        asdu::BaseASDU::operator=(rhs);
        _count = rhs._count;
        _sq = rhs._sq;
        _read = rhs._read;
        if (!_eVec) {
#if __cplusplus >= 201403L
            _eVec = make_unique<vector<ParaElem>>();
#elif __cplusplus < 201403L
            elemVec = unique_ptr<vector<ParaElem>>(new vector<ParaElem>());
#endif
        }
        if (rhs._eVec)
            _eVec->assign(rhs._eVec->cbegin(), rhs._eVec->cend());
        else
            std::cerr << "Parameter::operator=(const Parameter &): elemVec is nullptr." << std::endl;
        return *this;
    }
} // ASDUSouth