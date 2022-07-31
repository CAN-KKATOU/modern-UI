//
// Created by Astria on 2022/4/25.
//

#include "GeneralCallSouth.h"

namespace ASDUSouth {
    GeneralCallSouth::GeneralCallSouth(std::unique_ptr<std::vector<int>> data, const asdu::ASDULengthSet &als)
            : BaseASDU(std::move(data), als) {
        size_t cIndex{static_cast<size_t>(asduLength.type +
                                          asduLength.qualifier +
                                          asduLength.cot +
                                          asduLength.commonAd +
                                          asduLength.objectAd)};
        try {
            _qoi = util::vecToLL(data, cIndex, asduLength.qualifier);

            if (_qoi != 20)
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

    std::shared_ptr<nlohmann::json> GeneralCallSouth::toJson() {
        auto object = std::make_shared<nlohmann::json>();
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

    std::shared_ptr<nlohmann::json> GeneralCallSouth::getPosition() {
        auto object = std::make_shared<nlohmann::json>();

        size_t index{static_cast<size_t>(asduLength.startIndex)};
        (*object)["ASDU报文类型"] = util::posJson(index, index + asduLength.type);

        index += asduLength.type + asduLength.qualifier;
        (*object)["传送原因"] = util::posJson(index, index + asduLength.cot);

        index += asduLength.cot;
        (*object)["公共地址"] = util::posJson(index, index + asduLength.commonAd);

        index += asduLength.commonAd;
        (*object)["信息对象地址"] = util::posJson(index, index + asduLength.objectAd);

        index += asduLength.objectAd;
        (*object)["召唤限定词"] = util::posJson(index, index + asduLength.qualifier);

        return object;
    }

    GeneralCallSouth::GeneralCallSouth(const asdu::ASDULengthSet &asdu_length_set)
            : BaseASDU(asdu_length_set) {
        for (int i = 1; i <= asduLength.objectAd + asduLength.qualifier; i++)
            data->push_back(0);

        tie = 100;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    GeneralCallSouth::GeneralCallSouth(const asdu::ASDULengthSet &asdu_length_set, long long int tie, long long int vsq,
                                       long long int cot, size_t command_address, size_t object_address,
                                       long long int qoi)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address, object_address), _qoi(qoi) {
        util::pushElems(data, asduLength.qualifier, _qoi);
    }

    void GeneralCallSouth::set_object_address(long long int object_address) {
        oAddr = object_address;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd)};
        util::replaceElems(data, index, asduLength.objectAd, oAddr);
    }

    GeneralCallSouth::GeneralCallSouth(const GeneralCallSouth &rhs) : asdu::BaseASDU(rhs), _qoi(rhs._qoi) {

    }

    GeneralCallSouth &GeneralCallSouth::operator=(const GeneralCallSouth &rhs) {
        if (this == &rhs)
            return *this;

        asdu::BaseASDU::operator=(rhs);
        _qoi = rhs._qoi;
        return *this;
    }
} // ASDUSouth