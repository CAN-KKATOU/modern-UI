//
// Created by Astria on 2022/4/24.
//

#ifndef TEST_MESSAGE_101_H
#define TEST_MESSAGE_101_H

#include "../BasePacket.h"

namespace PktParse {

    class Packet101 : public BasePacket {
    protected:
        bool tMode{true}; // true为平衡模式
        int cArea{0};     // 控制域
        size_t aArea{0};  // 地址域
        bool dir{false};
        bool prm{false};
        bool fcb{false};
        bool fcv{false};
        bool acd{false};
        bool dfc{false};
        int fcCode{0};

        string fc2string() const;

    public:
        explicit Packet101(const LengthSet &lengthSet) : BasePacket(lengthSet) { tMode = length.mode; }

        Packet101(const LengthSet &lengthSet, int controlArea, size_t addressArea);

        Packet101(const LengthSet &lengthSet, bool dir, bool prm, bool fcb, bool fcv, bool acd, bool dfc, int fc,
                  size_t addressArea);

        Packet101(const string &dataStr, const LengthSet &lengthSet);

        Packet101(const Packet101 &rhs);

        ~Packet101() override = default;

        Packet101 &operator=(const Packet101 &rhs);

        long long get_tie() const override = 0;

        long long get_vsq() const override = 0;

        long long get_cot() const override = 0;

        shared_ptr<json> toJson() override = 0;

        shared_ptr<json> getPosition() override = 0;

        string getType() const override = 0;

        size_t pktLength() const override = 0;

        /**
         * @brief 解析101协议报文类型
         * @param dataStr 原始报文字符串
         * @param linkLength 链路地址长度
         * @param typeLength 类型标识符长度
         * @return 若解析成功则返回对应类型字符串，否则返回‘未知报文’
         * @version 2022-5-25
         */
        static string parse101Type(const string &dataStr, int linkLength, int typeLength);

        /**
         * @brief 解析101协议报文类型
         * @param dataStr 原始报文字符串
         * @param linkLength 链路地址长度
         * @param typeLength 类型标识符长度
         * @return -4 未知报文
         * @return -1 固定帧长报文
         * @return >0 可变帧长具体类型标识符
         * @version 2022-7-12
         */
        static int parse101TypeInt(const string &dataStr, int linkLength, int typeLength);

        /**
         * @brief 返回101报文类型
         * @param dataStr 原始报文数据
         * @param linkLength 链路地址长度
         * @param dataVec 如果已有转化为std::vector<int>对象的智能指针，则传入，否则缺省nullptr
         * @return 1 固定帧长报文
         * @return 2 可变帧长报文
         * @return -1 解析失败，未知报文
         * @version 2022-5-25
         */
        static int
        select101Type(const string &dataStr, int linkLength, const shared_ptr<vector<int>> &dataVec = nullptr);

        /**
         * @brief 获取101包括未知报文，固定帧长报文在内的所有报文类型的列表
         * @return std::vector<std::string>
         * @version 2022-5-25
         */
        static vector<string> getAllTypes();
    };

} // PktParse

#endif //TEST_MESSAGE_101_H
