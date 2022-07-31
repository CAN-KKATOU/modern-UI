//
// Created by Astria on 2022/4/25.
//

#include "ResetProcessSouth.h"

namespace ASDUSouth {
    ResetProcessSouth::ResetProcessSouth(std::unique_ptr<std::vector<int>> data, const asdu::ASDULengthSet &als)
            : BaseASDU(std::move(data), als) {
        size_t cIndex{static_cast<size_t>(asduLength.type +
                                          asduLength.qualifier +
                                          asduLength.cot +
                                          asduLength.commonAd +
                                          asduLength.objectAd)};
        try {
            _qrp = util::vecToLL(data, cIndex, asduLength.qualifier);

            if (_qrp != 1)
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
                break;
            default:
                errInfo += "传送原因COT错误 ";
                break;
        }
    }

    std::shared_ptr<nlohmann::json> ResetProcessSouth::toJson() {
        auto object = std::make_shared<nlohmann::json>();
        (*object)["ASDU报文类型"] = "复位进程命令报文";
        switch (cot) {
            case 6:
                (*object)["传送原因"] = "激活";
                break;
            case 7:
                (*object)["传送原因"] = "激活确认";
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

    std::shared_ptr<nlohmann::json> ResetProcessSouth::getPosition() {
        auto object = std::make_shared<nlohmann::json>();

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

    ResetProcessSouth::ResetProcessSouth(const asdu::ASDULengthSet &asdu_length_set)
            : BaseASDU(asdu_length_set) {
        for (int i = 0; i < asduLength.objectAd + asduLength.qualifier; i++)
            data->push_back(0);

        tie = 105;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    ResetProcessSouth::ResetProcessSouth(const asdu::ASDULengthSet &asdu_length_set, long long int tie,
                                         long long int vsq, long long int cot, size_t command_address,
                                         size_t object_address, long long int qrp)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address, object_address), _qrp(qrp) {
        util::pushElems(data, asduLength.qualifier, _qrp);
    }

    void ResetProcessSouth::set_object_address(long long int object_address) {
        oAddr = object_address;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd)};
        util::replaceElems(data, index, asduLength.objectAd, object_address);
    }

    ResetProcessSouth::ResetProcessSouth(const ResetProcessSouth &rhs) : asdu::BaseASDU(rhs), _qrp(rhs._qrp) {

    }

    ResetProcessSouth &ResetProcessSouth::operator=(const ResetProcessSouth &rhs) {
        if (this == &rhs)
            return *this;

        asdu::BaseASDU::operator=(rhs);
        _qrp = rhs._qrp;
        return *this;
    }
} // ASDUSouth