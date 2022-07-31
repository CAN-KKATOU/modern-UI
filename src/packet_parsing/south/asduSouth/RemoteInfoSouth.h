//
// Created by Astria on 2022/4/25.
//

#ifndef TEST_REMOTE_INFO_FS_H
#define TEST_REMOTE_INFO_FS_H

#include "../../asdu/BaseASDU.h"

namespace ASDUSouth {

    struct InfoElem {
        size_t address{0ULL};
        long long quality{0LL};
        std::string time;
    };

    class RemoteInfoSouth : public asdu::BaseASDU {
    private:
        long long _count{0LL};
        bool _sq{false};
        bool _single{false};
        bool _time{false};
        std::unique_ptr<std::vector<InfoElem>> _eVec;

    public:
        explicit RemoteInfoSouth(const asdu::ASDULengthSet& asdu_length_set);

        RemoteInfoSouth(const asdu::ASDULengthSet &asdu_length_set,
                        long long tie,
                        long long vsq,
                        long long cot,
                        size_t command_address);

        RemoteInfoSouth(std::unique_ptr<std::vector<int>> data, const asdu::ASDULengthSet &als);

        RemoteInfoSouth(const RemoteInfoSouth& rhs);

        ~RemoteInfoSouth() override = default;

        RemoteInfoSouth& operator=(const RemoteInfoSouth& rhs);

        void set_tie(long long tie) override;

        void set_vsq(long long vsq) override;

        bool set_sq_addr(size_t addr);

        void add_elem(const InfoElem &elem);

        void add_elems(const shared_ptr<vector<InfoElem>> &elems);

        void add_elems(const vector<InfoElem> &elems);

        std::shared_ptr<nlohmann::json> toJson() override;

        std::shared_ptr<nlohmann::json> getPosition() override;
    };

} // ASDUSouth

#endif //TEST_REMOTE_INFO_FS_H
