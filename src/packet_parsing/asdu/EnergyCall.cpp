//
// Created by Astria on 2022/4/25.
//

#include "EnergyCall.h"

namespace asdu {
    EnergyCall::EnergyCall(unique_ptr<vector<int>> data, const ASDULengthSet &als)
            : BaseASDU(move(data), als) {
        size_t cIndex{static_cast<size_t>(asduLength.type +
                                          asduLength.qualifier +
                                          asduLength.cot +
                                          asduLength.commonAd +
                                          asduLength.objectAd)};
        try {
            _qcc = util::vecToLL(data, cIndex, asduLength.qualifier);

            if (_qcc != 5)
                errInfo.append("召唤限定词QOI错误 ");
        } catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
        }
        catch (const std::out_of_range &e) {
            std::cerr << e.what() << std::endl;
        }

        switch (cot) {
            case 6:
            case 7:
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
        if (_qcc != 5)
            errInfo.append("召唤限定词QOI错误 ");
    }

    shared_ptr<json> EnergyCall::toJson() {
        auto object = make_shared<json>();

        (*object)["ASDU报文类型"] = "电能量召唤报文";
        switch (cot) {
            case 6:
                (*object)["传送原因"] = "激活";
                break;
            case 7:
                (*object)["传送原因"] = "激活确认";
                break;
            case 10:
                (*object)["传送原因"] = "激活终止";
                break;
            case 44:
                (*object)["传送原因"] = "未知的类型标识";
                break;
            case 45:
                (*object)["传送原因"] = "未知的传送原因";
                break;
            case 46:
                (*object)["传送原因"] = "未知的应用服务数据单元公共地址";
                break;
            case 47:
                (*object)["传送原因"] = "未知的信息对象地址";
                break;
            default:
                (*object)["传送原因"] = "Data Error";
                break;
        }
        (*object)["公共地址"] = cAddr;
        (*object)["信息对象地址"] = oAddr;
        (*object)["召唤限定词"] = "总的请求电能量";
        return object;
    }

    shared_ptr<json> EnergyCall::getPosition() {
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
        (*object)["召唤限定词"] = util::posJson(cIndex, cIndex + asduLength.qualifier);

        return object;
    }

    EnergyCall::EnergyCall(const ASDULengthSet &asdu_length_set)
            : BaseASDU(asdu_length_set) {
        for (int i = 1; i <= asduLength.objectAd + asduLength.qualifier; i++)
            data->push_back(0);

        tie = 101;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    EnergyCall::EnergyCall(const ASDULengthSet &asdu_length_set, long long int tie, long long int vsq,
                           long long int cot, size_t command_address, size_t object_address, long long int qcc)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address, object_address), _qcc(qcc) {
        util::pushElems(data, asduLength.qualifier, _qcc);
    }

    void EnergyCall::set_object_address(long long int object_address) {
        oAddr = object_address;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd)};
        util::replaceElems(data, index, asduLength.objectAd, object_address);
    }

    EnergyCall::EnergyCall(const EnergyCall &rhs) : BaseASDU(rhs), _qcc(rhs._qcc) {

    }

    EnergyCall &EnergyCall::operator=(const EnergyCall &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);
        _qcc = rhs._qcc;
        return *this;
    }
} // asdu