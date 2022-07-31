//
// Created by Astria on 2022/4/25.
//

#include "Initialized.h"

namespace asdu {
    Initialized::Initialized(unique_ptr<vector<int>> dataStr, const ASDULengthSet &asduLengthSet)
            : BaseASDU(move(dataStr), asduLengthSet) {
        size_t skipLength{static_cast<size_t>(asduLength.type +
                                              asduLength.qualifier +
                                              asduLength.cot +
                                              asduLength.commonAd +
                                              asduLength.objectAd)};

        try {
            coi = util::vecToLL(dataStr, skipLength, asduLength.qualifier);
        } catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
        }
        catch (const std::out_of_range &e) {
            std::cerr << e.what() << std::endl;
        }

        if (cot != 4)
            errInfo += "传送原因COT错误 ";

        switch (coi) {
            case 0:
            case 1:
            case 2:
                break;
            default:
                errInfo += "初始化原因COI错误 ";
                break;
        }
    }

    shared_ptr<json> Initialized::toJson() {
        auto object = make_shared<json>();
        (*object)["ASDU报文类型"] = "初始化结束命令报文";
        switch (cot) {
            case 4:
                (*object)["传送原因"] = "初始化完成";
                break;
            case 70:
                (*object)["传送原因"] = "初始化结束";
                break;
            default:
                (*object)["传送原因"] = "Data Error";
                break;
        }
        (*object)["公共地址"] = cAddr;
        (*object)["信息对象地址"] = oAddr;

        switch (coi) {
            case 0 :
                (*object)["初始化原因"] = "当地电源合上";
                break;
            case 1 :
                (*object)["初始化原因"] = "当地手动复位";
                break;
            case 2 :
                (*object)["初始化原因"] = "远方复位";
                break;
            default:
                (*object)["初始化原因"] = "Data Error";
                break;
        }
        return object;
    }

    shared_ptr<json> Initialized::getPosition() {
        auto object = make_shared<json>();

        size_t cIndex{static_cast<size_t>(asduLength.startIndex)};
        (*object)["ASDU报文类型"] = util::posJson(cIndex, cIndex + asduLength.type);

        cIndex += asduLength.type + asduLength.qualifier;
        (*object)["传送原因"] = util::posJson(cIndex, cIndex + asduLength.cot);

        cIndex += asduLength.cot;
        (*object)["公共地址"] = util::posJson(cIndex, cIndex + asduLength.commonAd);

        cIndex += asduLength.commonAd;
        (*object)["信息对象地址"] = util::posJson(cIndex, cIndex + asduLength.objectAd);

        cIndex += asduLength.objectAd;
        (*object)["初始化原因"] = util::posJson(cIndex, cIndex + asduLength.qualifier);

        return object;
    }

    Initialized::Initialized(const ASDULengthSet &asduLengthSet) : BaseASDU(asduLengthSet) {
        for (int i = 0; i < asduLength.objectAd + asduLength.qualifier; i++)
            data->push_back(0);

        tie = 70;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    Initialized::Initialized(const ASDULengthSet &asduLengthSet, long long int tie, long long int vsq,
                             long long int cot, size_t commandAddress, size_t objectAddress, long long int coi)
            : BaseASDU(asduLengthSet, tie, vsq, cot, commandAddress, objectAddress), coi(coi) {
        util::pushElems(data, asduLength.qualifier, coi);
    }

    void Initialized::set_object_address(long long int objectAddress) {
        oAddr = objectAddress;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd)};
        util::replaceElems(data, index, asduLength.objectAd, objectAddress);
    }

    Initialized::Initialized(const Initialized &rhs) : BaseASDU(rhs), coi(rhs.coi){

    }

    Initialized &Initialized::operator=(const Initialized &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);
        coi = rhs.coi;
        return *this;
    }
} // asdu