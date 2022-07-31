//
// Created by Astria on 2022/4/25.
//

#ifndef READ_CURRENT_SN_H
#define READ_CURRENT_SN_H

#include "BaseASDU.h"

namespace asdu {

    class ReadCurrentSN : public BaseASDU {
    private:
        bool _dir{false}; // true为控制方向，false为监视方向
        long long _cur_sn{0};
        long long _min_sn{0};
        long long _max_sn{0};

    public:
        explicit ReadCurrentSN(const ASDULengthSet &asdu_length_set);

        ReadCurrentSN(const ASDULengthSet &asdu_length_set,
                      long long tie,
                      long long vsq,
                      long long cot,
                      size_t command_address,
                      size_t object_address,
                      long long current_sn = 0,
                      long long min_sn = 0,
                      long long max_sn = 0);

        ReadCurrentSN(unique_ptr<vector<int>> data, const ASDULengthSet &als);

        ReadCurrentSN(const ReadCurrentSN& rhs);

        ~ReadCurrentSN() override = default;

        void set_cot(long long cot) override;

        void set_object_address(long long object_address);

        bool set_cur_sn(long long sn);

        bool set_min_sn(long long sn);

        bool set_max_sn(long long sn);

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        ReadCurrentSN& operator=(const ReadCurrentSN& rhs);
    };

} // asdu

#endif //READ_CURRENT_SN_H
