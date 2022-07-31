//
// Created by Astria on 2022/6/1.
//

#ifndef PACKET_FACTORY_H
#define PACKET_FACTORY_H

#include "BasePacket.h"
#include <memory>

namespace PktParse {

    class PacketFactory {
    public:
        PacketFactory() = default;

        ~PacketFactory() = default;

        /**
         * @brief 根据协议获取该解析协议下所有报文类型
         * @param mode 协议
         * @return QStringList
         * @version 20220608
         */
        static vector<string> get_types_list(AnalysisMode mode);

        /**
         * @brief 根据原始数据，协议和配置长度返回报文类型
         * @param data_str 原始16进制数据
         * @param mode 协议
         * @param length 配置长度
         * @return 判断成功则返回对应报文类型，否则返回‘未知报文’
         * @version 20220608
         */
        static string parse_type(const string &data_str, AnalysisMode mode, const LengthSet &length);

        /**
         * @brief 以数字返回报文类型
         * @param data_str 原始16进制数据
         * @param mode 协议
         * @param length 配置长度
         * @return >0 可变帧长格式报文或I格式报文具体ASDU类型
         * @return -1 固定帧长报文
         * @return -2 S格式报文
         * @return -3 U格式报文
         * @return -4 错误
         * @version 20220608
         */
        static int parse_type_int(const string &data_str, AnalysisMode mode, const LengthSet &length);

        static BasePacket *getPtr(const string &str, AnalysisMode mode, const LengthSet &length, const string &time_str = "");

        static std::unique_ptr<BasePacket>
        getUPtr(const string &dataStr, AnalysisMode mode, const LengthSet &length, const string &time_str = "");

        static std::shared_ptr<BasePacket>
        getSPtr(const string &dataStr, AnalysisMode mode, const LengthSet &length, const string &time_str = "");
    };
}


#endif //PACKET_FACTORY_H
