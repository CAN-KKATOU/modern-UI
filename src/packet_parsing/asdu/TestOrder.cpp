//
// Created by Astria on 2022/4/25.
//

#include "TestOrder.h"

namespace asdu {
    TestOrder::TestOrder(unique_ptr<vector<int>> data, const ASDULengthSet &als)
            : BaseASDU(move(data), als) {
        size_t skipLength = asduLength.type +
                            asduLength.qualifier +
                            asduLength.cot +
                            asduLength.commonAd +
                            asduLength.objectAd;

        try {
            _fbp = data->at(skipLength + 1) * 256 + data->at(skipLength);

            if (_fbp != 21930) // 55aaH
                errInfo.append("固定测试图像FBP错误 ");
        } catch (const std::out_of_range &e) {
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

        if (_fbp != 21930) // 55aaH
            errInfo.append("固定测试图像FBP错误 ");
    }

    shared_ptr<json> TestOrder::toJson() {
        auto object = make_shared<json>();
        (*object)["ASDU报文类型"] = "测试命令报文";
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
        (*object)["固定测试图像"] = util::decToHex(_fbp, 4);
        return object;
    }

    shared_ptr<json> TestOrder::getPosition() {
        auto object = make_shared<json>();

        size_t skipLength = asduLength.startIndex;
        (*object)["ASDU报文类型"] = util::posJson(skipLength, skipLength + asduLength.type);

        skipLength += asduLength.type + asduLength.qualifier;
        (*object)["传送原因"] = util::posJson(skipLength, skipLength + asduLength.cot);

        skipLength += asduLength.cot;
        (*object)["公共地址"] = util::posJson(skipLength, skipLength + asduLength.commonAd);

        skipLength += asduLength.commonAd;
        (*object)["信息对象地址"] = util::posJson(skipLength, skipLength + asduLength.objectAd);

        skipLength += asduLength.objectAd;
        (*object)["固定测试图像"] = util::posJson(skipLength, skipLength + 2);

        return object;
    }

    TestOrder::TestOrder(const ASDULengthSet &asdu_length_set)
            : BaseASDU(asdu_length_set) {
        for (int i = 1; i <= asduLength.objectAd + 2; i++)
            data->push_back(0);

        tie = 104;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    TestOrder::TestOrder(const ASDULengthSet &asdu_length_set, long long int tie, long long int vsq, long long int cot,
                         size_t command_address, size_t object_address, long long int fbp)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address, object_address), _fbp(fbp) {
        util::pushElems(data, 2, _fbp);
    }

    TestOrder::TestOrder(const TestOrder &rhs) : BaseASDU(rhs), _fbp(rhs._fbp){

    }

    TestOrder &TestOrder::operator=(const TestOrder &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);
        _fbp = rhs._fbp;
        return *this;
    }

    void TestOrder::set_object_address(long long int object_address) {
        oAddr = object_address;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd)};
        util::replaceElems(data, index, asduLength.objectAd, oAddr);
    }
} // asdu