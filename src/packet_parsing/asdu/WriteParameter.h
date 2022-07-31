//
// Created by Astria on 2022/4/25.
//

#ifndef WRITE_PARAMETER_H
#define WRITE_PARAMETER_H

#include "BaseASDU.h"

namespace asdu {

    struct WriteElem {
        size_t address{0};
        int tag{0};
        int value_length{0};
        string value; // 期望数据为16进制字符串，按报文顺序排列，中间用空格隔开
    };

    class WriteParameter : public BaseASDU {
    private:
        long long _sn{0LL};
        bool _dir{false};
        long long _count{0LL};
        int _features{0};
        bool _preset{false}; // 预置标志位
        unique_ptr<vector<WriteElem>> _oVec;

    public:
        explicit WriteParameter(const ASDULengthSet& asdu_length_set);

        WriteParameter(const ASDULengthSet &asdu_length_set,
                       long long tie,
                       long long vsq,
                       long long cot,
                       size_t command_address,
                       long long sn,
                       int features);

        WriteParameter(unique_ptr<vector<int>> data, const ASDULengthSet &als);

        WriteParameter(const WriteParameter& rhs);

        ~WriteParameter() override = default;

        void set_vsq(long long vsq) override;

        void set_sn(long long sn);

        void set_features(int features);

        bool add_elem(const WriteElem& elem);

        bool add_elems(const vector<WriteElem>& elems);

        bool add_elems(const shared_ptr<vector<WriteElem>>& elems);

        WriteParameter& operator=(const WriteParameter& rhs);

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;
    };

} // asdu

#endif //WRITE_PARAMETER_H
