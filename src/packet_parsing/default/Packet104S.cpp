//
// Created by Astria on 2022/4/24.
//

#include "Packet104S.h"

namespace PktParse {
    Packet104S::Packet104S(const LengthSet &lengthSet)
            : Packet104(lengthSet) {
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

    Packet104S::Packet104S(const LengthSet &lengthSet, int rNum)
            : Packet104(lengthSet) {
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

    Packet104S::Packet104S(const string &dataStr, const LengthSet &lengthSet)
            : Packet104(dataStr, lengthSet) {
        if (length_flag)
            rNum = (cArea4 * 256 + cArea3) / 2;
    }

    shared_ptr<json> Packet104S::toJson() {
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

    shared_ptr<json> Packet104S::getPosition() {
        auto object = make_shared<json>();

        if (length_flag) {
            (*object)["报文类型"] = util::posJson(1, 2);
            (*object)["接收序列号"] = util::posJson(4, 6);
        } else {
            (*object)["解析错误"] = util::posJson(0, origin_data->size() - 1);
        }
        return object;
    }

    void Packet104S::set_receive_num(int rNum) {
        cArea4 = rNum / 128;
        cArea3 = (rNum % 128) * 2;
        this->rNum = rNum;

        origin_data->at(4) = cArea3;
        origin_data->at(5) = cArea4;
    }

    Packet104S::Packet104S(const Packet104S &rhs) : Packet104(rhs), rNum(rhs.rNum) {

    }

    Packet104S &Packet104S::operator=(const Packet104S &rhs) {
        if (this == &rhs)
            return *this;

        Packet104::operator=(rhs);
        rNum = rhs.rNum;
        return *this;
    }
} // PktParse
