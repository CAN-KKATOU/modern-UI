//
// Created by Astria on 2022/4/25.
//

#ifndef CHANGE_SN_H
#define CHANGE_SN_H

#include "BaseASDU.h"

namespace asdu {

    class ChangeSN : public BaseASDU {
    private:
        long long sn {0LL };

    public:
        explicit ChangeSN(const ASDULengthSet& asdu_length_set);

        ChangeSN(const ASDULengthSet& asdu_length_set,
                 long long tie,
                 long long vsq,
                 long long cot,
                 size_t command_address,
                 size_t object_address,
                 long long sn);

        ChangeSN(unique_ptr<vector<int>> data, const ASDULengthSet &als);

        ChangeSN(const ChangeSN& rhs);

        ~ChangeSN() override = default;

        void set_object_address(long long object_address);

        void set_sn(long long sn);

        ChangeSN& operator=(const ChangeSN& rhs);

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;
    };

} // asdu

#endif //CHANGE_SN_H
