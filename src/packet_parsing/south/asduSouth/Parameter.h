//
// Created by Astria on 2022/4/25.
//

#ifndef TEST_PARAMETER_H
#define TEST_PARAMETER_H

#include "../../asdu/BaseASDU.h"

namespace ASDUSouth {

    struct ParaElem {
        size_t address{0ULL};
        float value{0.0};
        int QOS{0};
    };

    class Parameter : public asdu::BaseASDU {
    private:
        long long _count{0LL};
        bool _sq{false};
        bool _read{false}; // tie=55时为读取参数报文
        std::unique_ptr<std::vector<ParaElem>> _eVec;

    public:
        explicit Parameter(const asdu::ASDULengthSet &asdu_length_set);

        Parameter(const asdu::ASDULengthSet &asdu_length_set,
                  long long tie,
                  long long vsq,
                  long long cot,
                  size_t command_address);

        Parameter(std::unique_ptr<std::vector<int>> data, const asdu::ASDULengthSet &als);

        Parameter(const Parameter &rhs);

        ~Parameter() override = default;

        Parameter &operator=(const Parameter &rhs);

        void set_tie(long long tie) override;

        void set_vsq(long long vsq) override;

        void add_elem(const ParaElem &elem);

        void add_elems(const std::vector<ParaElem> &elems);

        void add_elems(const std::shared_ptr<std::vector<ParaElem>> &elems);

        std::shared_ptr<nlohmann::json> toJson() override;

        std::shared_ptr<nlohmann::json> getPosition() override;
    };

} // ASDUSouth

#endif //TEST_PARAMETER_H
