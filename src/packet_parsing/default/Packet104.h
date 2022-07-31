//
// Created by Astria on 2022/4/24.
//

#ifndef TEST_MESSAGE_104_H
#define TEST_MESSAGE_104_H

#include "../BasePacket.h"

namespace PktParse {

    class Packet104 : public BasePacket {
    protected:
        size_t mLength{0ULL};
        int cArea1{0};
        int cArea2{0};
        int cArea3{0};
        int cArea4{0};

    public:
        explicit Packet104(const LengthSet &lengthSet) : BasePacket(lengthSet) {}

        Packet104(const string &dataStr, const LengthSet &lengthSet);

        Packet104(const Packet104 &rhs);

        ~Packet104() override = default;

        Packet104 &operator=(const Packet104 &rhs);

        long long get_tie() const override = 0;

        long long get_vsq() const override = 0;

        long long get_cot() const override = 0;

        shared_ptr<json> toJson() override = 0;

        shared_ptr<json> getPosition() override = 0;

        string getType() const override = 0;

        size_t pktLength() const override { return mLength; };

        /**
         * @brief 解析104协议报文类型
         * @param dataStr 原始报文数据
         * @param typeLength 类型标识符长度
         * @return 若解析成功则返回对应类型字符串，否则返回‘未知报文’
         * @version 2022-5-25
         */
        static string parse104Type(const string &dataStr, int typeLength);

        /**
         * @brief 返回104报文类型
         * @param dataStr 原始报文数据
         * @param typeLength 类型标识符长度
         * @return >0 I格式报文具体ASDU类型
         * @return -2 S格式报文
         * @return -3 U格式报文
         * @return -4 错误
         */
        static int parse104TypeInt(const string &dataStr, int typeLength);

        /**
         * @brief 获取104包括未知报文在内的所有报文类型的列表
         * @return std::vector<std::string>
         * @version 2022-5-25
         */
        static vector<string> getAllTypes();

        /**
         * @brief 返回104报文类型
         * @param dataStr 原始报文数据
         * @param dataVec 如果已有转化为std::vector<int>对象的智能指针，则传入，否则缺省nullptr
         * @return 1 I格式报文
         * @return 2 S格式报文
         * @return 3 U格式报文
         * @return -1 解析失败，未知报文
         * @version 2022-5-25
         */
        static int select104Type(const string &dataStr, const shared_ptr<vector<int>> &dataVec = nullptr);
    };

} // PktParse

#endif //TEST_MESSAGE_104_H
