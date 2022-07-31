//
// Created by Astria on 2022/7/6.
//

#ifndef ANALYSIS_CORE_C__17_SOUTHFIXED_H
#define ANALYSIS_CORE_C__17_SOUTHFIXED_H

#include "South101.h"

namespace PktParse {

    class SouthFixed : public South101 {
    public:
        explicit SouthFixed(const LengthSet &lengthSet);

        SouthFixed(const LengthSet &lengthSet, int controlArea, size_t addressArea);

        SouthFixed(const LengthSet &lengthSet, bool dir, bool prm, bool fcb, bool fcv, bool acd, bool dfc, int fc,
                   size_t addressArea);

        SouthFixed(const string &dataStr, const LengthSet &lengthSet);

        SouthFixed(const SouthFixed &rhs);

        ~SouthFixed() override = default;

        SouthFixed &operator=(const SouthFixed &rhs);

        long long get_tie() const override { return -1; }

        long long get_vsq() const override { return -1; }

        long long get_cot() const override { return -1; }

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        string getType() const override { return "固定帧长报文"; }

        size_t pktLength() const override { return 1 + length.l_addr; }
    };

} // PktParse

#endif //ANALYSIS_CORE_C__17_SOUTHFIXED_H
