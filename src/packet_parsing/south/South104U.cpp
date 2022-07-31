//
// Created by Astria on 2022/7/6.
//

#include "South104U.h"

namespace PktParse {
    South104U::South104U(const string &dataStr, const LengthSet &lengthSet)
            : South104(dataStr, lengthSet) {
        if (length_flag) {
            switch (cArea1) {
                case 67:
                    bTest = true;
                    bOrder = true;
                    break;
                case 131:
                    bTest = true;
                    break;
                case 19:
                    bStop = true;
                    bOrder = true;
                    break;
                case 35:
                    bStop = true;
                    break;
                case 7:
                    bStart = true;
                    bOrder = true;
                    break;
                case 11:
                    bStart = true;
                    break;
                default:
                    error_info += "控制信息错误 ";
                    break;
            }
        }
    }

    shared_ptr<json> South104U::toJson() {
        auto object = make_shared<json>();

        if (!time_str.empty())
            (*object)["报文时间"] = time_str;

        if (length_flag) {
            (*object)["报文类型"] = "104U格式报文";
            if (this->bTest)
                (*object)["功能"] = "TESTFR";
            else if (this->bStop)
                (*object)["功能"] = "STOPDT";
            else if (this->bStart)
                (*object)["功能"] = "STARTDT";
            else (*object)["功能"] = "None";

            if (bOrder)
                (*object)["确认/命令"] = "命令";
            else (*object)["确认/命令"] = "确认";
        } else {
            (*object)["解析错误"] = "报文长度错误";
        }
        return object;
    }

    shared_ptr<json> South104U::getPosition() {
        auto object = make_shared<json>();

        if (length_flag) {
            (*object)["报文类型"] = util::posJson(1, 2);

            (*object)["功能"] = util::posJson(2, 3);

            (*object)["确认/命令"] = util::posJson(2, 3);
        } else {
            (*object)["解析错误"] = util::posJson(0, origin_data->size() - 1);
        }
        return object;
    }

    South104U::South104U(const LengthSet &lengthSet)
            : South104(lengthSet) {
        mLength = 4;

        origin_data->push_back(104);
        origin_data->push_back(6);
        origin_data->push_back(cArea1);
        origin_data->push_back(cArea2);
        origin_data->push_back(cArea3);
        origin_data->push_back(cArea4);
    }

    South104U::South104U(const LengthSet &lengthSet, Packet104UOrder order, bool isOrder)
            : South104(lengthSet) {
        switch (order) {
            case Packet104UOrder::TESTFR: {
                if (isOrder)
                    cArea1 = 67;
                else
                    cArea1 = 131;
                break;
            }
            case Packet104UOrder::STOPDT: {
                if (isOrder)
                    cArea1 = 19;
                else
                    cArea1 = 35;
                break;
            }
            case Packet104UOrder::STARTDT: {
                if (isOrder)
                    cArea1 = 7;
                else
                    cArea1 = 11;
                break;
            }
        }
        mLength = 4;

        origin_data->push_back(104);
        origin_data->push_back(6);
        origin_data->push_back(cArea1);
        origin_data->push_back(cArea2);
        origin_data->push_back(cArea3);
        origin_data->push_back(cArea4);
    }

    South104U::South104U(const South104U &rhs)
            : South104(rhs), bOrder(rhs.bOrder), bTest(rhs.bTest), bStop(rhs.bStop), bStart(rhs.bStart) {

    }

    South104U &South104U::operator=(const South104U &rhs) {
        if (this == &rhs)
            return *this;

        South104::operator=(rhs);
        bOrder = rhs.bOrder;
        bTest = rhs.bTest;
        bStop = rhs.bStop;
        bStart = rhs.bStart;
        return *this;
    }

    void South104U::set_order(Packet104UOrder order, bool isOrder) {
        switch (order) {
            case Packet104UOrder::TESTFR: {
                if (isOrder)
                    cArea1 = 67;
                else
                    cArea1 = 131;
                break;
            }
            case Packet104UOrder::STOPDT: {
                if (isOrder)
                    cArea1 = 19;
                else
                    cArea1 = 35;
                break;
            }
            case Packet104UOrder::STARTDT: {
                if (isOrder)
                    cArea1 = 7;
                else
                    cArea1 = 11;
                break;
            }
        }
        mLength = 4;

        origin_data->push_back(104);
        origin_data->push_back(6);
        origin_data->push_back(cArea1);
        origin_data->push_back(cArea2);
        origin_data->push_back(cArea3);
        origin_data->push_back(cArea4);
    }
} // PktParse
