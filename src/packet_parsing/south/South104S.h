//
// Created by Astria on 2022/7/6.
//

#ifndef ANALYSIS_CORE_C__17_SOUTH104S_H
#define ANALYSIS_CORE_C__17_SOUTH104S_H

#include "South104.h"

namespace PktParse {

    class South104S : public South104 {
    private:
        size_t rNum = 0;

    public:
        explicit South104S(const LengthSet& lengthSet);

        South104S(const LengthSet& lengthSet, int rNum);

        South104S(const string &dataStr, const LengthSet &lengthSet);

        South104S(const South104S& rhs);

        ~South104S() override = default;

        South104S& operator=(const South104S& rhs);

        long long get_tie() const override { return -2; }

        long long get_vsq() const override { return -1; }

        long long get_cot() const override { return -1; }

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        string getType() const override { return "S格式报文"; }
    };

} // PktParse

#endif //ANALYSIS_CORE_C__17_SOUTH104S_H
