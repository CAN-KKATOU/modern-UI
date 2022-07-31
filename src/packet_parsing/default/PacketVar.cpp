//
// Created by Astria on 2022/4/24.
//

#include "PacketVar.h"

#include <memory>
#include <utility>

#include "../asdu/GeneralCall.h"
#include "../asdu/Clock.h"
#include "../asdu/ResetProcess.h"
#include "../asdu/Initialized.h"
#include "../asdu/TestOrder.h"
#include "../asdu/RemoteInfo.h"
#include "../asdu/RemoteTest.h"
#include "../asdu/RemoteControl.h"
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
    PacketVar::PacketVar(const string &dataStr, const LengthSet &lengthSet)
            : Packet101(dataStr, lengthSet) {
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
                auto asduData = make_unique<vector<int>>();
                asduData->assign(origin_data->cbegin() + static_cast<long long>(aIndex), origin_data->cend() - 2);

                switch (tie) {
                    case 100:
                        aPtr = make_shared<asdu::GeneralCall>(move(asduData), set);
                        break;
                    case 103:
                        aPtr = make_shared<asdu::Clock>(move(asduData), set);
                        break;
                    case 105:
                        aPtr = make_shared<asdu::ResetProcess>(move(asduData), set);
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
                        aPtr = make_shared<asdu::RemoteInfo>(move(asduData), set);
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
                        error_info += "TI类型标识错误 ";
                        break;
                }
#elif __cplusplus < 201403L
                auto asduData = unique_ptr<vector<int>>(new vector<int>());
                asduData->assign(origin_data->cbegin() + static_cast<long long>(aIndex), origin_data->cend() - 2);

                switch (tie) {
                    case 100:
                        aPtr = make_shared<asdu::GeneralCall>(move(asduData), set);
                        break;
                    case 103:
                        aPtr = make_shared<asdu::Clock>(move(asduData), set);
                        break;
                    case 105:
                        aPtr = make_shared<asdu::ResetProcess>(move(asduData), set);
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
                        aPtr = make_shared<asdu::RemoteInfo>(move(asduData), set);
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
                        error_info += "TI类型标识错误 ";
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

    shared_ptr<json> PacketVar::toJson() {
        auto object = make_shared<json>();

        if (!time_str.empty())
            (*object)["报文时间"] = time_str;

        if (length_flag) {
            (*object)["报文类型"] = "101可变帧长报文";
            (*object)["地址"] = aArea;
            if (dir) {// 平衡模式上行文
                (*object)["方向"] = "平衡模式上行报文";
                if (prm)
                    (*object)["来源"] = "启动站";
                else (*object)["来源"] = "从动站";
                if (dfc)
                    (*object)["从动站接收后续报文"] = "是";
                else (*object)["从动站接收后续报文"] = "否";
            } else {// 平衡模式下行文或非平衡模式下保留位
                if (prm) {// 下行报文
                    if (tMode)
                        (*object)["方向"] = "平衡模式下行报文";
                    else
                        (*object)["方向"] = "非平衡模式下行报文";
                    (*object)["来源"] = "启动站";
                    if (fcv) {
                        (*object)["帧计数位有效"] = "是";
                        if (fcb)
                            (*object)["帧计数位"] = 1;
                        else (*object)["帧计数位"] = 0;
                    } else (*object)["帧计数位有效"] = "否";
                } else {// 非平衡模式上行报文
                    (*object)["方向"] = "非平衡模式上行报文";
                    (*object)["来源"] = "从动站";
                    if (acd)
                        (*object)["配电终端1级数据等待访问"] = "是";
                    else (*object)["配电终端1级数据等待访问"] = "否";
                    if (dfc)
                        (*object)["从动站可以接收后续报文"] = "是";
                    else (*object)["从动站可以接收后续报文"] = "否";
                }
            }

            (*object)["FC功能码"] = fc2string();

            if (aPtr)
                (*object)["ASDU数据"] = *aPtr->toJson();
            else (*object)["ASDU数据"] = "ASDU数据出错";

        } else {
            (*object)["解析错误"] = "报文长度错误";
        }
        return object;
    }

    shared_ptr<json> PacketVar::getPosition() {
        auto object = make_shared<json>();

        if (length_flag) {
            size_t index = 0;
            (*object)["报文类型"] = util::posJson(index, index + 1);
            index += 4;

            if (dir) {// 平衡模式上行文
                (*object)["方向"] = util::posJson(index, index + 1);
                if (prm)
                    (*object)["来源"] = util::posJson(index, index + 1);
                else (*object)["来源"] = util::posJson(index, index + 1);
                if (dfc)
                    (*object)["从动站接收后续报文"] = util::posJson(index, index + 1);
                else (*object)["从动站接收后续报文"] = util::posJson(index, index + 1);
            } else {// 平衡模式下行文或非平衡模式下保留位
                if (prm) {// 下行报文
                    (*object)["方向"] = util::posJson(index, index + 1);
                    (*object)["来源"] = util::posJson(index, index + 1);
                    if (fcv) {
                        (*object)["帧计数位有效"] = util::posJson(index, index + 1);
                        if (fcb)
                            (*object)["帧计数位"] = util::posJson(index, index + 1);
                        else (*object)["帧计数位"] = util::posJson(index, index + 1);
                    } else
                        (*object)["帧计数位有效"] = util::posJson(index, index + 1);
                } else {// 非平衡模式上行报文
                    (*object)["方向"] = util::posJson(index, index + 1);
                    (*object)["来源"] = util::posJson(index, index + 1);
                    if (acd)
                        (*object)["配电终端1级数据等待访问"] = util::posJson(index, index + 1);
                    else (*object)["配电终端1级数据等待访问"] = util::posJson(index, index + 1);
                    if (dfc)
                        (*object)["从动站可以接收后续报文"] = util::posJson(index, index + 1);
                    else (*object)["从动站可以接收后续报文"] = util::posJson(index, index + 1);
                }
            }

            (*object)["FC功能码"] = util::posJson(index, index + 1);
            index++;

            (*object)["地址"] = util::posJson(index, index + length.l_addr);
            index += length.l_addr;

            if (aPtr) {
                (*object)["ASDU数据位置"] = util::posJson(index, index + mLength - 1 - length.l_addr);
                (*object)["ASDU数据"] = *aPtr->getPosition();
            } else
                (*object)["ASDU数据"] = util::posJson(index, index + mLength - 1 - length.l_addr);
        } else {
            (*object)["解析错误"] = util::posJson(0, origin_data->size() - 1);
        }
        return object;
    }

    string PacketVar::getType() const {
        string typeStr;
        switch (tie) {
            case 100:
                typeStr = "总召唤报文";
                break;
            case 103:
                typeStr = "时钟同步/读取报文";
                break;
            case 105:
                typeStr = "复位进程命令报文";
                break;
            case 70:
                typeStr = "初始化结束命令报文";
                break;
            case 104:
                typeStr = "测试命令报文";
                break;
            case 1:
            case 3:
            case 30:
            case 31:
                typeStr = "遥信命令报文";
                break;
            case 9:
            case 11:
            case 13:
                typeStr = "遥测命令报文";
                break;
            case 45:
            case 46:
                typeStr = "遥控命令报文";
                break;
            case 42:
                typeStr = "故障值信息报文";
                break;
            case 210:
                typeStr = "文件服务报文";
                break;
            case 101:
                typeStr = "电能量召唤报文";
                break;
            case 206:
            case 207:
                typeStr = "电能量数据报文";
                break;
            case 200:
                typeStr = "切换定值区报文";
                break;
            case 201:
                typeStr = "读当前定值区号报文";
                break;
            case 202:
                typeStr = "读参数和定值报文";
                break;
            case 203:
                typeStr = "写参数和定值报文";
                break;
            case 211:
                typeStr = "软件升级报文";
                break;
            default:
                typeStr = "未知报文";
                break;
        }
        return typeStr;
    }


    string
    PacketVar::parseVarType(const string &dataStr, int linkLength, int typeLength,
                            const shared_ptr<vector<int>> &dataVec) {
        try {
            string typeStr;
            switch (parseVarTypeInt(dataStr, linkLength, typeLength, dataVec)) {
                case 100:
                    typeStr = "总召唤报文";
                    break;
                case 103:
                    typeStr = "时钟同步/读取报文";
                    break;
                case 105:
                    typeStr = "复位进程命令报文";
                    break;
                case 70:
                    typeStr = "初始化结束命令报文";
                    break;
                case 104:
                    typeStr = "测试命令报文";
                    break;
                case 1:
                case 3:
                case 30:
                case 31:
                    typeStr = "遥信命令报文";
                    break;
                case 9:
                case 11:
                case 13:
                    typeStr = "遥测命令报文";
                    break;
                case 45:
                case 46:
                    typeStr = "遥控命令报文";
                    break;
                case 42:
                    typeStr = "故障值信息报文";
                    break;
                case 210:
                    typeStr = "文件服务报文";
                    break;
                case 101:
                    typeStr = "电能量召唤报文";
                    break;
                case 206:
                case 207:
                    typeStr = "电能量数据报文";
                    break;
                case 200:
                    typeStr = "切换定值区报文";
                    break;
                case 201:
                    typeStr = "读当前定值区号报文";
                    break;
                case 202:
                    typeStr = "读参数和定值报文";
                    break;
                case 203:
                    typeStr = "写参数和定值报文";
                    break;
                case 211:
                    typeStr = "软件升级报文";
                    break;
                default:
                    typeStr = "未知报文";
                    break;
            }
            return typeStr;
        }
        catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
            return "未知报文";
        }
    }

    size_t PacketVar::pktLength() const {
        return mLength;
    }

    long long PacketVar::get_tie() const {
        if (aPtr)
            return aPtr->get_tie();
        else
            return -1;
    }

    long long PacketVar::get_vsq() const {
        if (aPtr)
            return aPtr->get_vsq();
        else
            return -1;
    }

    long long PacketVar::get_cot() const {
        if (aPtr)
            return aPtr->get_cot();
        else
            return -1;
    }

    int PacketVar::parseVarTypeInt(const string &data_str, int link_length, int type_length,
                                   const shared_ptr<vector<int>> &data_vec) {
        auto data = make_shared<vector<int>>();

        if (data_vec == nullptr) {
            *data = util::splitToInt(data_str, " ");
        } else
            data = data_vec;

        try {
            return util::vecToInt(data, 5 + link_length, type_length);
        } catch (const std::out_of_range &e) {
            std::cerr << e.what() << std::endl;
            return -4;
        }
    }

    PacketVar::PacketVar(const LengthSet &lengthSet) : Packet101(lengthSet) {
        mLength = 7 + length.l_addr;
        aIndex = 5 + length.l_addr;

        origin_data->push_back(104);
        origin_data->push_back(7 + length.l_addr);
        origin_data->push_back(7 + length.l_addr);
        origin_data->push_back(104);
        origin_data->push_back(cArea);
        util::pushElems(origin_data, length.l_addr, aArea);

        // 控制域C、地址域A、应用服务数据单元ASDU的字节的八位位组算术和
        int parity{0};
        for (int index{4}; index <= 4 + length.l_addr; index++)
            parity += origin_data->at(index);
        origin_data->push_back(parity % 256);

        origin_data->push_back(22);
    }

    PacketVar::PacketVar(const LengthSet &lengthSet, int controlArea, size_t addressArea,
                         long long int type, shared_ptr<asdu::BaseASDU> asduPtr)
            : Packet101(lengthSet, controlArea, addressArea), tie(type), aPtr(move(asduPtr)) {
        mLength = 7 + length.l_addr + aPtr->getAsduLength();
        aIndex = 5 + length.l_addr;

        origin_data->push_back(104);
        origin_data->push_back(static_cast<int>(mLength));
        origin_data->push_back(static_cast<int>(mLength));
        origin_data->push_back(104);
        origin_data->push_back(cArea);
        util::pushElems(origin_data, length.l_addr, aArea);

        vector<int> temp = aPtr->get_vec();
        origin_data->insert(origin_data->cend(), temp.cbegin(), temp.cend());

        // 控制域C、地址域A、应用服务数据单元ASDU的字节的八位位组算术和
        long long parity{0LL};
        for (int index{4}; index <= 4 + length.l_addr; index++)
            parity += origin_data->at(index);
        parity += aPtr->getParity();
        origin_data->push_back(static_cast<int>(parity % 256));

        origin_data->push_back(22);
    }

    PacketVar::PacketVar(const LengthSet &lengthSet, bool dir, bool prm, bool fcb, bool fcv, bool acd,
                         bool dfc, int fc, size_t addressArea, long long int type,
                         shared_ptr<asdu::BaseASDU> asdu) :
            Packet101(lengthSet, dir, prm, fcb, fcv, acd, dfc, fc, addressArea), tie(type), aPtr(move(asdu)) {
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

    PacketVar::PacketVar(const PacketVar &rhs)
            : Packet101(rhs), tie(rhs.tie), mLength(rhs.mLength), aIndex(rhs.aIndex) {
        if (rhs.aPtr) {
            switch (tie) {
                case 100: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::GeneralCall>(rhs.aPtr);
                    aPtr = make_shared<asdu::GeneralCall>(*temp_ptr);
                    break;
                }
                case 103: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::Clock>(rhs.aPtr);
                    aPtr = make_shared<asdu::Clock>(*temp_ptr);
                    break;
                }
                case 105: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::ResetProcess>(rhs.aPtr);
                    aPtr = make_shared<asdu::ResetProcess>(*temp_ptr);
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
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::RemoteInfo>(rhs.aPtr);
                    aPtr = make_shared<asdu::RemoteInfo>(*temp_ptr);
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
                    error_info += "TI类型标识错误 ";
                    break;
            }
        }
    }

    PacketVar &PacketVar::operator=(const PacketVar &rhs) {
        if (this == &rhs)
            return *this;

        Packet101::operator=(rhs);
        mLength = rhs.mLength;
        tie = rhs.tie;
        aIndex = rhs.aIndex;
        if (rhs.aPtr) {
            switch (tie) {
                case 100: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::GeneralCall>(rhs.aPtr);
                    aPtr = make_shared<asdu::GeneralCall>(*temp_ptr);
                    break;
                }
                case 103: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::Clock>(rhs.aPtr);
                    aPtr = make_shared<asdu::Clock>(*temp_ptr);
                    break;
                }
                case 105: {
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::ResetProcess>(rhs.aPtr);
                    aPtr = make_shared<asdu::ResetProcess>(*temp_ptr);
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
                    auto temp_ptr = std::dynamic_pointer_cast<asdu::RemoteInfo>(rhs.aPtr);
                    aPtr = make_shared<asdu::RemoteInfo>(*temp_ptr);
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
                    error_info += "TI类型标识错误 ";
                    break;
            }
        }
        return *this;
    }

    bool PacketVar::set_asdu(shared_ptr<asdu::BaseASDU> asduPtr) {
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
