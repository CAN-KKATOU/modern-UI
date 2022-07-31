//
// Created by Astria on 2022/4/25.
//

#ifndef ENERGY_DATA_H
#define ENERGY_DATA_H

#include "BaseASDU.h"

namespace asdu {

    struct EnergyElem {
        size_t address{0ULL};
        float value{0.0};
        long long qds{0LL};
        string time;
    };

    class EnergyData : public BaseASDU {
    private:
        long long _count{0LL};
        bool _sq{false};
        bool _time{false};
        unique_ptr<vector<EnergyElem>> _eVec;

    public:
        explicit EnergyData(const ASDULengthSet &asdu_length_set);

        EnergyData(const ASDULengthSet &asdu_length_set,
                   long long tie,
                   long long vsq,
                   long long cot,
                   size_t command_address);

        EnergyData(unique_ptr<vector<int>> data, const ASDULengthSet &als);

        EnergyData(const EnergyData& rhs);

        ~EnergyData() override = default;

        void set_tie(long long tie) override;

        void set_vsq(long long vsq) override;

        bool set_sq_addr(size_t addr);

        void add_elem(const EnergyElem &elem);

        void add_elems(const vector<EnergyElem> &elems);

        void add_elems(const shared_ptr<vector<EnergyElem>> &elems);

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        EnergyData& operator=(const EnergyData& rhs);
    };

} // asdu

#endif //ENERGY_DATA_H
