//
// Created by Astria on 2022/4/25.
//

#ifndef SOFTWARE_UPDATE_H
#define SOFTWARE_UPDATE_H

#include "BaseASDU.h"

namespace asdu {

    class SoftwareUpdate : public BaseASDU {
    private:
        int _cType{0};

    public:
        explicit SoftwareUpdate(const ASDULengthSet& asdu_length_set);

        SoftwareUpdate(const ASDULengthSet &asdu_length_set,
                       long long tie,
                       long long vsq,
                       long long cot,
                       size_t command_address,
                       size_t object_address,
                       int cType = 0);

        SoftwareUpdate(unique_ptr<vector<int>> data, const ASDULengthSet &als);

        SoftwareUpdate(const SoftwareUpdate& rhs);

        ~SoftwareUpdate() override = default;

        SoftwareUpdate& operator=(const SoftwareUpdate& rhs);

        void set_object_address(long long object_address);

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;
    };

} // asdu

#endif //SOFTWARE_UPDATE_H
