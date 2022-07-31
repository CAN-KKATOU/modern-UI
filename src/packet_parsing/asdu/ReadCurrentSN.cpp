//
// Created by Astria on 2022/4/25.
//

#include "ReadCurrentSN.h"

namespace asdu {
    ReadCurrentSN::ReadCurrentSN(unique_ptr<vector<int>> data, const ASDULengthSet &als)
            : BaseASDU(move(data), als) {
        _dir = cot == 6;
        if (!_dir) {
            size_t cIndex{static_cast<size_t>(asduLength.type +
                                              asduLength.qualifier +
                                              asduLength.cot +
                                              asduLength.commonAd +
                                              asduLength.objectAd)};

            try {
                _cur_sn = util::vecToLL(data, cIndex, asduLength.sn);
                cIndex += asduLength.sn;

                _min_sn = util::vecToLL(data, cIndex, asduLength.sn);
                cIndex += asduLength.sn;

                _max_sn = util::vecToLL(data, cIndex, asduLength.sn);

            } catch (const std::invalid_argument &e) {
                std::cerr << e.what() << std::endl;
            }
            catch (const std::out_of_range &e) {
                std::cerr << e.what() << std::endl;
            }
        }

        switch (cot) {
            case 6:
            case 7:
            case 47:
                break;
            default:
                errInfo += "传送原因COT错误 ";
                break;
        }
    }

    shared_ptr<json> ReadCurrentSN::toJson() {
        auto object = make_shared<json>();
        (*object)["ASDU报文类型"] = "读当前定值区号报文";
        switch (cot) {
            case 6:
                (*object)["传送原因"] = "激活";
                (*object)["传送方向"] = "控制方向";
                break;
            case 7:
                (*object)["传送原因"] = "激活确认";
                (*object)["传送方向"] = "监视方向";
                break;
            case 47:
                (*object)["传送原因"] = "激活终止";
                (*object)["传送方向"] = "监视方向";
                break;
            default:
                (*object)["传送原因"] = "Data Error";
                break;
        }

        (*object)["公共地址"] = cAddr;
        (*object)["信息体地址"] = oAddr;

        if (!_dir) {
            (*object)["当前定值区区号"] = _cur_sn;
            (*object)["终端支持的最小定值区号"] = _min_sn;
            (*object)["终端支持的最大定值区号"] = _max_sn;
        }
        return object;
    }

    shared_ptr<json> ReadCurrentSN::getPosition() {
        auto object = make_shared<json>();

        size_t cIndex{static_cast<size_t>(asduLength.startIndex)};
        (*object)["ASDU报文类型"] = util::posJson(cIndex, cIndex + asduLength.type);

        cIndex += asduLength.type + asduLength.qualifier;
        (*object)["传送原因"] = util::posJson(cIndex, cIndex + asduLength.cot);
        (*object)["传送方向"] = util::posJson(cIndex, cIndex + asduLength.cot);

        cIndex += asduLength.cot;
        (*object)["公共地址"] = util::posJson(cIndex, cIndex + asduLength.commonAd);

        cIndex += asduLength.commonAd;
        (*object)["信息体地址"] = util::posJson(cIndex, cIndex + asduLength.objectAd);

        cIndex += asduLength.objectAd;

        if (!_dir) {
            (*object)["当前定值区区号"] = util::posJson(cIndex, cIndex + asduLength.sn);
            cIndex = asduLength.sn;

            (*object)["终端支持的最小定值区号"] = util::posJson(cIndex, cIndex + asduLength.sn);
            cIndex = asduLength.sn;

            (*object)["终端支持的最大定值区号"] = util::posJson(cIndex, cIndex + asduLength.sn);
        }
        return object;
    }

    ReadCurrentSN::ReadCurrentSN(const ASDULengthSet &asdu_length_set)
            : BaseASDU(asdu_length_set) {
        for (int i = 1; i <= asduLength.objectAd + asduLength.sn * 3; i++)
            data->push_back(0);

        tie = 201;
    }

    ReadCurrentSN::ReadCurrentSN(const ASDULengthSet &asdu_length_set, long long int tie, long long int vsq,
                                 long long int cot, size_t command_address, size_t object_address,
                                 long long int current_sn, long long int min_sn, long long int max_sn)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address, object_address),
              _cur_sn(current_sn), _min_sn(min_sn), _max_sn(max_sn) {
        _dir = cot == 6;
        if (!_dir) {
            util::pushElems(data, asduLength.sn, _cur_sn);
            util::pushElems(data, asduLength.sn, _min_sn);
            util::pushElems(data, asduLength.sn, _max_sn);
        }
    }

    void ReadCurrentSN::set_object_address(long long int object_address) {
        oAddr = object_address;
        size_t index{static_cast<size_t>(asduLength.type +
                                         asduLength.qualifier +
                                         asduLength.cot +
                                         asduLength.commonAd)};
        util::replaceElems(data, index, asduLength.objectAd, object_address);
    }

    bool ReadCurrentSN::set_cur_sn(long long int sn) {
        if (!_dir) {
            _cur_sn = sn;
            size_t index{static_cast<size_t>(asduLength.type +
                                             asduLength.qualifier +
                                             asduLength.cot +
                                             asduLength.commonAd +
                                             asduLength.objectAd)};
            if (data->size() < index + asduLength.sn)
                util::pushElems(data, asduLength.sn, _cur_sn);
            else
                util::replaceElems(data, index, asduLength.sn, _cur_sn);
            return true;
        } else
            return false;
    }

    bool ReadCurrentSN::set_min_sn(long long int sn) {
        if (!_dir) {
            _min_sn = sn;
            size_t index{static_cast<size_t>(asduLength.type +
                                             asduLength.qualifier +
                                             asduLength.cot +
                                             asduLength.commonAd +
                                             asduLength.objectAd +
                                             asduLength.sn)};
            if (data->size() < index + asduLength.sn)
                util::pushElems(data, asduLength.sn, _max_sn);
            else
                util::replaceElems(data, index, asduLength.sn, _min_sn);
            return true;
        } else
            return false;
    }

    bool ReadCurrentSN::set_max_sn(long long int sn) {
        if (!_dir) {
            _max_sn = sn;
            size_t index{static_cast<size_t>(asduLength.type +
                                             asduLength.qualifier +
                                             asduLength.cot +
                                             asduLength.commonAd +
                                             asduLength.objectAd +
                                             asduLength.sn * 2)};
            if (data->size() < index + asduLength.sn)
                util::pushElems(data, asduLength.sn, _max_sn);
            else
                util::replaceElems(data, index, asduLength.sn, _max_sn);
            return true;
        } else
            return false;
    }

    void ReadCurrentSN::set_cot(long long int cot) {
        BaseASDU::set_cot(cot);
        _dir = cot == 6;
    }

    ReadCurrentSN::ReadCurrentSN(const ReadCurrentSN &rhs) : BaseASDU(rhs), _dir(rhs._dir) {
        if (!_dir) {
            _cur_sn = rhs._cur_sn;
            _min_sn = rhs._min_sn;
            _max_sn = rhs._max_sn;
        }
    }

    ReadCurrentSN &ReadCurrentSN::operator=(const ReadCurrentSN &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);
        if (!_dir) {
            _cur_sn = rhs._cur_sn;
            _min_sn = rhs._min_sn;
            _max_sn = rhs._max_sn;
        }
        return *this;
    }
} // asdu