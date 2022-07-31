//
// Created by Astria on 2022/4/24.
//

#include "BasePacket.h"

namespace PktParse {

    LengthSet::LengthSet(bool mode, int link, int type, int ca, int cot, int oa, int qua, int des, int sn)
            : mode(mode), l_addr(link), type(type), c_addr(ca), cot(cot), o_addr(oa), qualifier(qua),
              describer(des), sn(sn) {

    }

    LengthSet &LengthSet::operator=(const LengthSet &rls) {
        mode = rls.mode;
        l_addr = rls.l_addr;
        type = rls.type;
        c_addr = rls.c_addr;
        cot = rls.cot;
        o_addr = rls.o_addr;
        qualifier = rls.qualifier;
        describer = rls.describer;
        sn = rls.sn;
        return *this;
    }

    bool LengthSet::operator==(const LengthSet &rls) const {
        return (l_addr == rls.l_addr) and
               (type == rls.type) and
               (c_addr == rls.c_addr) and
               (cot == rls.cot) and
               (o_addr == rls.o_addr) and
               (qualifier == rls.qualifier) and
               (describer == rls.describer) and
               (sn == rls.sn);
    }

    bool LengthSet::operator!=(const LengthSet &rls) const {
        return !(*this == rls);
    }

    void BasePacket::set_time_str(const string &timeStr) {
        time_str = timeStr;
    }

    string BasePacket::toString(bool format) {
        if (format)
            return toJson()->dump(4);
        else
            return toJson()->dump();
    }

    string BasePacket::getPositionStr(bool format) {
        if (format)
            return getPosition()->dump(4);
        else
            return getPosition()->dump();
    }

    BasePacket::BasePacket(const BasePacket &rhs)
            : error_info(rhs.error_info), length(rhs.length), length_flag(rhs.length_flag), time_str(rhs.time_str) {
#if __cplusplus >= 201403L
        origin_data = make_unique<vector<int>>();
#elif __cplusplus < 201403L
        origin_data = unique_ptr<vector<int>>(new vector<int>);
#endif
        if (rhs.origin_data)
            origin_data->assign(rhs.origin_data->cbegin(), rhs.origin_data->cend());
        else
            std::cerr << "BasePacket(const BasePacket&): origin_data is nullptr." << std::endl;
    }

    BasePacket &BasePacket::operator=(const BasePacket &rhs) {
        if (this == &rhs)
            return *this;

        error_info = rhs.error_info;
        length = rhs.length;
        length_flag = rhs.length_flag;
        time_str = rhs.time_str;
        if (!origin_data) {
#if __cplusplus >= 201403L
            origin_data = make_unique<vector<int>>();
#elif __cplusplus < 201403L
            origin_data = unique_ptr<vector<int>>(new vector<int>);
#endif
        }
        if (rhs.origin_data)
            origin_data->assign(rhs.origin_data->cbegin(), rhs.origin_data->cend());
        else
            std::cerr << "BasePacket(const BasePacket&): origin_data is nullptr." << std::endl;
        return *this;
    }

    std::ostream &operator<<(std::ostream &cout, BasePacket &basePacket) {
        cout << basePacket.toString(true) << std::endl;
        return cout;
    }

    string BasePacket::toPkt() {
        string res;
        for (const auto& val : *origin_data)
            res += util::decToHex(val, 2) + " ";

        return res;
    }
} // Message
