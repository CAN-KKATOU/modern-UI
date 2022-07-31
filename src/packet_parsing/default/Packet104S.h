//
// Created by Astria on 2022/4/24.
//

#ifndef TEST_MESSAGE_104_S_H
#define TEST_MESSAGE_104_S_H

#include "Packet104.h"

namespace PktParse {

    class Packet104S : public Packet104 {
    private:
        size_t rNum {0ULL };

    public:
        explicit Packet104S(const LengthSet& lengthSet);

        Packet104S(const LengthSet& lengthSet, int rNum);

        Packet104S(const string &dataStr, const LengthSet &lengthSet);

        Packet104S(const Packet104S& rhs);

        ~Packet104S() override = default;

        Packet104S& operator=(const Packet104S& rhs);

        long long get_tie() const override { return -2; }

        long long get_vsq() const override { return -1; }

        long long get_cot() const override { return -1; }

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        string getType() const override { return "S格式报文"; }

        void set_receive_num(int rNum);

        size_t get_receive_num() const { return rNum; }
    };

} // PktParse

#endif //TEST_MESSAGE_104_S_H
