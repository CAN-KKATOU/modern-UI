//
// Created by Astria on 2022/7/6.
//

#include "South104.h"
#include "South104I.h"

namespace PktParse {
    South104::South104(const string &dataStr, const LengthSet &lengthSet)
            : BasePacket(dataStr, lengthSet) {
        mLength = origin_data->at(1);
        if (origin_data->size() - 2 == mLength) {
            length_flag = true;
            cArea1 = origin_data->at(2);
            cArea2 = origin_data->at(3);
            cArea3 = origin_data->at(4);
            cArea4 = origin_data->at(5);
        } else length_flag = false;
    }

    string South104::parse104Type(const string &dataStr, int typeLength) {
        auto data = make_shared<vector<int>>();
        *data = util::splitToInt(dataStr, " ");

        switch (select104Type(dataStr)) {
            case 1:
                return South104I::parseIType(dataStr, typeLength, data);
            case 2:
                return "S格式报文";
            case 3:
                return "U格式报文";
            default:
                return "未知报文";
        }
    }

    int South104::select104Type(const string &dataStr, const shared_ptr<vector<int>> &dataVec) {
        auto data = make_shared<vector<int>>();

        if (dataVec == nullptr) {
            *data = util::splitToInt(dataStr, " ");
        } else data = dataVec;

        if (data->size() < 6)
            return -1;

        int length = data->at(1);
        int ca_1 = data->at(2);
        int ca_3 = data->at(4);

        if (length > 4 && (ca_1 % 2 == 0) && (ca_3 % 2 == 0)) // I格式
            return 1;
        else if (length == 4 && (ca_1 % 4 == 1) && (ca_3 % 2 == 0)) // S格式
            return 2;
        else if (length == 4 && (ca_1 % 4 == 3) && (ca_3 % 2 == 0)) // U格式
            return 3;

        return -1;
    }

    vector<string> South104::getAllTypes() {
        vector<string> vec;
        vec.emplace_back("总召唤报文");
        vec.emplace_back("时钟同步/读取报文");
        vec.emplace_back("复位进程命令报文");
        vec.emplace_back("初始化结束命令报文");
        vec.emplace_back("遥信命令报文");
        vec.emplace_back("遥测命令报文");
        vec.emplace_back("遥控命令报文");
        vec.emplace_back("预置/激活参数报文");
        vec.emplace_back("读参数报文");
        vec.emplace_back("故障值信息报文");
        vec.emplace_back("文件服务报文");
        vec.emplace_back("电能量召唤报文");
        vec.emplace_back("电能量数据报文");
        vec.emplace_back("切换定值区报文");
        vec.emplace_back("读当前定值区号报文");
        vec.emplace_back("读参数和定值报文");
        vec.emplace_back("写参数和定值报文");
        vec.emplace_back("软件升级报文");
        vec.emplace_back("S格式报文");
        vec.emplace_back("U格式报文");
        vec.emplace_back("未知报文");
        return vec;
    }

    int South104::parse104TypeInt(const string &dataStr, int typeLength) {
        auto data = make_shared<vector<int>>();
        *data = util::splitToInt(dataStr, " ");

        switch (select104Type(dataStr, data)) {
            case 1:
                return South104I::parseITypeInt(dataStr, typeLength, data);
            case 2:
                return -2;
            case 3:
                return -3;
            default:
                return -4;
        }
    }

    South104::South104(const South104 &rhs)
            : BasePacket(rhs), mLength(rhs.mLength),
              cArea1(rhs.cArea1), cArea2(rhs.cArea2), cArea3(rhs.cArea3), cArea4(rhs.cArea4) {

    }

    South104 &South104::operator=(const South104 &rhs) {
        if (this == &rhs)
            return *this;

        BasePacket::operator=(rhs);
        mLength = rhs.mLength;
        cArea1 = rhs.cArea1;
        cArea2 = rhs.cArea2;
        cArea3 = rhs.cArea3;
        cArea4 = rhs.cArea4;
        return *this;
    }
} // PktParse