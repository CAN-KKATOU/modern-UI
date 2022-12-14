//
// Created by Astria on 2022/7/6.
//

#include "South104I.h"

#include "asduSouth/GeneralCallSouth.h"
#include "../asdu/Clock.h"
#include "asduSouth/ResetProcessSouth.h"
#include "../asdu/Initialized.h"
#include "asduSouth/RemoteInfoSouth.h"
#include "../asdu/RemoteTest.h"
#include "../asdu/RemoteControl.h"
#include "asduSouth/Parameter.h"
#include "../asdu/ErrorEvent.h"
#include "../asdu/FileService.h"
#include "../asdu/EnergyCall.h"
#include "../asdu/EnergyData.h"
#include "../asdu/ChangeSN.h"
#include "../asdu/ReadCurrentSN.h"
#include "../asdu/ReadParameter.h"
#include "../asdu/WriteParameter.h"
#include "../asdu/SoftwareUpdate.h"

namespace PktParse {
    South104I::South104I(const string &data, const LengthSet &ls)
            : South104(data, ls) {
        if (length_flag) {
            asdu::ASDULengthSet set;
            set.startIndex = 6;
            set.type = length.type;
            set.commonAd = length.c_addr;
            set.cot = length.cot;
            set.objectAd = length.o_addr;
            set.qualifier = length.qualifier;
            set.describer = length.describer;
            set.sn = length.sn;

            sNum = (origin_data->at(3) * 256 + origin_data->at(2)) / 2;
            rNum = (origin_data->at(5) * 256 + origin_data->at(4)) / 2;
            aIndex = 6;

            string type_str;
            try {
                for (size_t index = aIndex + length.type; index > aIndex; index--)
                    type_str += util::decToHex(origin_data->at(index - 1), 2);

                tie = std::stoll(type_str, nullptr, 16);

#if __cplusplus >= 201403L
                auto asduData = unique_ptr<vector<int>>(new vector<int>);
                asduData->assign(origin_data->cbegin() + static_cast<long long>(aIndex), origin_data->cend());

                switch (tie) {
                    case 100:
                        aPtr = make_shared<ASDUSouth::GeneralCallSouth>(move(asduData), set);
                        break;
                    case 103:
                        aPtr = make_shared<asdu::Clock>(move(asduData), set);
                        break;
                    case 105:
                        aPtr = make_shared<ASDUSouth::ResetProcessSouth>(move(asduData), set);
                        break;
                    case 70:
                        aPtr = make_shared<asdu::Initialized>(move(asduData), set);
                        break;
                    case 1:
                    case 3:
                    case 30:
                    case 31:
                        aPtr = make_shared<ASDUSouth::RemoteInfoSouth>(move(asduData), set);
                        break;
                    case 9:
                    case 11:
                    case 13:
                        aPtr = make_shared<asdu::RemoteTest>(move(asduData), set);
                        break;
                    case 45:
                    case 46:
                        aPtr = make_shared<asdu::RemoteControl>(move(asduData), set);
                        break;
                    case 55:
                    case 108:
                        aPtr = make_shared<ASDUSouth::Parameter>(move(asduData), set);
                        break;
                    case 42:
                        aPtr = make_shared<asdu::ErrorEvent>(move(asduData), set);
                        break;
                    case 210:
                        aPtr = make_shared<asdu::FileService>(move(asduData), set);
                        break;
                    case 101:
                        aPtr = make_shared<asdu::EnergyCall>(move(asduData), set);
                        break;
                    case 206:
                    case 207:
                        aPtr = make_shared<asdu::EnergyData>(move(asduData), set);
                        break;
                    case 200:
                        aPtr = make_shared<asdu::ChangeSN>(move(asduData), set);
                        break;
                    case 201:
                        aPtr = make_shared<asdu::ReadCurrentSN>(move(asduData), set);
                        break;
                    case 202:
                        aPtr = make_shared<asdu::ReadParameter>(move(asduData), set);
                        break;
                    case 203:
                        aPtr = make_shared<asdu::WriteParameter>(move(asduData), set);
                        break;
                    case 211:
                        aPtr = make_shared<asdu::SoftwareUpdate>(move(asduData), set);
                        break;
                    default:
                        aPtr = nullptr;
                        error_info += "TI?????????????????? ";
                        break;
                }
#elif __cplusplus < 201403L
                auto asduData = unique_ptr<vector<int>>(new vector<int>);
                asduData->assign(origin_data->cbegin() + static_cast<long long>(aIndex), origin_data->cend());

                switch (tie) {
                    case 100:
                        aPtr = make_shared<ASDUSouth::GeneralCallSouth>(move(asduData), set);
                        break;
                    case 103:
                        aPtr = make_shared<asdu::Clock>(move(asduData), set);
                        break;
                    case 105:
                        aPtr = make_shared<ASDUSouth::ResetProcessSouth>(move(asduData), set);
                        break;
                    case 70:
                        aPtr = make_shared<asdu::Initialized>(move(asduData), set);
                        break;
                    case 1:
                    case 3:
                    case 30:
                    case 31:
                        aPtr = make_shared<ASDUSouth::RemoteInfoSouth>(move(asduData), set);
                        break;
                    case 9:
                    case 11:
                    case 13:
                        aPtr = make_shared<asdu::RemoteTest>(move(asduData), set);
                        break;
                    case 45:
                    case 46:
                        aPtr = make_shared<asdu::RemoteControl>(move(asduData), set);
                        break;
                    case 55:
                    case 108:
                        aPtr = make_shared<ASDUSouth::Parameter>(move(asduData), set);
                        break;
                    case 42:
                        aPtr = make_shared<asdu::ErrorEvent>(move(asduData), set);
                        break;
                    case 210:
                        aPtr = make_shared<asdu::FileService>(move(asduData), set);
                        break;
                    case 101:
                        aPtr = make_shared<asdu::EnergyCall>(move(asduData), set);
                        break;
                    case 206:
                    case 207:
                        aPtr = make_shared<asdu::EnergyData>(move(asduData), set);
                        break;
                    case 200:
                        aPtr = make_shared<asdu::ChangeSN>(move(asduData), set);
                        break;
                    case 201:
                        aPtr = make_shared<asdu::ReadCurrentSN>(move(asduData), set);
                        break;
                    case 202:
                        aPtr = make_shared<asdu::ReadParameter>(move(asduData), set);
                        break;
                    case 203:
                        aPtr = make_shared<asdu::WriteParameter>(move(asduData), set);
                        break;
                    case 211:
                        aPtr = make_shared<asdu::SoftwareUpdate>(move(asduData), set);
                        break;
                    default:
                        aPtr = nullptr;
                        error_info += "TI?????????????????? ";
                        break;
                }
#endif
            }
            catch (const std::out_of_range &e) {
                std::cerr << e.what() << std::endl;
            }
            catch (const std::invalid_argument &e) {
                std::cerr << e.what() << std::endl;
            }
            catch (const std::bad_alloc &e) {
                std::cerr << e.what() << std::endl;
            }

            if (aPtr)
                error_info += aPtr->get_error_info();
        }
    }

