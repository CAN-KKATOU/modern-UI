//
// Created by Astria on 2022/7/6.
//

#ifndef ANALYSIS_CORE_C__17_SOUTH104_H
#define ANALYSIS_CORE_C__17_SOUTH104_H

#include "../BasePacket.h"

namespace PktParse {

    class South104 : public BasePacket {
    protected:
        size_t mLength{0ULL};
        int cArea1{0};
        int cArea2{0};
        int cArea3{0};
        int cArea4{0};

    public:
        explicit South104(const LengthSet& lengthSet) : BasePacket(lengthSet) {}

        South104(const string &dataStr, const LengthSet &lengthSet);

        South104(const South104& rhs);

        ~South104() override = default;

        South104& operator=(const South104& rhs);

        long long get_tie() const override = 0;

        long long get_vsq() const override = 0;

        long long get_cot() const override = 0;

        shared_ptr<json> toJson() override = 0;

        shared_ptr<json> getPosition() override = 0;

        string getType() const override = 0;

        size_t pktLength() const override { return mLength; }

        static string parse104Type(const string &dataStr, int typeLength);

        static int parse104TypeInt(const string &dataStr, int typeLength);

        static int select104Type(const string &dataStr, const shared_ptr<vector<int>> &dataVec = nullptr);

        static vector<string> getAllTypes();
    };

} // PktParse

#endif //ANALYSIS_CORE_C__17_SOUTH104_H
