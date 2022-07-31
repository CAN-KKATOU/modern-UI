//
// Created by Astria on 2022/4/25.
//

#ifndef INITIALIZED_H
#define INITIALIZED_H

#include "BaseASDU.h"

namespace asdu {

    class Initialized : public BaseASDU {
    private:
        long long coi{0LL};

    public:
        explicit Initialized(const ASDULengthSet &asduLengthSet);

        Initialized(const ASDULengthSet &asduLengthSet,
                    long long tie,
                    long long vsq,
                    long long cot,
                    size_t commandAddress,
                    size_t objectAddress,
                    long long coi = 0);

        Initialized(unique_ptr<vector<int>> dataStr, const ASDULengthSet &asduLengthSet);

        Initialized(const Initialized& rhs);

        ~Initialized() override = default;

        void set_object_address(long long objectAddress);

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        Initialized& operator=(const Initialized& rhs);

        long long get_coi() const { return coi; }
    };

} // asdu

#endif //INITIALIZED_H