    shared_ptr<json> South104I::toJson() {
        auto object = make_shared<json>();

        if (!time_str.empty())
            (*object)["????????????"] = time_str;

        if (length_flag) {
            (*object)["????????????"] = "104I????????????";
            (*object)["???????????????"] = sNum;
            (*object)["???????????????"] = rNum;

            if (aPtr)
                (*object)["ASDU??????"] = *aPtr->toJson();
            else (*object)["ASDU??????"] = "ASDU????????????";
        } else {
            (*object)["????????????"] = "??????????????????";
        }

        return object;
    }

    shared_ptr<json> South104I::getPosition() {
        auto object = make_shared<json>();

        if (length_flag) {
            (*object)["????????????"] = util::posJson(1, 2);
            (*object)["???????????????"] = util::posJson(2, 4);
            (*object)["???????????????"] = util::posJson(4, 6);

            if (aPtr) {
                (*object)["ASDU????????????"] = util::posJson(6, mLength + 2);
                (*object)["ASDU??????"] = *aPtr->getPosition();
            } else
                (*object)["ASDU??????"] = util::posJson(6, mLength + 2);
        } else {
            (*object)["????????????"] = util::posJson(0, origin_data->size() - 1);
        }

        return object;
    }

    string South104I::getType() const {
        string typeStr;
        switch (tie) {
            case 100:
                typeStr = "???????????????";
                break;
            case 103:
                typeStr = "????????????/????????????";
                break;
            case 105:
                typeStr = "????????????????????????";
                break;
            case 70:
                typeStr = "???????????????????????????";
                break;
            case 1:
            case 3:
            case 30:
            case 31:
                typeStr = "??????????????????";
                break;
            case 9:
            case 11:
            case 13:
                typeStr = "??????????????????";
                break;
            case 45:
            case 46:
                typeStr = "??????????????????";
                break;
            case 55:
                typeStr = "??????/??????????????????";
                break;
            case 108:
                typeStr = "???????????????";
                break;
            case 42:
                typeStr = "?????????????????????";
                break;
            case 210:
                typeStr = "??????????????????";
                break;
            case 101:
                typeStr = "?????????????????????";
                break;
            case 206:
            case 207:
                typeStr = "?????????????????????";
                break;
            case 200:
                typeStr = "?????????????????????";
                break;
            case 201:
                typeStr = "???????????????????????????";
                break;
            case 202:
                typeStr = "????????????????????????";
                break;
            case 203:
                typeStr = "????????????????????????";
                break;
            case 211:
                typeStr = "??????????????????";
                break;
            default:
                typeStr = "????????????";
                break;
        }
        return typeStr;
    }

