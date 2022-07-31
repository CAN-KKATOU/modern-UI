//
// Created by Astria on 2022/4/24.
//

#ifndef BASE_ASDU_H
#define BASE_ASDU_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "../util.h"
#include "../nlohmann/json.hpp"

namespace asdu {

    using std::make_shared;
    using std::move;
    using std::string;
    using std::vector;
    using std::unique_ptr;
    using std::shared_ptr;
    using std::tuple;
    using nlohmann::json;

#if __cplusplus >= 201403L
    using std::make_unique;
#endif

    struct ASDULengthSet {
        int startIndex{7};
        int type{1};
        int commonAd{2};
        int cot{2};
        int objectAd{2};
        int qualifier{1};
        int describer{1};
        int sn{2};
    };

    class BaseASDU {
    protected:
        ASDULengthSet asduLength;
        long long tie{0LL};
        long long vsq{0LL};
        long long cot{0LL};
        size_t cAddr{0ULL};
        size_t oAddr{0ULL};
        string errInfo;
        unique_ptr<vector<int>> data;

    public:
        explicit BaseASDU(const ASDULengthSet &asduLengthSet);

        BaseASDU(const ASDULengthSet &asduLengthSet, long long tie, long long vsq, long long cot,
                 size_t commandAddress);

        BaseASDU(const ASDULengthSet &asduLengthSet, long long tie, long long vsq, long long cot,
                 size_t commandAddress, size_t objectAddress);

        BaseASDU(unique_ptr<vector<int>> dataVec, const ASDULengthSet &asduLengthSet);

        BaseASDU(const BaseASDU &rhs);

        virtual ~BaseASDU() = default;

        virtual void set_tie(long long tie);

        virtual void set_vsq(long long vsq);

        virtual void set_cot(long long cot);

        void set_command_address(size_t commandAddress);

        size_t get_command_address() const { return cAddr; }

        long long get_tie() const { return tie; }

        long long get_vsq() const { return vsq; }

        long long get_cot() const { return cot; }

        virtual shared_ptr<json> toJson() = 0;

        virtual shared_ptr<json> getPosition() = 0;

        string toString();

        string getPositionStr();

        string get_error_info() const { return errInfo; }

        vector<int> get_vec() const { return *data; }

        size_t getAsduLength() const { return data->size(); }

        long long getParity() const;

        BaseASDU &operator=(const BaseASDU &rhs);
    };

} // asdu

#endif //BASE_ASDU_H
