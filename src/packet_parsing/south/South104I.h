//
// Created by Astria on 2022/7/6.
//

#ifndef ANALYSIS_CORE_C__17_SOUTH104I_H
#define ANALYSIS_CORE_C__17_SOUTH104I_H

#include "South104.h"
#include "../asdu/BaseASDU.h"

namespace PktParse {

    class South104I : public South104 {
    private:
        size_t sNum = 0;
        size_t rNum = 0;
        size_t aIndex = 0;
        long long tie = 0;
        shared_ptr<asdu::BaseASDU> aPtr;

    public:
        explicit South104I(const LengthSet &lengthSet);

        South104I(const LengthSet &lengthSet,
                  size_t sendNum,
                  size_t receiveNum,
                  shared_ptr<asdu::BaseASDU> asduPtr
        );

        South104I(const string &data, const LengthSet &length_set);

        South104I(const South104I &rhs);

        ~South104I() override = default;

        South104I &operator=(const South104I &rhs);

        bool set_asdu(shared_ptr<asdu::BaseASDU> asduPtr);

        long long get_tie() const override;

        long long get_vsq() const override;

        long long get_cot() const override;

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        string getType() const override;

        /**
        * @brief 解析佛山104协议I格式报文类型
        * @param dataStr 原始报文数据
        * @param typeLength 类型标识符长度
        * @param dataVec 如果已有转化为std::vector<int>对象的智能指针，则传入，否则缺省nullptr
        * @return 若解析成功，则返回对应报文类型，否则返回‘未知报文’
        * @version 2022-5-25
        */
        static string
        parseIType(const string &dataStr, int typeLength, const shared_ptr<vector<int>> &dataVec = nullptr);

        static int
        parseITypeInt(const std::string &dataStr, int typeLength, const shared_ptr<vector<int>> &dataVec = nullptr);
    };

} // PktParse

#endif //ANALYSIS_CORE_C__17_SOUTH104I_H
