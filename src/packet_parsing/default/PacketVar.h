//
// Created by Astria on 2022/4/24.
//

#ifndef TEST_VARIABLE_MESSAGE_H
#define TEST_VARIABLE_MESSAGE_H

#include "Packet101.h"
#include "../asdu/BaseASDU.h"

namespace PktParse {

    class PacketVar : public Packet101 {
    private:
        size_t mLength{0ULL};               // 报文长度
        long long tie{0LL};                 // asdu类型
        size_t aIndex{0LL};                 // asdu开始位置
        shared_ptr<asdu::BaseASDU> aPtr;    // asdu指针

    public:
        explicit PacketVar(const LengthSet &lengthSet);

        PacketVar(const LengthSet &lengthSet, int controlArea, size_t addressArea, long long type,
                  shared_ptr<asdu::BaseASDU> asduPtr);

        PacketVar(const LengthSet &lengthSet, bool dir, bool prm, bool fcb, bool fcv, bool acd, bool dfc, int fc,
                  size_t addressArea, long long type, shared_ptr<asdu::BaseASDU> asdu);

        PacketVar(const string &dataStr, const LengthSet &lengthSet);

        PacketVar(const PacketVar &rhs);

        ~PacketVar() override = default;

        PacketVar &operator=(const PacketVar &rhs);

        bool set_asdu(shared_ptr<asdu::BaseASDU> asduPtr);

        long long get_tie() const override;

        long long get_vsq() const override;

        long long get_cot() const override;

        std::shared_ptr<nlohmann::json> toJson() override;

        std::shared_ptr<nlohmann::json> getPosition() override;

        string getType() const override;

        size_t pktLength() const override;

        /**
         * @brief 返回101可变帧长报文类型
         * @param dataStr 原始报文数据
         * @param linkLength 链路地址长度
         * @param typeLength 类型长度
         * @param dataVec 如果已有转化为QList<int>对象的智能指针，则传入，否则缺省nullptr
         * @return string
         * @version 2022-5-25
         */
        static string
        parseVarType(const string &dataStr, int linkLength, int typeLength,
                     const shared_ptr<vector<int>> &dataVec = nullptr);

        static int parseVarTypeInt(const string &data_str, int link_length, int type_length,
                                   const shared_ptr<vector<int>> &data_vec);
    };

} // PktParse

#endif //TEST_VARIABLE_MESSAGE_H
