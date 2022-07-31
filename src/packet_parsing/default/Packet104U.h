//
// Created by Astria on 2022/4/24.
//

#ifndef TEST_MESSAGE_104_U_H
#define TEST_MESSAGE_104_U_H

#include "Packet104.h"

namespace PktParse {

    class Packet104U : public Packet104 {
    private:
        bool bOrder{false};
        bool bTest{false};
        bool bStop{false};
        bool bStart{false};

    public:
        explicit Packet104U(const LengthSet &lengthSet);

        Packet104U(const LengthSet &lengthSet, Packet104UOrder order, bool isOrder);

        Packet104U(const string &dataStr, const LengthSet &lengthSet);

        Packet104U(const Packet104U &rhs);

        ~Packet104U() override = default;

        Packet104U &operator=(const Packet104U &rhs);

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

#endif //TEST_MESSAGE_104_U_H
