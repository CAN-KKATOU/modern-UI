//
// Created by Astria on 2022/6/1.
//

#include "PacketFactory.h"

#include "PacketError.h"
#include "default/Packet101.h"
#include "default/PacketFixed.h"
#include "default/PacketVar.h"
#include "default/Packet104.h"
#include "default/Packet104I.h"
#include "default/Packet104S.h"
#include "default/Packet104U.h"
#include "south/South101.h"
#include "south/SouthFixed.h"
#include "south/SouthVar.h"
#include "south/South104.h"
#include "south/South104I.h"
#include "south/South104S.h"
#include "south/South104U.h"

namespace PktParse {
    BasePacket *PacketFactory::getPtr(const string &str, AnalysisMode mode, const LengthSet &length, const string &time_str) {
        BasePacket *mesPtr;
        switch (mode) {
            case AnalysisMode::DEFAULT_101: {
                switch (Packet101::select101Type(str, length.l_addr)) {
                    case 1: {
                        mesPtr = new PacketFixed(str, length);
                        break;
                    }
                    case 2: {
                        mesPtr = new PacketVar(str, length);
                        break;
                    }
                    case -1: {
                        mesPtr = new PacketError(str, length);
                        break;
                    }
                }
                break;
            }
            case AnalysisMode::DEFAULT_104: {
                switch (Packet104::select104Type(str)) {
                    case 1: {
                        mesPtr = new Packet104I(str, length);
                        break;
                    }
                    case 2: {
                        mesPtr = new Packet104S(str, length);
                        break;
                    }
                    case 3: {
                        mesPtr = new Packet104U(str, length);
                        break;
                    }
                    case -1: {
                        mesPtr = new PacketError(str, length);
                        break;
                    }
                }
                break;
            }
            case AnalysisMode::SOUTH_101: {
                switch (South101::select101Type(str, length.l_addr)) {
                    case 1: {
                        mesPtr = new SouthFixed(str, length);
                        break;
                    }
                    case 2: {
                        mesPtr = new SouthVar(str, length);
                        break;
                    }
                    case -1: {
                        mesPtr = new PacketError(str, length);
                        break;
                    }
                }
                break;
            }
            case AnalysisMode::SOUTH_104: {
                switch (South104::select104Type(str)) {
                    case 1: {
                        mesPtr = new South104I(str, length);
                        break;
                    }
                    case 2: {
                        mesPtr = new South104S(str, length);
                        break;
                    }
                    case 3: {
                        mesPtr = new South104U(str, length);
                        break;
                    }
                    case -1: {
                        mesPtr = new PacketError(str, length);
                        break;
                    }
                }
                break;
            }
            default: {
                mesPtr = new PacketError(str, length);
                break;
            }
        }
        if (!time_str.empty())
            mesPtr->set_time_str(time_str);

        return mesPtr;
    }

    shared_ptr<BasePacket>
    PacketFactory::getSPtr(const string &dataStr, AnalysisMode mode, const LengthSet &length, const string &time_str) {
        return shared_ptr<BasePacket>(getPtr(dataStr, mode, length, time_str));
    }

    unique_ptr<BasePacket>
    PktParse::PacketFactory::getUPtr(const std::string &dataStr, AnalysisMode mode, const PktParse::LengthSet &length,
                                     const string &time_str) {
        return unique_ptr<BasePacket>(getPtr(dataStr, mode, length, time_str));
    }

    vector<string> PacketFactory::get_types_list(AnalysisMode mode) {
        switch (mode) {
            case AnalysisMode::DEFAULT_101:
                return Packet101::getAllTypes();
            case AnalysisMode::DEFAULT_104:
                return Packet104::getAllTypes();
            case AnalysisMode::SOUTH_101:
                return South101::getAllTypes();
            case AnalysisMode::SOUTH_104:
                return South104::getAllTypes();
            default:
                return {};
        }
    }

    string PacketFactory::parse_type(const string &data_str, AnalysisMode mode, const LengthSet &length) {
        switch (mode) {
            case AnalysisMode::DEFAULT_101:
                return Packet101::parse101Type(data_str, length.l_addr, length.type);
            case AnalysisMode::DEFAULT_104:
                return Packet104::parse104Type(data_str, length.type);
            case AnalysisMode::SOUTH_101:
                return South101::parse101Type(data_str, length.l_addr, length.type);
            case AnalysisMode::SOUTH_104:
                return South104::parse104Type(data_str, length.type);
            default:
                return {};
        }
    }

    int PacketFactory::parse_type_int(const string &data_str, AnalysisMode mode, const LengthSet &length) {
        switch (mode) {
            case AnalysisMode::DEFAULT_101:
                return Packet101::parse101TypeInt(data_str, length.l_addr, length.type);
            case AnalysisMode::DEFAULT_104:
                return Packet104::parse104TypeInt(data_str, length.type);
            case AnalysisMode::SOUTH_101:
                return South101::parse101TypeInt(data_str, length.l_addr, length.type);
            case AnalysisMode::SOUTH_104:
                return South104::parse104TypeInt(data_str, length.type);
            default:
                return -4;
        }
    }
}

