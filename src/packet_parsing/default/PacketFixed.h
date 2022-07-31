//
// Created by Astria on 2022/4/24.
//

#ifndef TEST_FIXED_MESSAGE_H
#define TEST_FIXED_MESSAGE_H

#include "Packet101.h"

namespace PktParse {

    class PacketFixed : public Packet101 {
    public:
        explicit PacketFixed(const LengthSet &lengthSet);

        PacketFixed(const LengthSet &lengthSet, int controlArea, size_t addressArea);

        PacketFixed(const LengthSet &lengthSet, bool dir, bool prm, bool fcb, bool fcv, bool acd, bool dfc, int fc,
                    size_t addressArea);

        PacketFixed(const string &dataStr, const LengthSet &lengthSet);

        PacketFixed(const PacketFixed& rhs);

        ~PacketFixed() override = default;

        PacketFixed& operator=(const PacketFixed& rhs);

        long long get_tie() const override { return -1; }

        long long get_vsq() const override { return -1; }

        long long get_cot() const override { return -1; }

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        string getType() const override { return "固定帧长报文"; }

        size_t pktLength() const override { return 1 + length.l_addr; }
    };

} // PktParse

#endif //TEST_FIXED_MESSAGE_H
