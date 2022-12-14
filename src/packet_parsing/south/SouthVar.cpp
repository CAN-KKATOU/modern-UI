//
// Created by Astria on 2022/7/6.
//

#include "SouthVar.h"

#include "asduSouth/GeneralCallSouth.h"
#include "../asdu/Clock.h"
#include "asduSouth/ResetProcessSouth.h"
#include "../asdu/Initialized.h"
#include "../asdu/TestOrder.h"
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
    SouthVar::SouthVar(const string &dataStr, const LengthSet &lengthSet)
            : South101(dataStr, lengthSet) {
        if (length_flag) {
            asdu::ASDULengthSet set;
            set.startIndex = 5 + length.l_addr;
            set.type = length.type;
            set.commonAd = length.c_addr;
            set.cot = length.cot;
            set.objectAd = length.o_addr;
            set.qualifier = length.qualifier;
            set.describer = length.describer;
            set.sn = length.sn;

            mLength = origin_data->at(1);
            aIndex = 5 + length.l_addr;

            string type_str;
            try {
                for (size_t index = aIndex + length.type; index > aIndex; index--)
                    type_str += util::decToHex(origin_data->at(index - 1), 2);

                tie = std::stoll(type_str, nullptr, 16);

#if __cplusplus >= 201403L
                auto asduData = unique_ptr<vector<int>>(new vector<int>);
                asduData->assign(origin_data->cbegin() + static_cast<long long>(aIndex), origin_data->cend() - 2);

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
                    case 104:
                        aPtr = make_shared<asdu::TestOrder>(move(asduData), set);
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
                asduData->assign(origin_data->cbegin() + static_cast<long long>(aIndex), origin_data->cend() - 2);

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
                    case 104:
                        aPtr = make_shared<asdu::TestOrder>(move(asduData), set);
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

    shared_ptr<json> SouthVar::toJson() {
        auto object = make_shared<json>();

        if (!time_str.empty())
            (*object)["????????????"] = time_str;

        if (length_flag) {
            (*object)["????????????"] = "101??????????????????";
            (*object)["??????"] = aArea;
            if (dir) {// ?????????????????????
                (*object)["??????"] = "????????????????????????";
                if (prm)
                    (*object)["??????"] = "?????????";
                else (*object)["??????"] = "?????????";
                if (dfc)
                    (*object)["???????????????????????????"] = "???";
                else (*object)["???????????????????????????"] = "???";
            } else {// ???????????????????????????????????????????????????
                if (prm) {// ????????????
                    if (tMode)
                        (*object)["??????"] = "????????????????????????";
                    else
                        (*object)["??????"] = "???????????????????????????";
                    (*object)["??????"] = "?????????";
                    if (fcv) {
                        (*object)["??????????????????"] = "???";
                        if (fcb)
                            (*object)["????????????"] = 1;
                        else (*object)["????????????"] = 0;
                    } else (*object)["??????????????????"] = "???";
                } else {// ???????????????????????????
                    (*object)["??????"] = "???????????????????????????";
                    (*object)["??????"] = "?????????";
                    if (acd)
                        (*object)["????????????1?????????????????????"] = "???";
                    else (*object)["????????????1?????????????????????"] = "???";
                    if (dfc)
                        (*object)["?????????????????????????????????"] = "???";
                    else (*object)["?????????????????????????????????"] = "???";
                }
            }

            (*object)["FC?????????"] = fc2string();

            if (aPtr)
                (*object)["ASDU??????"] = *aPtr->toJson();
            else (*object)["ASDU??????"] = "ASDU????????????";

        } else {
            (*object)["????????????"] = "??????????????????";
        }
        return object;
    }

    shared_ptr<json> SouthVar::getPosition() {
        auto object = make_shared<json>();

        if (length_flag) {
            int index = 0;
            (*object)["????????????"] = util::posJson(index, index + 1);
            index += 4;

            if (dir) {// ?????????????????????
                (*object)["??????"] = util::posJson(index, index + 1);
                if (prm)
                    (*object)["??????"] = util::posJson(index, index + 1);
                else (*object)["??????"] = util::posJson(index, index + 1);
                if (dfc)
                    (*object)["???????????????????????????"] = util::posJson(index, index + 1);
                else (*object)["???????????????????????????"] = util::posJson(index, index + 1);
            } else {// ???????????????????????????????????????????????????
                if (prm) {// ????????????
                    (*object)["??????"] = util::posJson(index, index + 1);
                    (*object)["??????"] = util::posJson(index, index + 1);
                    if (fcv) {
                        (*object)["??????????????????"] = util::posJson(index, index + 1);
                        if (fcb)
                            (*object)["????????????"] = util::posJson(index, index + 1);
                        else (*object)["????????????"] = util::posJson(index, index + 1);
                    } else
                        (*object)["??????????????????"] = util::posJson(index, index + 1);
                } else {// ???????????????????????????
                    (*object)["??????"] = util::posJson(index, index + 1);
                    (*object)["??????"] = util::posJson(index, index + 1);
                    if (acd)
                        (*object)["????????????1?????????????????????"] = util::posJson(index, index + 1);
                    else (*object)["????????????1?????????????????????"] = util::posJson(index, index + 1);
                    if (dfc)
                        (*object)["?????????????????????????????????"] = util::posJson(index, index + 1);
                    else (*object)["?????????????????????????????????"] = util::posJson(index, index + 1);
                }
            }

            (*object)["FC?????????"] = util::posJson(index, index + 1);
            index++;

            (*object)["??????"] = util::posJson(index, index + length.l_addr);
            index += length.l_addr;

            if (aPtr) {
                (*object)["ASDU????????????"] = util::posJson((size_t) index,
                                                             index + mLength - 1 - length.l_addr);
                (*object)["ASDU??????"] = *aPtr->getPosition();
            } else
                (*object)["ASDU??????"] = util::posJson((size_t) index,
                                                         index + mLength - 1 - length.l_addr);
        } else {
            (*object)["????????????"] = util::posJson((size_t) 0, origin_data->size() - 1);
        }
        return object;
    }

    string SouthVar::getType() const {
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
            case 104:
                typeStr = "??????????????????";
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

    size_t SouthVar::pktLength() const {
        return mLength;
    }

    string SouthVar::parseVarType(const string &dataStr, int linkLength, int typeLength,
                                  const shared_ptr<vector<int>> &dataVec) {
        auto data = make_shared<vector<int>>();

        if (dataVec == nullptr) {
            *data = util::splitToInt(dataStr, " ");
        } else
            data = dataVec;

        string tieStr;
        for (int index = 5 + linkLength + typeLength; index > 5 + linkLength; index--) {
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
                case 104:
                    typeStr = "??????????????????";
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

    long long SouthVar::get_tie() const {
        if (aPtr)
            return aPtr->get_tie();
        else
            return -1;
    }

    long long SouthVar::get_vsq() const {
        if (aPtr)
            return aPtr->get_vsq();
        else
            return -1;
    }

    long long SouthVar::get_cot() const {
        if (aPtr)
            return aPtr->get_cot();
        else
            return -1;
    }

    int SouthVar::parseVarTypeInt(const string &dataStr, int linkLength, int typeLength,
                                  const shared_ptr<vector<int>> &dataVec) {
        auto data = make_shared<vector<int>>();

        if (dataVec == nullptr) {
            *data = util::splitToInt(dataStr, " ");
        } else
            data = dataVec;

        try {
            return util::vecToInt(data, 5 + linkLength, typeLength);
        } catch (const std::out_of_range &e) {
            std::cerr << e.what() << std::endl;
            return -4;
        }
    }

    SouthVar::SouthVar(const LengthSet &lengthSet) : South101(lengthSet) {
        mLength = 1 + length.l_addr;
        aIndex = 5 + length.l_addr;

        origin_data->push_back(104);
        origin_data->push_back(1 + length.l_addr);
        origin_data->push_back(1 + length.l_addr);
        origin_data->push_back(104);
        origin_data->push_back(cArea);
        util::pushElems(origin_data, length.l_addr, aArea);

        // ?????????C????????????A???????????????????????????ASDU?????????????????????????????????
        int parity{0};
        for (int index{4}; index <= 4 + length.l_addr; index++)
            parity += origin_data->at(index);
        origin_data->push_back(parity % 256);

        origin_data->push_back(22);
    }

    SouthVar::SouthVar(const LengthSet &lengthSet, int controlArea, size_t addressArea, long long int type,
                       shared_ptr<asdu::BaseASDU> asduPtr)
            : South101(lengthSet, controlArea, addressArea), tie(type), aPtr(move(asduPtr)) {
        mLength = 1 + length.l_addr + aPtr->getAsduLength();
        aIndex = 5 + length.l_addr;

        origin_data->push_back(104);
        origin_data->push_back(static_cast<int>(mLength));
        origin_data->push_back(static_cast<int>(mLength));
        origin_data->push_back(104);
        origin_data->push_back(cArea);
        util::pushElems(origin_data, length.l_addr, aArea);

        vector<int> temp = aPtr->get_vec();
        origin_data->insert(origin_data->cend(), temp.cbegin(), temp.cend());

        // ?????????C????????????A???????????????????????????ASDU?????????????????????????????????
        long long parity{0LL};
        for (int index{4}; index <= 4 + length.l_addr; index++)
            parity += origin_data->at(index);
        parity += aPtr->getParity();
        origin_data->push_back(static_cast<int>(parity % 256));

        origin_data->push_back(22);
    }

    SouthVar::SouthVar(const LengthSet &lengthSet, bool dir, bool prm, bool fcb, bool fcv, bool acd,
                       bool dfc, int fc, size_t addressArea, long long int type,
                       shared_ptr<asdu::BaseASDU> asduPtr)
            : South101(lengthSet, dir, prm, fcb, fcv, acd, dfc, fc, addressArea), tie(type), aPtr(move(asduPtr)) {
        mLength = 1 + length.l_addr + aPtr->getAsduLength();
        aIndex = 5 + length.l_addr;

        origin_data->push_back(104);
        origin_data->push_back(static_cast<int>(mLength));
        origin_data->push_back(static_cast<int>(mLength));
        origin_data->push_back(104);
        origin_data->push_back(cArea);

        util::pushElems(origin_data, length.l_addr, aArea);

        vector<int> temp = aPtr->get_vec();
        origin_data->insert(origin_data->cend(), temp.cbegin(), temp.cend());

        long long parity{0LL};
        for (int index{4}; index <= 4 + length.l_addr; index++)
            parity += origin_data->at(index);
        parity += aPtr->getParity();
        origin_data->push_back(static_cast<int>(parity % 256));

        origin_data->push_back(22);
    }

    SouthVar::SouthVar(const SouthVar &rhs)
            : South101(rhs), tie(rhs.tie), mLength(rhs.mLength), aIndex(rhs.aIndex) {
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
                case 104: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::TestOrder>(rhs.aPtr);
                    aPtr = make_shared<asdu::TestOrder>(*temp_ptr);
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

    SouthVar &SouthVar::operator=(const SouthVar &rhs) {
        if (this == &rhs)
            return *this;

        South101::operator=(rhs);
        mLength = rhs.mLength;
        tie = rhs.tie;
        aIndex = rhs.aIndex;
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
                case 104: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::TestOrder>(rhs.aPtr);
                    aPtr = make_shared<asdu::TestOrder>(*temp_ptr);
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

    bool SouthVar::set_asdu(shared_ptr<asdu::BaseASDU> asduPtr) {
        aPtr = move(asduPtr);
        mLength = 1 + length.l_addr + aPtr->getAsduLength();

        origin_data->clear();
        origin_data->push_back(104);
        origin_data->push_back(static_cast<int>(mLength));
        origin_data->push_back(static_cast<int>(mLength));
        origin_data->push_back(104);
        origin_data->push_back(cArea);

        util::pushElems(origin_data, length.l_addr, aArea);

        vector<int> temp = aPtr->get_vec();
        origin_data->insert(origin_data->cend(), temp.cbegin(), temp.cend());
        long long parity{0LL};
        for (int index{4}; index <= 4 + length.l_addr; index++)
            parity += origin_data->at(index);
        parity += aPtr->getParity();
        origin_data->push_back(static_cast<int>(parity % 256));

        origin_data->push_back(22);
        return true;
    }
} // PktParse
