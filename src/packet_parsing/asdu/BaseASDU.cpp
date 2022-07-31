//
// Created by Astria on 2022/4/24.
//

#include "BaseASDU.h"

#include <memory>
#include <utility>

namespace asdu {
    BaseASDU::BaseASDU(const ASDULengthSet &asduLengthSet) : asduLength(asduLengthSet) {
#if __cplusplus >= 201403L
        data = std::make_unique<vector<int>>();
#elif __cplusplus < 201433L
        data = unique_ptr<vector<int>>(new vector<int>);
#endif
        for (int i = 1; i <= asduLength.type + asduLength.qualifier + asduLength.cot + asduLength.commonAd; i++)
            data->push_back(0);
    }

    BaseASDU::BaseASDU(const ASDULengthSet &asduLengthSet, long long int tie, long long int vsq, long long int cot,
                       size_t commandAddress)
            : asduLength(asduLengthSet), tie(tie), vsq(vsq), cot(cot), cAddr(commandAddress) {
#if __cplusplus >= 201403L
        data = make_unique<vector<int>>();
#elif __cplusplus < 201433L
        data = unique_ptr<vector<int>>(new vector<int>);
#endif

        util::pushElems(data, asduLength.type, tie);
        util::pushElems(data, asduLength.qualifier, vsq);
        util::pushElems(data, asduLength.cot, cot);
        util::pushElems(data, asduLength.commonAd, cAddr);
    }

    BaseASDU::BaseASDU(const ASDULengthSet &asduLengthSet, long long int tie, long long int vsq, long long int cot,
                       size_t commandAddress, size_t objectAddress)
            : BaseASDU(asduLengthSet, tie, vsq, cot, commandAddress) {
        oAddr = objectAddress;
        util::pushElems(data, asduLength.objectAd, oAddr);
    }

    BaseASDU::BaseASDU(unique_ptr<vector<int>> dataVec, const ASDULengthSet &asduLengthSet)
            : data(move(dataVec)), asduLength(asduLengthSet) {
        size_t cIndex = 0;
        try {
            tie = util::vecToLL(dataVec, cIndex, asduLength.type);
            cIndex += asduLength.type;

            vsq = util::vecToLL(dataVec, cIndex, asduLength.qualifier);
            cIndex += asduLength.qualifier;

            cot = util::vecToLL(dataVec, cIndex, asduLength.cot);
            cIndex += asduLength.cot;

            cAddr = util::vecToULL(dataVec, cIndex, asduLength.commonAd);
            cIndex += asduLength.commonAd;

            oAddr = util::vecToULL(dataVec, cIndex, asduLength.objectAd);
        }
        catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
        }
        catch (const std::out_of_range &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void BaseASDU::set_tie(long long int tie) {
        tie = tie;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    void BaseASDU::set_vsq(long long int vsq) {
        vsq = vsq;
        size_t start_index = asduLength.type;
        util::replaceElems(data, start_index, asduLength.qualifier, vsq);
    }

    void BaseASDU::set_cot(long long int cot) {
        cot = cot;
        size_t start_index = asduLength.type + asduLength.qualifier;
        util::replaceElems(data, start_index, asduLength.cot, cot);
    }

    void BaseASDU::set_command_address(size_t commandAddress) {
        cAddr = commandAddress;
        size_t start_index = asduLength.type + asduLength.qualifier + asduLength.cot;
        util::replaceElems(data, start_index, asduLength.commonAd, cAddr);
    }

    std::string BaseASDU::toString() {
        return toJson()->dump();
    }

    std::string BaseASDU::getPositionStr() {
        return getPosition()->dump();
    }

    BaseASDU::BaseASDU(const BaseASDU &rhs)
            : asduLength(rhs.asduLength),
              tie(rhs.tie),
              vsq(rhs.vsq),
              cot(rhs.cot),
              cAddr(rhs.cAddr),
              oAddr(rhs.oAddr),
              errInfo(rhs.errInfo) {
#if __cplusplus >= 201403L
        data = std::make_unique<vector<int>>();
#elif __cplusplus < 201433L
        data = unique_ptr<vector<int>>(new vector<int>);
#endif
        if (rhs.data)
            data->assign(rhs.data->cbegin(), rhs.data->cend());
        else
            std::cerr << "BaseASDU(const BaseASDU&): data is nullptr." << std::endl;
    }

    BaseASDU &BaseASDU::operator=(const BaseASDU &rhs) {
        if (this == &rhs)
            return *this;

        asduLength = rhs.asduLength;
        tie = rhs.tie;
        vsq = rhs.vsq;
        cot = rhs.cot;
        cAddr = rhs.cAddr;
        oAddr = rhs.oAddr;
        errInfo = rhs.errInfo;
        if (!data) {
#if __cplusplus >= 201403L
            data = std::make_unique<vector<int>>();
#elif __cplusplus < 201433L
            data = unique_ptr<vector<int>>(new vector<int>);
#endif
        }


        if (rhs.data)
            data->assign(rhs.data->cbegin(), rhs.data->cend());
        else
            std::cerr << "BaseASDU(const BaseASDU&): data is nullptr." << std::endl;
        return *this;
    }

    long long BaseASDU::getParity() const {
        if (data) {
            long long res{0ULL};
            for (const auto& val : *data)
                res += val;
            return res;
        } else
            return -1;
    }
} // asdu
