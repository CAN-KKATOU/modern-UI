//
// Created by Astria on 2022/4/25.
//

#ifndef GENERAL_CALL_H
#define GENERAL_CALL_H

#include "BaseASDU.h"

namespace asdu {

    class GeneralCall : public BaseASDU {
    private:
        long long qoi{0LL};

    public:
        explicit GeneralCall(const ASDULengthSet &asduLengthSet);

        GeneralCall(const ASDULengthSet &asduLengthSet,
                    long long tie,
                    long long vsq,
                    long long cot,
                    size_t commandAddress,
                    size_t objectAddress,
                    long long qoi = 20
        );

        GeneralCall(const GeneralCall& rhs);

        GeneralCall(unique_ptr<vector<int>> dataVec, const ASDULengthSet &aasduLengthSet);

        ~GeneralCall() override = default;

        void set_object_address(long long objectAddress);

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        long long get_qoi() const { return qoi; }

        GeneralCall& operator=(const GeneralCall& rhs);
    };

} // asdu

#endif //GENERAL_CALL_H
