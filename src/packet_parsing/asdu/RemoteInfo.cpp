//
// Created by Astria on 2022/4/25.
//

#include "RemoteInfo.h"

#include <memory>

namespace asdu {
    RemoteInfo::RemoteInfo(unique_ptr<vector<int>> dataVec, const ASDULengthSet &asduLengthSet)
            : BaseASDU(move(dataVec), asduLengthSet) {
        long long flag = (long) (pow(2, asduLength.qualifier * 8 - 1));
        bSQ = vsq >= flag;
        count = vsq % flag;
        bSingle = tie == 1 or tie == 30;
        bTime = tie == 30 or tie == 31;

        size_t cIndex{static_cast<size_t>(asduLength.type +
                                          asduLength.qualifier +
                                          asduLength.cot +
                                          asduLength.commonAd)};

        try {
            size_t addFirst{util::vecToULL(dataVec, cIndex, asduLength.objectAd)}; // 首个元素地址

            int elementLength; // 单个元素长度
            if (bSQ) {
                cIndex += asduLength.objectAd; // 将标识往后到带品质描述词的单/双点信息位置
                if (bTime)
                    elementLength = 7 + asduLength.describer;
                else elementLength = asduLength.describer;
            } else {
                if (bTime)
                    elementLength = asduLength.objectAd + asduLength.describer + 7;
                else elementLength = asduLength.objectAd + asduLength.describer;
            }

#if __cplusplus >= 201403L
            elemVec = std::make_unique<std::vector<InfoElem>>();
#elif __cplusplus < 201403L
            elemVec = unique_ptr<vector<InfoElem>>(new vector<InfoElem>());
#endif

            for (int index = 0; index < count; index++) {
                try {
                    InfoElem element;

                    if (bSQ) {
                        element.address = addFirst + index;

                        element.quality = util::vecToLL(dataVec, cIndex, asduLength.describer);
                        if (bTime)
                            element.time = util::parseTime(
                                    util::vecMid(dataVec, cIndex + asduLength.describer, 7));
                    } else {
                        element.address = util::vecToULL(dataVec, cIndex, asduLength.objectAd);
                        element.quality = util::vecToLL(dataVec, cIndex + asduLength.objectAd,
                                                        asduLength.describer);

                        if (bTime)
                            element.time = util::parseTime(util::vecMid(dataVec,
                                                                        cIndex + asduLength.describer +
                                                                        asduLength.objectAd, 7));
                    }

                    elemVec->push_back(element);
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

        switch (cot) {
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

    shared_ptr<json> RemoteInfo::toJson() {
        auto object = make_shared<json>();
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

        (*object)["公共地址"] = cAddr;

        auto objectList = new json;
        int index{1};
        for (const auto &val: *elemVec) {
            json element;
            element["遥信信息对象地址"] = val.address;
            if (bSingle)
                element["带品质描述词的单点信息"] = val.quality;
            else element["带品质描述词的双点信息"] = val.quality;
            if (bTime)
                element["时标"] = val.time;
            (*objectList)["对象" + std::to_string(index)] = element;
            index++;
        }
        (*object)["遥信对象列表"] = *objectList;
        delete objectList;
        return object;
    }

    shared_ptr<json> RemoteInfo::getPosition() {
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

        if (bSQ)
            cIndex += asduLength.objectAd;

        size_t mark1{cIndex};
        auto objectList = new json;
        int index{1};
        for (const auto &val: *elemVec) {
            size_t mark2{cIndex};
            json element;
            if (!bSQ) {
                element["遥信信息对象地址"] = util::posJson(cIndex, cIndex + asduLength.objectAd);
                cIndex += asduLength.objectAd;
            } else
                element["遥信信息对象地址"] = addSQ;

            if (bSingle)
                element["带品质描述词的单点信息"] = util::posJson(cIndex, cIndex + asduLength.describer);
            else element["带品质描述词的双点信息"] = util::posJson(cIndex, cIndex + asduLength.describer);
            cIndex += asduLength.describer;

            if (bTime) {
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

    void RemoteInfo::add_elems(const vector<InfoElem> &elems) {
        for (const auto &elem: elems) {
            elemVec->push_back(elem);

            if (!bSQ)
                util::pushElems(data, asduLength.objectAd, elem.address);

            util::pushElems(data, asduLength.describer, elem.quality);

            if (bTime)
                util::pushTime(data, elem.time);
        }
    }

    void RemoteInfo::add_elems(const shared_ptr<vector<InfoElem>> &elems) {
        for (const auto &elem: *elems) {
            elemVec->push_back(elem);

            if (!bSQ)
                util::pushElems(data, asduLength.objectAd, elem.address);

            util::pushElems(data, asduLength.describer, elem.quality);

            if (bTime)
                util::pushTime(data, elem.time);
        }
    }

    void RemoteInfo::add_elem(const InfoElem &elem) {
        elemVec->push_back(elem);

        if (!bSQ)
            util::pushElems(data, asduLength.objectAd, elem.address);

        util::pushElems(data, asduLength.describer, elem.quality);

        if (bTime)
            util::pushTime(data, elem.time);
    }

    RemoteInfo::RemoteInfo(const ASDULengthSet &asduLengthSet, long long tie, long long vsq,
                           long long cot, size_t commandAddress)
            : BaseASDU(asduLengthSet, tie, vsq, cot, commandAddress) {
#if __cplusplus >= 201403L
        elemVec = make_unique<vector<InfoElem>>();
#elif __cplusplus < 201403L
        elemVec = unique_ptr<vector<InfoElem>>(new vector<InfoElem>());
#endif

        long long flag = (long) (pow(2, asduLength.qualifier * 8 - 1));
        bSQ = vsq >= flag;
        count = vsq % flag;
        bSingle = tie == 1 or tie == 30;
        bTime = tie == 30 or tie == 31;

        if (bSQ)
            util::pushElems(data, asduLength.objectAd, 0);
    }

    RemoteInfo::RemoteInfo(const ASDULengthSet &asduLengthSet) : BaseASDU(asduLengthSet) {
#if __cplusplus >= 201403L
        elemVec = make_unique<vector<InfoElem>>();
#elif __cplusplus < 201403L
        elemVec = unique_ptr<vector<InfoElem>>(new vector<InfoElem>());
#endif

        tie = 3;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    void RemoteInfo::set_tie(long long int tie) {
        BaseASDU::set_tie(tie);
        bSingle = tie == 1 or tie == 30;
        bTime = tie == 30 or tie == 31;
    }

    void RemoteInfo::set_vsq(long long int vsq) {
        BaseASDU::set_vsq(vsq);
        long long flag = (long) (pow(2, asduLength.qualifier * 8 - 1));
        bSQ = vsq >= flag;
        count = vsq % flag;

        if (bSQ)
            util::pushElems(data, asduLength.objectAd, 0);
    }

    bool RemoteInfo::set_sq_addr(size_t addr) {
        if (bSQ) {
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

    RemoteInfo::RemoteInfo(const RemoteInfo &rhs)
            : BaseASDU(rhs), count(rhs.count), bSQ(rhs.bSQ), bSingle(rhs.bSingle), bTime(rhs.bTime) {
#if __cplusplus >= 201403L
        elemVec = make_unique<vector<InfoElem>>();
#elif __cplusplus < 201403L
        elemVec = unique_ptr<vector<InfoElem>>(new vector<InfoElem>());
#endif
        if (rhs.elemVec)
            elemVec->assign(rhs.elemVec->cbegin(), rhs.elemVec->cend());
        else
            std::cerr << "RemoteInfo(const RemoteInfo&): elemVec is nullptr." << std::endl;
    }

    RemoteInfo &RemoteInfo::operator=(const RemoteInfo &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);
        count = rhs.count;
        bSQ = rhs.bSQ;
        bSingle = rhs.bSingle;
        bTime = rhs.bTime;
        if (!elemVec) {
#if __cplusplus >= 201403L
            elemVec = make_unique<vector<InfoElem>>();
#elif __cplusplus < 201403L
            elemVec = unique_ptr<vector<InfoElem>>(new vector<InfoElem>());
#endif
        }
        if (rhs.elemVec)
            elemVec->assign(rhs.elemVec->cbegin(), rhs.elemVec->cend());
        else
            std::cerr << "RemoteInfo(const RemoteInfo&): elemVec is nullptr." << std::endl;
        return *this;
    }
}
// asdu
