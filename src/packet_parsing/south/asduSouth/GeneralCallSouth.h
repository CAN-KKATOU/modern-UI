//
// Created by Astria on 2022/4/25.
//

#ifndef TEST_GENERAL_CALL_FS_H
#define TEST_GENERAL_CALL_FS_H

#include "../../asdu/BaseASDU.h"

namespace ASDUSouth {

    class GeneralCallSouth : public asdu::BaseASDU {
    private:
        long long _qoi{0LL};

    public:
        explicit GeneralCallSouth(const asdu::ASDULengthSet &asdu_length_set);

        GeneralCallSouth(const asdu::ASDULengthSet &asdu_length_set,
                         long long tie,
                         long long vsq,
                         long long cot,
                         size_t command_address,
                         size_t object_address,
                         long long qoi = 20);

        GeneralCallSouth(std::unique_ptr<std::vector<int>> data, const asdu::ASDULengthSet &als);

        GeneralCallSouth(const GeneralCallSouth& rhs);

        ~GeneralCallSouth() override = default;

        void set_object_address(long long object_address);

        GeneralCallSouth& operator=(const GeneralCallSouth& rhs);

        std::shared_ptr<nlohmann::json> toJson() override;

        std::shared_ptr<nlohmann::json> getPosition() override;
    };

} // ASDUSouth

#endif //TEST_GENERAL_CALL_FS_H