    string South104I::parseIType(const string &dataStr, int typeLength, const shared_ptr<vector<int>> &dataVec) {
        auto data = make_shared<vector<int>>();

        if (dataVec == nullptr) {
            *data = util::splitToInt(dataStr, " ");
        } else data = dataVec;

        string tieStr;
        for (int index = 6 + typeLength; index > 6; index--) {
            try {
                tieStr += util::decToHex(data->at(index - 1), 2);
            }
            catch (const std::out_of_range &e) {
                std::cerr << e.what() << std::endl;
                return "????????????";
            }
        }
        try {
            int type = std::stoi(tieStr, nullptr, 16);
            string typeStr;
            switch (type) {
                case 100:
                    typeStr = "???????????????";
                    break;
                case 103:
                    typeStr = "????????????/????????????";
                    break;
                case 105:
                    typeStr = "????????????????????????";
                    break;
                case 70:
                    typeStr = "???????????????????????????";
                    break;
                case 1:
                case 3:
                case 30:
                case 31:
                    typeStr = "??????????????????";
                    break;
                case 9:
                case 11:
                case 13:
                    typeStr = "??????????????????";
                    break;
                case 45:
                case 46:
                    typeStr = "??????????????????";
                    break;
                case 55:
                    typeStr = "??????/??????????????????";
                    break;
                case 108:
                    typeStr = "???????????????";
                    break;
                case 42:
                    typeStr = "?????????????????????";
                    break;
                case 210:
                    typeStr = "??????????????????";
                    break;
                case 101:
                    typeStr = "?????????????????????";
                    break;
                case 206:
                case 207:
                    typeStr = "?????????????????????";
                    break;
                case 200:
                    typeStr = "?????????????????????";
                    break;
                case 201:
                    typeStr = "???????????????????????????";
                    break;
                case 202:
                    typeStr = "????????????????????????";
                    break;
                case 203:
                    typeStr = "????????????????????????";
                    break;
                case 211:
                    typeStr = "??????????????????";
                    break;
                default:
                    typeStr = "????????????";
                    break;
            }
            return typeStr;
        }
        catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
            return "????????????";
        }
    }

    long long South104I::get_tie() const {
        if (aPtr)
            return aPtr->get_tie();
        else
            return -1;
    }

    long long South104I::get_vsq() const {
        if (aPtr)
            return aPtr->get_vsq();
        else
            return -1;
    }

    long long South104I::get_cot() const {
        if (aPtr)
            return aPtr->get_cot();
        else
            return -1;
    }

    int South104I::parseITypeInt(const string &dataStr, int typeLength, const shared_ptr<vector<int>> &dataVec) {
        auto data = make_shared<vector<int>>();

        if (dataVec == nullptr) {
            *data = util::splitToInt(dataStr, " ");
        } else data = dataVec;

        try {
            return util::vecToInt(data, 6, typeLength);
        }
        catch (const std::out_of_range &e) {
            std::cerr << e.what() << std::endl;
            return -4;
        }
    }

    South104I::South104I(const LengthSet &lengthSet)
            : South104(lengthSet) {
        mLength = 4;
        aIndex = 6;

        origin_data->push_back(104);
        origin_data->push_back(6);
        origin_data->push_back(cArea1);
        origin_data->push_back(cArea2);
        origin_data->push_back(cArea3);
        origin_data->push_back(cArea4);
    }

    South104I::South104I(const LengthSet &lengthSet, size_t sendNum, size_t receiveNum,
                         shared_ptr<asdu::BaseASDU> asduPtr)
            : South104(lengthSet), sNum(sendNum), rNum(receiveNum), tie(asduPtr->get_tie()), aPtr(move(asduPtr)) {
        mLength = 4 + aPtr->getAsduLength();
        cArea1 = static_cast<int>((sNum % 128) * 2);
        cArea2 = static_cast<int>(sNum / 128);
        cArea3 = static_cast<int>((rNum % 128) * 2);
        cArea4 = static_cast<int>(rNum / 128);
        aIndex = 6;

        origin_data->push_back(104);
        origin_data->push_back(static_cast<int>(mLength));
        origin_data->push_back(cArea1);
        origin_data->push_back(cArea2);
        origin_data->push_back(cArea3);
        origin_data->push_back(cArea4);

        vector<int> temp = aPtr->get_vec();
        origin_data->insert(origin_data->cend(), temp.cbegin(), temp.cend());
    }

    South104I::South104I(const South104I &rhs)
            : South104(rhs), sNum(rhs.sNum), rNum(rhs.rNum), aIndex(rhs.aIndex), tie(rhs.tie) {
        if (rhs.aPtr) {
            switch (tie) {
                case 100: {
                    auto temp_ptr = std::dynamic_pointer_cast<ASDUSouth::GeneralCallSouth>(rhs.aPtr);
                    aPtr = make_shared<ASDUSouth::GeneralCallSouth>(*temp_ptr);
                    break;
                }
                case 103: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::Clock>(rhs.aPtr);
                    aPtr = make_shared<asdu::Clock>(*temp_ptr);
                    break;
                }
                case 105: {
                    auto temp_ptr = std::dynamic_pointer_cast<ASDUSouth::ResetProcessSouth>(rhs.aPtr);
                    aPtr = make_shared<ASDUSouth::ResetProcessSouth>(*temp_ptr);
                    break;
                }
                case 70: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::Initialized>(rhs.aPtr);
                    aPtr = make_shared<asdu::Initialized>(*temp_ptr);
                    break;
                }
                case 1:
                case 3:
                case 30:
                case 31: {
                    auto temp_ptr = std::dynamic_pointer_cast<ASDUSouth::RemoteInfoSouth>(rhs.aPtr);
                    aPtr = make_shared<ASDUSouth::RemoteInfoSouth>(*temp_ptr);
                    break;
                }
                case 9:
                case 11:
                case 13: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::RemoteTest>(rhs.aPtr);
                    aPtr = make_shared<asdu::RemoteTest>(*temp_ptr);
                    break;
                }
                case 45:
                case 46: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::RemoteControl>(rhs.aPtr);
                    aPtr = make_shared<asdu::RemoteControl>(*temp_ptr);
                    break;
                }
                case 55:
                case 108: {
                    auto temp_ptr = std::dynamic_pointer_cast<ASDUSouth::Parameter>(rhs.aPtr);
                    aPtr = make_shared<ASDUSouth::Parameter>(*temp_ptr);
                    break;
                }
                case 42: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::ErrorEvent>(rhs.aPtr);
                    aPtr = make_shared<asdu::ErrorEvent>(*temp_ptr);
                    break;
                }
                case 210: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::FileService>(rhs.aPtr);
                    aPtr = make_shared<asdu::FileService>(*temp_ptr);
                    break;
                }
                case 101: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::EnergyCall>(rhs.aPtr);
                    aPtr = make_shared<asdu::EnergyCall>(*temp_ptr);
                    break;
                }
                case 206:
                case 207: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::EnergyData>(rhs.aPtr);
                    aPtr = std::make_shared<asdu::EnergyData>(*temp_ptr);
                    break;
                }
                case 200: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::ChangeSN>(rhs.aPtr);
                    aPtr = make_shared<asdu::ChangeSN>(*temp_ptr);
                    break;
                }
                case 201: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::ReadCurrentSN>(rhs.aPtr);
                    aPtr = make_shared<asdu::ReadCurrentSN>(*temp_ptr);
                    break;
                }
                case 202: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::ReadParameter>(rhs.aPtr);
                    aPtr = make_shared<asdu::ReadParameter>(*temp_ptr);
                    break;
                }
                case 203: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::WriteParameter>(rhs.aPtr);
                    aPtr = make_shared<asdu::WriteParameter>(*temp_ptr);
                    break;
                }
                case 211: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::SoftwareUpdate>(rhs.aPtr);
                    aPtr = make_shared<asdu::SoftwareUpdate>(*temp_ptr);
                    break;
                }
                default:
                    aPtr = nullptr;
                    error_info += "TI?????????????????? ";
                    break;
            }
        }
    }

    South104I &South104I::operator=(const South104I &rhs) {
        if (this == &rhs)
            return *this;

        South104::operator=(rhs);
        sNum = rhs.sNum;
        rNum = rhs.rNum;
        aIndex = rhs.aIndex;
        tie = rhs.tie;

        if (rhs.aPtr) {
            switch (tie) {
                case 100: {
                    auto temp_ptr = std::dynamic_pointer_cast<ASDUSouth::GeneralCallSouth>(rhs.aPtr);
                    aPtr = make_shared<ASDUSouth::GeneralCallSouth>(*temp_ptr);
                    break;
                }
                case 103: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::Clock>(rhs.aPtr);
                    aPtr = make_shared<asdu::Clock>(*temp_ptr);
                    break;
                }
                case 105: {
                    auto temp_ptr = std::dynamic_pointer_cast<ASDUSouth::ResetProcessSouth>(rhs.aPtr);
                    aPtr = make_shared<ASDUSouth::ResetProcessSouth>(*temp_ptr);
                    break;
                }
                case 70: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::Initialized>(rhs.aPtr);
                    aPtr = make_shared<asdu::Initialized>(*temp_ptr);
                    break;
                }
                case 1:
                case 3:
                case 30:
                case 31: {
                    auto temp_ptr = std::dynamic_pointer_cast<ASDUSouth::RemoteInfoSouth>(rhs.aPtr);
                    aPtr = make_shared<ASDUSouth::RemoteInfoSouth>(*temp_ptr);
                    break;
                }
                case 9:
                case 11:
                case 13: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::RemoteTest>(rhs.aPtr);
                    aPtr = make_shared<asdu::RemoteTest>(*temp_ptr);
                    break;
                }
                case 45:
                case 46: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::RemoteControl>(rhs.aPtr);
                    aPtr = make_shared<asdu::RemoteControl>(*temp_ptr);
                    break;
                }
                case 55:
                case 108: {
                    auto temp_ptr = std::dynamic_pointer_cast<ASDUSouth::Parameter>(rhs.aPtr);
                    aPtr = make_shared<ASDUSouth::Parameter>(*temp_ptr);
                    break;
                }
                case 42: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::ErrorEvent>(rhs.aPtr);
                    aPtr = make_shared<asdu::ErrorEvent>(*temp_ptr);
                    break;
                }
                case 210: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::FileService>(rhs.aPtr);
                    aPtr = make_shared<asdu::FileService>(*temp_ptr);
                    break;
                }
                case 101: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::EnergyCall>(rhs.aPtr);
                    aPtr = make_shared<asdu::EnergyCall>(*temp_ptr);
                    break;
                }
                case 206:
                case 207: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::EnergyData>(rhs.aPtr);
                    aPtr = std::make_shared<asdu::EnergyData>(*temp_ptr);
                    break;
                }
                case 200: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::ChangeSN>(rhs.aPtr);
                    aPtr = make_shared<asdu::ChangeSN>(*temp_ptr);
                    break;
                }
                case 201: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::ReadCurrentSN>(rhs.aPtr);
                    aPtr = make_shared<asdu::ReadCurrentSN>(*temp_ptr);
                    break;
                }
                case 202: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::ReadParameter>(rhs.aPtr);
                    aPtr = make_shared<asdu::ReadParameter>(*temp_ptr);
                    break;
                }
                case 203: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::WriteParameter>(rhs.aPtr);
                    aPtr = make_shared<asdu::WriteParameter>(*temp_ptr);
                    break;
                }
                case 211: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::SoftwareUpdate>(rhs.aPtr);
                    aPtr = make_shared<asdu::SoftwareUpdate>(*temp_ptr);
                    break;
                }
                default:
                    aPtr = nullptr;
                    error_info += "TI?????????????????? ";
                    break;
            }
        }
        return *this;
    }

    bool South104I::set_asdu(shared_ptr<asdu::BaseASDU> asduPtr) {
        aPtr = move(asduPtr);
        mLength = 4 + aPtr->getAsduLength();

        origin_data->clear();
        origin_data->push_back(104);
        origin_data->push_back(static_cast<int>(mLength));
        origin_data->push_back(cArea1);
        origin_data->push_back(cArea2);
        origin_data->push_back(cArea3);
        origin_data->push_back(cArea4);

        vector<int> temp = aPtr->get_vec();
        origin_data->insert(origin_data->cend(), temp.begin(), temp.end());
        return true;
    }
} // PktParse
