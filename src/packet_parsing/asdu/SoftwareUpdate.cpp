//
// Created by Astria on 2022/4/25.
//

#include "SoftwareUpdate.h"

namespace asdu {
    SoftwareUpdate::SoftwareUpdate(unique_ptr<vector<int>> data, const ASDULengthSet &als)
            : BaseASDU(move(data), als) {
        size_t skipLength{static_cast<size_t>(asduLength.type +
                                              asduLength.qualifier +
                                              asduLength.cot +
                                              asduLength.commonAd +
                                              asduLength.objectAd)};

        _cType = data->at(skipLength);

        switch (cot) {
            case 6:
            case 8:
            case 7:
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

    shared_ptr<json> SoftwareUpdate::toJson() {
        auto object = make_shared<json>();
        (*object)["ASDU报文类型"] = "软件升级报文";
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
        (*object)["公共地址"] = cAddr;
        (*object)["信息对象地址"] = oAddr;

        if (_cType > 127)
            (*object)["命令类型"] = "软件升级启动";
        else (*object)["命令类型"] = "软件升级结束";

        return object;
    }

    shared_ptr<json> SoftwareUpdate::getPosition() {
        auto object = make_shared<json>();
        size_t cIndex{static_cast<size_t>(asduLength.startIndex)};
        (*object)["ASDU报文类型"] = util::posJson(cIndex, cIndex + asduLength.type);
        (*object)["标识类型"] = util::posJson(cIndex, cIndex + asduLength.type);

        cIndex += asduLength.type + asduLength.qualifier;
        (*object)["传送原因"] = util::posJson(cIndex, cIndex + asduLength.cot);
        (*object)["报文方向"] = util::posJson(cIndex, cIndex + asduLength.cot);

        cIndex += asduLength.cot;
        (*object)["公共地址"] = util::posJson(cIndex, cIndex + asduLength.commonAd);

        cIndex += asduLength.commonAd;
        (*object)["信息对象地址"] = util::posJson(cIndex, cIndex + asduLength.objectAd);

        cIndex += asduLength.objectAd;
        (*object)["命令类型"] = util::posJson(cIndex, cIndex + 1);

        return object;
    }

    SoftwareUpdate::SoftwareUpdate(const ASDULengthSet &asdu_length_set)
            : BaseASDU(asdu_length_set) {
        for (int i = 1; i <= asduLength.objectAd + 1; i++)
            data->push_back(0);

        tie = 211;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    SoftwareUpdate::SoftwareUpdate(const ASDULengthSet &asdu_length_set, long long int tie, long long int vsq,
                                   long long int cot, size_t command_address, size_t object_address, int cType)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address, object_address), _cType(cType) {
        data->push_back(_cType);
    }

    void SoftwareUpdate::set_object_address(long long int object_address) {
        oAddr = object_address;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd)};
        util::replaceElems(data, index, asduLength.objectAd, oAddr);
    }

    SoftwareUpdate::SoftwareUpdate(const SoftwareUpdate &rhs)
            : BaseASDU(rhs), _cType(rhs._cType) {

    }

    SoftwareUpdate &SoftwareUpdate::operator=(const SoftwareUpdate &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);
        _cType = rhs._cType;
        return *this;
    }
} // asdu