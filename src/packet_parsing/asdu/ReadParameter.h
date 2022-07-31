//
// Created by Astria on 2022/4/25.
//

#ifndef READ_PARAMETER_H
#define READ_PARAMETER_H

#include "BaseASDU.h"

namespace asdu {
    struct ReadElem {
        size_t address{0ULL};
        int tag{0};
        int value_length{0};
        string value; // 期望数据为16进制字符串，按报文顺序排列，中间用空格隔开
    };

    class ReadParameter : public BaseASDU {
    private:
        long long _sn{0LL};
        long long _count{0LL};
        bool _dir{false}; // true为控制方向，false为监视方向
        bool is_read_all{false};
        int _features{0};

        unique_ptr<vector<ReadElem>> _oVec;
        unique_ptr<vector<long long>> _aVec;

    public:
        explicit ReadParameter(const ASDULengthSet& asdu_length_set);

        ReadParameter(const ASDULengthSet &asdu_length_set,
                      long long tie,
                      long long vsq,
                      long long cot,
                      size_t command_address,
                      long long sn);

        ReadParameter(unique_ptr<vector<int>> data, const ASDULengthSet &als);

        ReadParameter(const ReadParameter& rhs);

        ~ReadParameter() override = default;

        void set_vsq(long long vsq) override;

        void set_cot(long long cot) override;

        void set_sn(long long sn);

        bool set_features(int features);

        bool add_addr(long long addr);

        bool add_addrs(const vector<long long>& addrs);

        bool add_addrs(const shared_ptr<vector<long long>>& addrs);

        bool add_elem(const ReadElem& elem);

        bool add_elems(const vector<ReadElem>& elems);

        bool add_elems(const shared_ptr<vector<ReadElem>>& elems);

        ReadParameter& operator=(const ReadParameter& rhs);

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;
    };

} // asdu

#endif //READ_PARAMETER_H
