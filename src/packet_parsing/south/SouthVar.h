//
// Created by Astria on 2022/7/6.
//

#ifndef ANALYSIS_CORE_C__17_SOUTHVARIABLE_H
#define ANALYSIS_CORE_C__17_SOUTHVARIABLE_H

#include "South101.h"
#include "../asdu/BaseASDU.h"

namespace PktParse {

    class SouthVar : public South101 {
    private:
        size_t mLength{0ULL};               // 报文长度
        long long tie{0LL};                 // asdu类型
        size_t aIndex{0LL};                 // asdu开始位置
        shared_ptr<asdu::BaseASDU> aPtr;   // asdu指针

    public:
        explicit SouthVar(const LengthSet &lengthSet);

        SouthVar(const LengthSet &lengthSet, int controlArea, size_t addressArea, long long type,
                 shared_ptr<asdu::BaseASDU> asduPtr);

        SouthVar(const LengthSet &lengthSet, bool dir, bool prm, bool fcb, bool fcv, bool acd, bool dfc, int fc,
                 size_t addressArea, long long type, shared_ptr<asdu::BaseASDU> asduPtr);

        SouthVar(const string &dataStr, const LengthSet &lengthSet);

        SouthVar(const SouthVar &rhs);

        ~SouthVar() override = default;

        SouthVar &operator=(const SouthVar &rhs);

        bool set_asdu(shared_ptr<asdu::BaseASDU> asduPtr);

        long long get_tie() const override;

        long long get_vsq() const override;

        long long get_cot() const override;

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        string getType() const override;

        size_t pktLength() const override;

        /**
         * @brief 返回佛山101可变帧长报文类型
         * @param dataStr 原始报文数据
         * @param linkLength 链路地址长度
         * @param typeLength 类型长度
         * @param dataVec 如果已有转化为QList<int>对象的智能指针，则传入，否则缺省nullptr
         * @return QString
         * @version 2022-5-25
         */
        static string
        parseVarType(const string &dataStr, int linkLength, int typeLength,
                     const shared_ptr<vector<int>> &dataVec = nullptr);

        static int parseVarTypeInt(const string &dataStr, int linkLength, int typeLength,
                                   const shared_ptr<vector<int>> &dataVec);
    };

} // PktParse

#endif //ANALYSIS_CORE_C__17_SOUTHVARIABLE_H
