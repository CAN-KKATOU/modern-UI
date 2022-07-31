//
// Created by Astria on 2022/4/25.
//

#include "RemoteInfoSouth.h"

#include <memory>

namespace ASDUSouth {
    RemoteInfoSouth::RemoteInfoSouth(std::unique_ptr<std::vector<int>> data, const asdu::ASDULengthSet &als)
            : BaseASDU(std::move(data), als) {
        long long flag = (long) (pow(2, asduLength.qualifier * 8 - 1));
        _sq = vsq >= flag;
        _count = vsq % flag;
        _single = tie == 1 or tie == 30;
        _time = tie == 30 or tie == 31;

        size_t cIndex{static_cast<size_t>(asduLength.type +
                                          asduLength.qualifier +
                                          asduLength.cot +
                                          asduLength.commonAd)};

        try {
            size_t addFirst{util::vecToULL(data, cIndex, asduLength.objectAd)}; // 首个元素地址

            int elementLength; // 单个元素长度
            if (_sq) {
                cIndex += asduLength.objectAd; // 将标识往后到带品质描述词的单/双点信息位置
                if (_time)
                    elementLength = 7 + asduLength.describer;
                else elementLength = asduLength.describer;
            } else {
                if (_time)
                    elementLength = asduLength.objectAd + asduLength.describer + 7;
                else elementLength = asduLength.objectAd + asduLength.describer;
            }

#if __cplusplus >= 201403L
            _eVec = std::make_unique<std::vector<InfoElem>>();
#elif __cplusplus < 201403L
            elemVec = std::unique_ptr<std::vector<InfoElem>>(new std::vector<InfoElem>);
#endif

            for (int index = 0; index < _count; index++) {
                try {
                    InfoElem element;

                    if (_sq) {
                        element.address = addFirst + index;

                        element.quality = util::vecToLL(data, cIndex, asduLength.describer);
                        if (_time)
                            element.time = util::parseTime(
                                    util::vecMid(data, cIndex + asduLength.describer, 7));
                    } else {
                        std::string elementAddStr;

                        element.address = util::vecToULL(data, cIndex, asduLength.objectAd); // 元素地址
                        element.quality = util::vecToLL(data, cIndex + asduLength.objectAd,
                                                        asduLength.describer);
                        if (_time)
                            element.time = util::parseTime(
                                    util::vecMid(data,
                                                 cIndex + asduLength.describer + asduLength.objectAd, 7));
                    }

                    _eVec->push_back(element);
                    cIndex += elementLength;
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

        if (_time) {
            switch (cot) {
                case 3:
                case 5:
                    break;
                default:
                    errInfo.append("传送原因COT错误 ");
                    break;
            }
        } else {
            switch (cot) {
                case 2:
                case 3:
                case 5:
                case 20:
                    break;
                default:
                    errInfo.append("传送原因COT错误 ");
                    break;
            }
        }
    }

    std::shared_ptr<nlohmann::json> RemoteInfoSouth::toJson() {
        auto object = std::make_shared<nlohmann::json>();
        (*object)["ASDU报文类型"] = "遥信命令报文";
        switch (tie) {
            case 1:
                (*object)["标识类型"] = "单点信息";
                break;
            case 3:
                (*object)["标识类型"] = "双点信息";
                break;
            case 30:
                (*object)["标识类型"] = "带CP56Time2a时标的单点信息";
                break;
            case 31:
                (*object)["标识类型"] = "带CP56Time2a时标的双点信息";
                break;
        }

        if (_time) {
            switch (cot) {
                case 3:
                    (*object)["传送原因"] = "突发（自发）";
                    break;
                case 5:
                    (*object)["传送原因"] = "被请求";
                    break;
                default:
                    (*object)["传送原因"] = "Data Error";
                    break;
            }
        } else {
            switch (cot) {
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
        }

        (*object)["公共地址"] = cAddr;

        auto objectList = new nlohmann::json;
        int index = 1;
        for (const auto &val: *_eVec) {
            nlohmann::json element;
            element["遥信信息对象地址"] = val.address;
            if (_single)
                element["带品质描述词的单点信息"] = val.quality;
            else element["带品质描述词的双点信息"] = val.quality;
            if (_time)
                element["时标"] = val.time;
            (*objectList)["对象" + std::to_string(index)] = element;
            index++;
        }
        (*object)["遥信对象列表"] = *objectList;
        delete objectList;
        return object;
    }

    std::shared_ptr<nlohmann::json> RemoteInfoSouth::getPosition() {
        auto object = std::make_shared<nlohmann::json>();

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
        auto objectList = new nlohmann::json;
        int index{1};
        for (const auto &val: *_eVec) {
            size_t mark2{cIndex};
            nlohmann::json element;
            if (!_sq) {
                element["遥信信息对象地址"] = util::posJson(cIndex, cIndex + asduLength.objectAd);
                cIndex += asduLength.objectAd;
            } else
                element["遥信信息对象地址"] = add_SQ;

            if (_single)
                element["带品质描述词的单点信息"] = util::posJson(cIndex, cIndex + asduLength.describer);
            else element["带品质描述词的双点信息"] = util::posJson(cIndex, cIndex + asduLength.describer);
            cIndex += asduLength.describer;

            if (_time) {
                element["时标"] = util::posJson(cIndex, cIndex + 7);
                cIndex += 7;
            }
            (*objectList)["对象" + std::to_string(index)] = element;
            (*objectList)["对象" + std::to_string(index) + "位置"] = util::posJson(mark2, cIndex);
            index++;
        }
        (*object)["遥信对象列表"] = *objectList;
        (*object)["遥信对象列表位置"] = util::posJson(mark1, cIndex);
        delete objectList;
        return object;
    }

    RemoteInfoSouth::RemoteInfoSouth(const asdu::ASDULengthSet &asdu_length_set)
            : BaseASDU(asdu_length_set) {
#if __cplusplus >= 201403L
        _eVec = make_unique<vector<InfoElem>>();
#elif __cplusplus < 201403L
        elemVec = unique_ptr<vector<InfoElem>>(new vector<InfoElem>());
#endif

        tie = 3;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    RemoteInfoSouth::RemoteInfoSouth(const asdu::ASDULengthSet &asdu_length_set, long long int tie, long long int vsq,
                                     long long int cot, size_t command_address)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address) {
#if __cplusplus >= 201403L
        _eVec = make_unique<vector<InfoElem>>();
#elif __cplusplus < 201403L
        elemVec = unique_ptr<vector<InfoElem>>(new vector<InfoElem>());
#endif

        long long flag = (long) (pow(2, asduLength.qualifier * 8 - 1));
        _sq = vsq >= flag;
        _count = vsq % flag;
        _single = tie == 1 or tie == 30;
        _time = tie == 30 or tie == 31;

        if (_sq)
            util::pushElems(data, asduLength.objectAd, 0);
    }

    void RemoteInfoSouth::add_elem(const InfoElem &elem) {
        _eVec->push_back(elem);

        if (!_sq)
            util::pushElems(data, asduLength.objectAd, elem.address);

        util::pushElems(data, asduLength.describer, elem.quality);

        if (_time)
            util::pushTime(data, elem.time);
    }

    void RemoteInfoSouth::add_elems(const shared_ptr<vector<InfoElem>> &elems) {
        for (const auto &elem: *elems) {
            _eVec->push_back(elem);

            if (!_sq)
                util::pushElems(data, asduLength.objectAd, elem.address);

            util::pushElems(data, asduLength.describer, elem.quality);

            if (_time)
                util::pushTime(data, elem.time);
        }
    }

    void RemoteInfoSouth::add_elems(const vector<InfoElem> &elems) {
        for (const auto &elem: elems) {
            _eVec->push_back(elem);

            if (!_sq)
                util::pushElems(data, asduLength.objectAd, elem.address);

            util::pushElems(data, asduLength.describer, elem.quality);

            if (_time)
                util::pushTime(data, elem.time);
        }
    }

    void RemoteInfoSouth::set_tie(long long tie) {
        BaseASDU::set_tie(tie);
        _single = tie == 1 or tie == 30;
        _time = tie == 30 or tie == 31;
    }

    void RemoteInfoSouth::set_vsq(long long int vsq) {
        BaseASDU::set_vsq(vsq);
        long long flag = (long) (pow(2, asduLength.qualifier * 8 - 1));
        _sq = vsq >= flag;
        _count = vsq % flag;

        if (_sq)
            util::pushElems(data, asduLength.objectAd, 0);
    }

    bool RemoteInfoSouth::set_sq_addr(size_t addr) {
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

    RemoteInfoSouth::RemoteInfoSouth(const RemoteInfoSouth &rhs)
            : asdu::BaseASDU(rhs), _count(rhs._count), _sq(rhs._sq), _single(rhs._single), _time(rhs._time) {
#if __cplusplus >= 201403L
        _eVec = make_unique<vector<InfoElem>>();
#elif __cplusplus < 201403L
        elemVec = unique_ptr<vector<InfoElem>>(new vector<InfoElem>());
#endif
        if (rhs._eVec)
            _eVec->assign(rhs._eVec->cbegin(), rhs._eVec->cend());
        else
            std::cerr << "RemoteInfoSouth(const RemoteInfoSouth&): elemVec is nullptr." << std::endl;
    }

    RemoteInfoSouth &RemoteInfoSouth::operator=(const RemoteInfoSouth &rhs) {
        if (this == &rhs)
            return *this;

        asdu::BaseASDU::operator=(rhs);
        _count = rhs._count;
        _sq = rhs._sq;
        _single = rhs._single;
        _time = rhs._time;
        if (!_eVec) {
#if __cplusplus >= 201403L
            _eVec = make_unique<vector<InfoElem>>();
#elif __cplusplus < 201403L
            elemVec = unique_ptr<vector<InfoElem>>(new vector<InfoElem>());
#endif
        }
        if (rhs._eVec)
            _eVec->assign(rhs._eVec->cbegin(), rhs._eVec->cend());
        else
            std::cerr << "RemoteInfoSouth::operator=(const RemoteInfoSouth &): elemVec is nullptr." << std::endl;
        return *this;
    }
} // ASDUSouth
