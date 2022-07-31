//
// Created by Astria on 2022/4/25.
//

#include "Clock.h"

namespace asdu {
    Clock::Clock(unique_ptr<vector<int>> data, const ASDULengthSet &als)
            : BaseASDU(move(data), als) {
        size_t cIndex{static_cast<size_t>(asduLength.type +
                                          asduLength.qualifier +
                                          asduLength.cot +
                                          asduLength.commonAd +
                                          asduLength.objectAd)};

        try {
            timeStr = util::parseTime(util::vecMid(data, cIndex, 7));
        }
        catch (const std::out_of_range &e) {
            std::cerr << e.what() << std::endl;
        }

        switch (cot) {
            case 5:
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
    }

    shared_ptr<json> Clock::toJson() {
        auto object = make_shared<json>();
        (*object)["ASDU报文类型"] = "时钟同步/读取报文";
        switch (cot) {
            case 5:
                (*object)["传送原因"] = "请求/被请求";
                break;
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
        (*object)["时标"] = timeStr;
        return object;
    }

    shared_ptr<json> Clock::getPosition() {
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
        (*object)["时标"] = util::posJson(cIndex, cIndex + 7);

        return object;
    }

    Clock::Clock(const ASDULengthSet &asdu_length_set) : BaseASDU(asdu_length_set) {
        for (int i = 1; i <= asduLength.objectAd + 7; i++)
            data->push_back(0);

        tie = 103;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    Clock::Clock(const ASDULengthSet &asdu_length_set, long long int tie, long long int vsq, long long int cot,
                 size_t command_address, size_t object_address, const string &time_str)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address, object_address), timeStr(time_str) {
        util::pushTime(data, time_str);
    }

    void Clock::set_object_address(long long int object_address) {
        oAddr = object_address;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd)};
        util::replaceElems(data, index, asduLength.objectAd, object_address);
    }

    void Clock::set_time(const string &time) {
        timeStr = time;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd +
                                         asduLength.objectAd)};
        util::replaceTime(data, index, timeStr);
    }

    Clock::Clock(const Clock &rhs) : BaseASDU(rhs), timeStr(rhs.timeStr) {

    }

    Clock &Clock::operator=(const Clock &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);
        timeStr = rhs.timeStr;
        return *this;
    }
} // asdu