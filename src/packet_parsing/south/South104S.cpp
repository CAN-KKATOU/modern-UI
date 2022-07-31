//
// Created by Astria on 2022/7/6.
//

#include "South104S.h"

namespace PktParse {
    South104S::South104S(const string &dataStr, const LengthSet &lengthSet)
            : South104(dataStr, lengthSet) {
        if (length_flag)
            rNum = (cArea4 * 256 + cArea3) / 2;
    }

    shared_ptr<json> South104S::toJson() {
        auto object = make_shared<json>();

        if (!time_str.empty())
            (*object)["报文时间"] = time_str;

        if (length_flag) {
            (*object)["报文类型"] = "104S格式报文";
            (*object)["接收序列号"] = rNum;
        } else {
            (*object)["解析错误"] = "报文长度错误";
        }
        return object;
    }

    shared_ptr<json> South104S::getPosition() {
        auto object = make_shared<json>();

        if (length_flag) {
            (*object)["报文类型"] = util::posJson(1, 2);
            (*object)["接收序列号"] = util::posJson(4, 6);
        } else {
            (*object)["解析错误"] = util::posJson(0, origin_data->size() - 1);
        }
        return object;
    }

    South104S::South104S(const LengthSet &lengthSet)
            : South104(lengthSet) {
        mLength = 4;
        cArea1 = 1;
        cArea2 = 0;

        origin_data->push_back(104);
        origin_data->push_back(6);
        origin_data->push_back(cArea1);
        origin_data->push_back(cArea2);
        origin_data->push_back(cArea3);
        origin_data->push_back(cArea4);
    }

    South104S::South104S(const LengthSet &lengthSet, int rNum)
            : South104(lengthSet) {
        mLength = 4;
        cArea1 = 1;
        cArea2 = 0;
        cArea3 = (rNum % 128) * 2;
        cArea4 = rNum / 128;
        this->rNum = rNum;

        origin_data->push_back(104);
        origin_data->push_back(6);
        origin_data->push_back(cArea1);
        origin_data->push_back(cArea2);
        origin_data->push_back(cArea3);
        origin_data->push_back(cArea4);
    }

    South104S::South104S(const South104S &rhs) : South104(rhs), rNum(rhs.rNum){

    }

    South104S &South104S::operator=(const South104S &rhs) {
        if (this == &rhs)
            return *this;

        South104::operator=(rhs);
        rNum = rhs.rNum;
        return *this;
    }
} // PktParse
