//
// Created by Astria on 2022/4/25.
//

#include "EnergyData.h"

namespace asdu {
    EnergyData::EnergyData(const ASDULengthSet &asdu_length_set) : BaseASDU(asdu_length_set) {
#if __cplusplus >= 201403L
        _eVec = make_unique<vector<EnergyElem>>();
#elif __cplusplus < 201403L
        elemVec = unique_ptr<vector<EnergyElem>>(new vector<EnergyElem>());
#endif
        tie = 206;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    EnergyData::EnergyData(const ASDULengthSet &asdu_length_set, long long int tie, long long int vsq,
                           long long int cot, size_t command_address)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address) {
#if __cplusplus >= 201403L
        _eVec = make_unique<vector<EnergyElem>>();
#elif __cplusplus < 201403L
        elemVec = unique_ptr<vector<EnergyElem>>(new vector<EnergyElem>());
#endif

        long long flag = (long) (pow(2, asduLength.qualifier * 8 - 1));
        _sq = vsq >= flag;
        _count = vsq % flag;
        _time = tie == 207;

        if (_sq)
            util::pushElems(data, asduLength.objectAd, 0);
    }

    EnergyData::EnergyData(unique_ptr<vector<int>> data, const ASDULengthSet &als)
            : BaseASDU(move(data), als) {
        long long flag = (long) (pow(2, asduLength.qualifier * 8 - 1));
        _sq = vsq >= flag;
        _count = vsq % flag;
        _time = tie == 207;

        size_t cIndex{static_cast<size_t>(asduLength.type +
                                          asduLength.qualifier +
                                          asduLength.cot +
                                          asduLength.commonAd)};

        try {
            size_t addFirst = util::vecToULL(data, cIndex, asduLength.objectAd);

            int element_length;
            if (_sq) {
                cIndex += asduLength.objectAd;
                if (_time)
                    element_length = 11 + asduLength.describer; // 11为短浮点数4位加时标7位
                else
                    element_length = 4 + asduLength.describer;
            } else {
                if (_time)
                    element_length = asduLength.objectAd + 11 + asduLength.describer;
                else
                    element_length = asduLength.objectAd + 4 + asduLength.describer;
            }

#if __cplusplus >= 201403L
            _eVec = make_unique<vector<EnergyElem>>();
#elif __cplusplus < 201403L
            elemVec = unique_ptr<vector<EnergyElem>>(new vector<EnergyElem>);
#endif
            for (int index = 0; index < _count; index++) {
                try {
                    EnergyElem element;
                    if (_sq) {
                        element.address = addFirst + index;
                        element.value = util::parseIeee574(
                                util::vecMid(data, cIndex, cIndex + 4));

                        element.qds = util::vecToLL(data, cIndex + 4, asduLength.describer);
                        if (_time)
                            element.time = util::parseTime(
                                    util::vecMid(data, cIndex + 4 + asduLength.describer, 7));
                    } else {
                        element.address = util::vecToLL(data, cIndex, asduLength.objectAd);
                        element.value = util::parseIeee574(
                                util::vecMid(data, cIndex + asduLength.objectAd, cIndex + 4));
                        element.qds = util::vecToLL(data, cIndex + 4 + asduLength.objectAd,
                                                    asduLength.describer);
                        if (_time)
                            element.time = util::parseTime(
                                    util::vecMid(data,
                                                 cIndex + 4 + asduLength.objectAd + asduLength.describer, 7));
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
            case 3:
            case 7:
                break;
            default:
                errInfo += "传送原因COT错误 ";
                break;
        }
    }

    shared_ptr<json> EnergyData::toJson() {
        auto object = make_shared<json>();
        (*object)["ASDU报文类型"] = "电能量数据报文";
        switch (cot) {
            case 3:
                (*object)["传送原因"] = "突发（自发）";
                break;
            case 37:
                (*object)["传送原因"] = "响应电能量总召唤";
                break;
            default:
                (*object)["传送原因"] = "Data Error";
                break;
        }

        (*object)["公共地址"] = cAddr;

        auto objectList = new json;
        int index{1};
        for (const auto &val: *_eVec) {
            json element;
            element["电能量对象地址"] = val.address;
            element["电能量数值（短浮点数）"] = val.value;
            element["品质描述词"] = val.qds;
            (*objectList)["对象" + std::to_string(index)] = element;
            index++;
        }
        (*object)["电能量对象列表"] = *objectList;
        delete objectList;
        return object;
    }

    shared_ptr<json> EnergyData::getPosition() {
        auto object = make_shared<json>();

        size_t cIndex{static_cast<size_t>(asduLength.startIndex)};
        (*object)["ASDU报文类型"] = util::posJson(cIndex, cIndex + asduLength.type);
        (*object)["标识类型"] = util::posJson(cIndex, cIndex + asduLength.type);

        cIndex += asduLength.type + asduLength.qualifier;
        (*object)["传送原因"] = util::posJson(cIndex, cIndex + asduLength.cot);

        cIndex += asduLength.cot;
        (*object)["公共地址"] = util::posJson(cIndex, cIndex + asduLength.commonAd);

        cIndex += asduLength.commonAd;

        nlohmann::json add_SQ = util::posJson(cIndex, cIndex + asduLength.objectAd);

        if (_sq)
            cIndex += asduLength.objectAd;

        size_t mark1{cIndex};
        auto objectList = new json;
        int index{1};
        for (const auto &val: *_eVec) {
            size_t mark2{cIndex};
            json element;
            if (!_sq) {
                element["电能量对象地址"] = util::posJson(cIndex, cIndex + asduLength.objectAd);
                cIndex += asduLength.objectAd;
            } else
                element["电能量对象地址"] = add_SQ;

            element["电能量数值（短浮点数）"] = util::posJson(cIndex, cIndex + 4);
            cIndex += 4;

            element["品质描述词"] = util::posJson(cIndex, cIndex + asduLength.describer);
            cIndex += asduLength.describer;

            (*objectList)["对象" + std::to_string(index)] = element;
            (*objectList)["对象" + std::to_string(index) + "位置"] = util::posJson(mark2, cIndex);
            index++;
        }
        (*object)["电能量对象列表"] = *objectList;
        (*object)["电能量对象列表位置"] = util::posJson(mark1, cIndex);
        delete objectList;
        return object;
    }

    void EnergyData::add_elem(const EnergyElem &elem) {
        _eVec->push_back(elem);

        if (_sq)
            util::pushElems(data, asduLength.objectAd, elem.address);

        util::pushIeee574(data, elem.value);
        util::pushElems(data, asduLength.describer, elem.qds);

        if (_time)
            util::pushTime(data, elem.time);
    }

    void EnergyData::add_elems(const vector<EnergyElem> &elems) {
        for (const auto &elem: elems) {
            _eVec->push_back(elem);

            if (_sq)
                util::pushElems(data, asduLength.objectAd, elem.address);

            util::pushIeee574(data, elem.value);
            util::pushElems(data, asduLength.describer, elem.qds);

            if (_time)
                util::pushTime(data, elem.time);
        }
    }

    void EnergyData::add_elems(const shared_ptr<vector<EnergyElem>> &elems) {
        for (const auto &elem: *elems) {
            _eVec->push_back(elem);

            if (_sq)
                util::pushElems(data, asduLength.objectAd, elem.address);

            util::pushIeee574(data, elem.value);
            util::pushElems(data, asduLength.describer, elem.qds);

            if (_time)
                util::pushTime(data, elem.time);
        }
    }

    void EnergyData::set_tie(long long int tie) {
        BaseASDU::set_tie(tie);
        _time = tie == 207;
    }

    void EnergyData::set_vsq(long long int vsq) {
        BaseASDU::set_vsq(vsq);
        long long flag = (long) (pow(2, asduLength.qualifier * 8 - 1));
        _sq = vsq >= flag;
        _count = vsq % flag;
    }

    bool EnergyData::set_sq_addr(size_t addr) {
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

    EnergyData::EnergyData(const EnergyData &rhs)
            : BaseASDU(rhs), _count(rhs._count), _sq(rhs._sq), _time(rhs._time) {
#if __cplusplus >= 201403L
        _eVec = make_unique<vector<EnergyElem>>();
#elif __cplusplus < 201403L
        elemVec = unique_ptr<vector<EnergyElem>>(new vector<EnergyElem>());
#endif

        if (rhs._eVec)
            _eVec->assign(rhs._eVec->cbegin(), rhs._eVec->cend());
        else
            std::cerr << "EnergyData(const EnergyData&): elemVec is nullptr." << std::endl;
    }

    EnergyData &EnergyData::operator=(const EnergyData &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);
        if (!_eVec) {
#if __cplusplus >= 201403L
            _eVec = make_unique<vector<EnergyElem>>();
#elif __cplusplus < 201403L
            elemVec = unique_ptr<vector<EnergyElem>>(new vector<EnergyElem>());
#endif
        }

        if (rhs._eVec)
            _eVec->assign(rhs._eVec->cbegin(), rhs._eVec->cend());
        else
            std::cerr << "EnergyData::operator=(const EnergyData&): elemVec is nullptr." << std::endl;

        return *this;
    }
} // asdu