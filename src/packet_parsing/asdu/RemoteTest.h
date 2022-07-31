//
// Created by Astria on 2022/4/25.
//

#ifndef REMOTE_TEST_H
#define REMOTE_TEST_H

#include "BaseASDU.h"

namespace asdu {

    struct NvaElem {
        size_t address{0ULL};
        long long qds{0LL};
        int value{0};
    };

    struct FloatElem {
        size_t address{0ULL};
        long long qds{0LL};
        float value{0.0};
    };

    class RemoteTest : public BaseASDU {
    private:
        long long _count{0};
        bool _sq{false};
        bool _isFloat{false};
        unique_ptr<vector<NvaElem>> _nVec;
        unique_ptr<vector<FloatElem>> _fVec;

    public:
        explicit RemoteTest(const ASDULengthSet &asdu_length_set);

        RemoteTest(const ASDULengthSet &asdu_length_set,
                   long long tie,
                   long long vsq,
                   long long cot,
                   size_t command_address);

        RemoteTest(unique_ptr<vector<int>> data, const ASDULengthSet &als);

        RemoteTest(const RemoteTest& rhs);

        ~RemoteTest() override = default;

        void set_tie(long long tie) override;

        void set_vsq(long long vsq) override;

        bool set_sq_addr(size_t addr);

        bool add_fElem(const FloatElem &elem);

        bool add_fElems(const vector<FloatElem> &elems);

        bool add_fElems(const shared_ptr<vector<FloatElem>> &elems);

        bool add_nElem(const NvaElem &elem);

        bool add_nElems(const vector<NvaElem> &elems);

        bool add_nElems(const shared_ptr<vector<NvaElem>> &elems);

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        RemoteTest& operator=(const RemoteTest& rhs);
    };

} // asdu

#endif //REMOTE_TEST_H
