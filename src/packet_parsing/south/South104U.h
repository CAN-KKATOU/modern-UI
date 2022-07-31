//
// Created by Astria on 2022/7/6.
//

#ifndef ANALYSIS_CORE_C__17_SOUTH104U_H
#define ANALYSIS_CORE_C__17_SOUTH104U_H

#include "South104.h"

namespace PktParse {

    class South104U : public South104 {
    private:
        bool bOrder{false};
        bool bTest{false};
        bool bStop{false};
        bool bStart{false};

    public:
        explicit South104U(const LengthSet &lengthSet);

        South104U(const LengthSet &lengthSet, Packet104UOrder order, bool isOrder);

        South104U(const string &dataStr, const LengthSet &lengthSet);

        South104U(const South104U& rhs);

        ~South104U() override = default;

        South104U& operator=(const South104U& rhs);

        long long get_tie() const override { return -3; }

        long long get_vsq() const override { return -1; }

        long long get_cot() const override { return -1; }

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        string getType() const override { return "U格式报文"; }

        void set_order(Packet104UOrder order, bool isOrder);

        bool is_order() const { return bOrder; }

        bool is_start() const { return bStart; }

        bool is_test() const { return bTest; }

        bool is_stop() const { return bStop; }
    };

} // PktParse

#endif //ANALYSIS_CORE_C__17_SOUTH104U_H
