//
// Created by Astria on 2022/4/25.
//

#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H

#include "BaseASDU.h"

namespace asdu {

    class RemoteControl : public BaseASDU {
    private:
        long long order{0};
        bool bSingle{false};
        bool bSelect{false};
        bool scs{false};
        int dcs{0};

    public:
        explicit RemoteControl(const ASDULengthSet &asduLengthSet);

        RemoteControl(const ASDULengthSet &asduLengthSet,
                      long long tie,
                      long long vsq,
                      long long cot,
                      size_t commandAddress,
                      size_t objectAddress,
                      long long order = 0);

        RemoteControl(unique_ptr<vector<int>> dataVec, const ASDULengthSet &asduLengthSet);

        RemoteControl(const RemoteControl& rhs);

        ~RemoteControl() override = default;

        void set_object_address(size_t objectAddress);

        size_t get_object_address() const { return oAddr; }

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        long long get_order() const { return order; }

        bool isSelect() const { return bSelect; }

        bool isClose() const;

        bool isSingle() const { return bSingle; }

        RemoteControl& operator=(const RemoteControl& rhs);
    };

} // asdu

#endif //REMOTE_CONTROL_H
