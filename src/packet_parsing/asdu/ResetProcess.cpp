//
// Created by Astria on 2022/4/25.
//

#include "ResetProcess.h"

namespace asdu {
    ResetProcess::ResetProcess(unique_ptr<vector<int>> data, const ASDULengthSet &als)
            : BaseASDU(move(data), als) {
        size_t cIndex{static_cast<size_t>(asduLength.type +
                                          asduLength.qualifier +
                                          asduLength.cot +
                                          asduLength.commonAd +
                                          asduLength.objectAd)};
        try {
            qrp = util::vecToLL(data, cIndex, asduLength.qualifier);

            if (qrp != 1)
                errInfo.append("复位进程命令限定词QRP错误 ");
        } catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
        }
        catch (const std::out_of_range &e) {
            std::cerr << e.what() << std::endl;
        }

        switch (cot) {
            case 6:
            case 7:
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

    shared_ptr<json> ResetProcess::toJson() {
        auto object = make_shared<json>();
        (*object)["ASDU报文类型"] = "复位进程命令报文";
        switch (cot) {
            case 6:
                (*object)["传送原因"] = "激活";
                break;
            case 7:
                (*object)["传送原因"] = "激活确认";
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
        (*object)["复位进程命令限定词"] = "进程的总复位";
        return object;
    }

    shared_ptr<json> ResetProcess::getPosition() {
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
        (*object)["复位进程命令限定词"] = util::posJson(cIndex, cIndex + asduLength.qualifier);

        return object;
    }

    ResetProcess::ResetProcess(const ASDULengthSet &asduLengthSet) : BaseASDU(asduLengthSet) {
        for (int i = 0; i < asduLength.objectAd + asduLength.qualifier; i++)
            data->push_back(0);

        tie = 105;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    ResetProcess::ResetProcess(const ASDULengthSet &asduLengthSet, long long int tie, long long int vsq,
                               long long int cot, size_t commandAddress, size_t objectAddress, long long int qrp)
            : BaseASDU(asduLengthSet, tie, vsq, cot, commandAddress, objectAddress), qrp(qrp) {
        util::pushElems(data, asduLength.qualifier, qrp);
    }

    void ResetProcess::set_object_address(long long int object_address) {
        oAddr = object_address;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd)};
        util::replaceElems(data, index, asduLength.objectAd, object_address);
    }

    ResetProcess::ResetProcess(const ResetProcess &rhs) : BaseASDU(rhs), qrp(rhs.qrp){

    }

    ResetProcess &ResetProcess::operator=(const ResetProcess &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);
        qrp = rhs.qrp;
        return *this;
    }
} // asdu