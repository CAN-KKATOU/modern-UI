//
// Created by Astria on 2022/4/25.
//

#include "GeneralCall.h"

namespace asdu {
    GeneralCall::GeneralCall(unique_ptr<vector<int>> dataVec, const ASDULengthSet &aasduLengthSet)
            : BaseASDU(move(dataVec), aasduLengthSet) {
        size_t cIndex{static_cast<size_t>(asduLength.type +
                                          asduLength.qualifier +
                                          asduLength.cot +
                                          asduLength.commonAd +
                                          asduLength.objectAd)};
        try {
            qoi = util::vecToLL(dataVec, cIndex, asduLength.qualifier);

            if (qoi != 20)
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
            case 8:
            case 9:
            case 10:
            case 20:
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

    shared_ptr<json> GeneralCall::toJson() {
        auto object = make_shared<json>();
        (*object)["ASDU报文类型"] = "总召唤报文";
        switch (cot) {
            case 6:
                (*object)["传送原因"] = "激活";
                break;
            case 7:
                (*object)["传送原因"] = "激活确认";
                break;
            case 8:
                (*object)["传送原因"] = "停止激活";
                break;
            case 9:
                (*object)["传送原因"] = "停止激活确认";
                break;
            case 10:
                (*object)["传送原因"] = "激活终止";
                break;
            case 20:
                (*object)["传送原因"] = "响应总召唤";
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
        (*object)["召唤限定词"] = "总召唤";
        return object;
    }

    shared_ptr<json> GeneralCall::getPosition() {
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

    GeneralCall::GeneralCall(const ASDULengthSet &asduLengthSet) : BaseASDU(asduLengthSet) {
        for (int i = 1; i <= asduLength.objectAd + asduLength.qualifier; i++)
            data->push_back(0);

        tie = 100;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    GeneralCall::GeneralCall(const ASDULengthSet &asduLengthSet, long long int tie, long long int vsq,
                             long long int cot, size_t commandAddress, size_t objectAddress, long long int qoi)
            : BaseASDU(asduLengthSet, tie, vsq, cot, commandAddress, objectAddress), qoi(qoi) {
        util::pushElems(data, asduLength.qualifier, qoi);
    }

    void GeneralCall::set_object_address(long long int objectAddress) {
        oAddr = objectAddress;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd)};
        util::replaceElems(data, index, asduLength.objectAd, oAddr);
    }

    GeneralCall::GeneralCall(const GeneralCall &rhs) : BaseASDU(rhs), qoi(rhs.qoi) {

    }

    GeneralCall &GeneralCall::operator=(const GeneralCall &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);
        qoi = rhs.qoi;
        return *this;
    }
} // asdu