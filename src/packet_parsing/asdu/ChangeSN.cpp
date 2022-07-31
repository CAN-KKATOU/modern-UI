//
// Created by Astria on 2022/4/25.
//

#include "ChangeSN.h"

namespace asdu {
    ChangeSN::ChangeSN(const ASDULengthSet &asdu_length_set)
            : BaseASDU(asdu_length_set) {
        for (int i = 1; i <= asduLength.objectAd + asduLength.sn; i++)
            data->push_back(0);

        tie = 200;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    ChangeSN::ChangeSN(const ASDULengthSet &asdu_length_set, long long int tie, long long int vsq, long long int cot,
                       size_t command_address, size_t object_address, long long int sn)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address, object_address), sn(sn) {
        util::pushElems(data, asduLength.sn, sn);
    }

    ChangeSN::ChangeSN(unique_ptr<std::vector<int>> data, const ASDULengthSet &als)
            : BaseASDU(move(data), als) {
        size_t cIndex{static_cast<size_t>(asduLength.type +
                                          asduLength.qualifier +
                                          asduLength.cot +
                                          asduLength.commonAd +
                                          asduLength.objectAd)};

        try {
            sn = util::vecToLL(data, cIndex, asduLength.sn);
        } catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
        }
        catch (const std::out_of_range &e) {
            std::cerr << e.what() << std::endl;
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

    shared_ptr<json> ChangeSN::toJson() {
        auto object = make_shared<json>();
        (*object)["ASDU报文类型"] = "切换定值区报文";
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
        (*object)["信息体地址"] = oAddr;
        (*object)["定值区号"] = sn;
        return object;
    }

    shared_ptr<json> ChangeSN::getPosition() {
        auto object = make_shared<json>();

        size_t cIndex{static_cast<size_t>(asduLength.startIndex)};
        (*object)["ASDU报文类型"] = util::posJson(cIndex, cIndex + asduLength.type);

        cIndex += asduLength.type + asduLength.qualifier;
        (*object)["传送原因"] = util::posJson(cIndex, cIndex + asduLength.cot);
        (*object)["传送方向"] = util::posJson(cIndex, cIndex + asduLength.cot);

        cIndex += asduLength.cot;
        (*object)["公共地址"] = util::posJson(cIndex, cIndex + asduLength.commonAd);

        cIndex += asduLength.commonAd;
        (*object)["信息体地址"] = util::posJson(cIndex, cIndex + asduLength.objectAd);

        cIndex += asduLength.objectAd;
        (*object)["定值区号"] = util::posJson(cIndex, cIndex + asduLength.sn);

        return object;
    }

    void ChangeSN::set_object_address(long long int object_address) {
        oAddr = object_address;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd)};
        util::replaceElems(data, index, asduLength.objectAd, object_address);
    }

    void ChangeSN::set_sn(long long int sn) {
        sn = sn;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd +
                                         asduLength.objectAd)};
        util::replaceElems(data, index, asduLength.sn, sn);
    }

    ChangeSN::ChangeSN(const ChangeSN &rhs) : BaseASDU(rhs), sn(rhs.sn){

    }

    ChangeSN &ChangeSN::operator=(const ChangeSN &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);
        sn = rhs.sn;
        return *this;
    }
} // asdu