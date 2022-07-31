//
// Created by Astria on 2022/4/25.
//

#ifndef TEST_RESET_PROCESS_FS_H
#define TEST_RESET_PROCESS_FS_H

#include "../../asdu/BaseASDU.h"

namespace ASDUSouth {

    class ResetProcessSouth : public asdu::BaseASDU {
    private:
        long long _qrp{0LL};

    public:
        explicit ResetProcessSouth(const asdu::ASDULengthSet& asdu_length_set);

        ResetProcessSouth(const asdu::ASDULengthSet& asdu_length_set,
                          long long tie,
                          long long vsq,
                          long long cot,
                          size_t command_address,
                          size_t object_address,
                          long long qrp = 1);

        ResetProcessSouth(std::unique_ptr<std::vector<int>> data, const asdu::ASDULengthSet &als);

        ResetProcessSouth(const ResetProcessSouth& rhs);

        ~ResetProcessSouth() override = default;

        ResetProcessSouth& operator=(const ResetProcessSouth& rhs);

        void set_object_address(long long object_address);

        std::shared_ptr<nlohmann::json> toJson() override;

        std::shared_ptr<nlohmann::json> getPosition() override;
    };

} // ASDUSouth

#endif //TEST_RESET_PROCESS_FS_H
