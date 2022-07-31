//
// Created by Astria on 2022/4/24.
//

#ifndef TEST_MESSAGE_104_I_H
#define TEST_MESSAGE_104_I_H

#include "Packet104.h"
#include "../asdu/BaseASDU.h"

namespace PktParse {

    class Packet104I : public Packet104 {
    private:
        size_t sNum{0ULL};                   // 发送序号
        size_t rNum{0ULL};                   // 接收序号
        size_t aIndex{6ULL};                 // asdu起始位置
        size_t tie{0ULL};                    // asdu类型
        shared_ptr<asdu::BaseASDU> aPtr;   // asdu指针

    public:
        explicit Packet104I(const LengthSet &lengthSet);

        Packet104I(const LengthSet &lengthSet,
                   size_t sendNum,
                   size_t receiveNum,
                   shared_ptr<asdu::BaseASDU> asduPtr
        );

        Packet104I(const string &dataStr, const LengthSet &lengthSet);

        Packet104I(const Packet104I& rhs);

        ~Packet104I() override = default;

        Packet104I& operator=(const Packet104I& rhs);

        bool set_asdu(shared_ptr<asdu::BaseASDU> asduPtr);

        long long get_tie() const override;

        long long get_vsq() const override;

        long long get_cot() const override;

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        string getType() const override;

        size_t get_send_num() const { return sNum; }

        size_t get_receive_num() const { return rNum; }

        shared_ptr<asdu::BaseASDU> get_asdu_ptr() { return aPtr; }

        /**
        * @brief 解析104协议I格式报文类型
        * @param dataStr 原始报文数据
        * @param typeLength 类型标识符长度
        * @param dataVec 如果已有转化为std::vector<int>对象的智能指针，则传入，否则缺省nullptr
        * @return 若解析成功，则返回对应报文类型，否则返回‘未知报文’
        * @version 2022-5-25
        */
        static string
        parseIType(const std::string &dataStr, int typeLength, const shared_ptr<vector<int>> &dataVec = nullptr);

        static int
        parseITypeInt(const std::string &dataStr, int typeLength, const shared_ptr<vector<int>> &dataVec = nullptr);
    };

} // PktParse

#endif //TEST_MESSAGE_104_I_H
