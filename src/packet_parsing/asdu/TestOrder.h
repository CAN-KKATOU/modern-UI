//
// Created by Astria on 2022/4/25.
//

#ifndef TEST_ORDER_H
#define TEST_ORDER_H

#include "BaseASDU.h"

namespace asdu {

    class TestOrder : public BaseASDU {
    private:
        int _fbp{0};

    public:
        explicit TestOrder(const ASDULengthSet &asdu_length_set);

        TestOrder(const ASDULengthSet &asdu_length_set,
                  long long tie,
                  long long vsq,
                  long long cot,
                  size_t command_address,
                  size_t object_address,
                  long long fbp = 21930
        );

        TestOrder(unique_ptr<vector<int>> data, const ASDULengthSet &als);

        TestOrder(const TestOrder &rhs);

        ~TestOrder() override = default;

        TestOrder &operator=(const TestOrder &rhs);

        void set_object_address(long long object_address);

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;
    };

} // asdu

#endif //TEST_ORDER_H
