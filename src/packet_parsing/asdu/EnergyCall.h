//
// Created by Astria on 2022/4/25.
//

#ifndef ENERGY_CALL_H
#define ENERGY_CALL_H

#include "BaseASDU.h"

namespace asdu {

    class EnergyCall : public BaseASDU {
    private:
        long long _qcc{0LL};

    public:
        explicit EnergyCall(const ASDULengthSet &asdu_length_set);

        EnergyCall(const ASDULengthSet &asdu_length_set,
                   long long tie,
                   long long vsq,
                   long long cot,
                   size_t command_address,
                   size_t object_address,
                   long long qcc = 5);

        EnergyCall(unique_ptr<vector<int>> data, const ASDULengthSet &als);

        EnergyCall(const EnergyCall& rhs);

        ~EnergyCall() override = default;

        void set_object_address(long long object_address);

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        EnergyCall& operator=(const EnergyCall& rhs);
    };

} // asdu

#endif //ENERGY_CALL_H
