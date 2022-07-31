//
// Created by Astria on 2022/4/25.
//

#ifndef RESET_PROCESS_H
#define RESET_PROCESS_H

#include "BaseASDU.h"

namespace asdu {

    class ResetProcess : public BaseASDU {
    private:
        long long qrp {0LL };

    public:
        explicit ResetProcess(const ASDULengthSet& asduLengthSet);

        ResetProcess(const ASDULengthSet& asduLengthSet,
                     long long tie,
                     long long vsq,
                     long long cot,
                     size_t commandAddress,
                     size_t objectAddress,
                     long long qrp = 1);

        ResetProcess(unique_ptr<vector<int>> data, const ASDULengthSet &als);

        ResetProcess(const ResetProcess& rhs);

        ~ResetProcess() override = default;

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        void set_object_address(long long object_address);

        ResetProcess& operator=(const ResetProcess& rhs);
    };

} // asdu

#endif //RESET_PROCESS_H
