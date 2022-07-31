//
// Created by Astria on 2022/4/25.
//

#ifndef REMOTE_INFO_H
#define REMOTE_INFO_H

#include "BaseASDU.h"

namespace asdu {
    struct InfoElem {
        size_t address{0ULL};
        long long quality{0LL};
        string time;
    };

    class RemoteInfo : public BaseASDU {
    private:
        long long count{0LL};
        bool bSQ{false};
        bool bSingle{false};
        bool bTime{false};
        unique_ptr<vector<InfoElem>> elemVec;

    public:
        explicit RemoteInfo(const ASDULengthSet &asduLengthSet);

        RemoteInfo(const ASDULengthSet &asduLengthSet,
                   long long tie,
                   long long vsq,
                   long long cot,
                   size_t commandAddress);

        RemoteInfo(unique_ptr<vector<int>> dataVec, const ASDULengthSet &asduLengthSet);

        RemoteInfo(const RemoteInfo& rhs);

        ~RemoteInfo() override = default;

        void set_tie(long long tie) override;

        void set_vsq(long long vsq) override;

        /**
         * @brief 当报文SQ=1时设置首地址
         * @param addr
         * @return
         */
        bool set_sq_addr(size_t addr);

        void add_elem(const InfoElem &elem);

        void add_elems(const shared_ptr<vector<InfoElem>> &elems);

        void add_elems(const vector<InfoElem> &elems);

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        RemoteInfo& operator=(const RemoteInfo& rhs);
    };

} // asdu

#endif //REMOTE_INFO_H
