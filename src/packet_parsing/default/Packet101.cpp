//
// Created by Astria on 2022/4/24.
//

#include "Packet101.h"
#include "PacketVar.h"

namespace PktParse {
    Packet101::Packet101(const string &dataStr, const LengthSet &lengthSet)
            : BasePacket(dataStr, lengthSet) {
        if (origin_data->at(0) == 16) {// 固定帧长
            tMode = length.mode;
            if (origin_data->size() == 4 + length.l_addr) {
                length_flag = true;

                cArea = origin_data->at(1);

                try {
                    aArea = util::vecToULL(origin_data, 2, length.l_addr);
                }
                catch (const std::invalid_argument &e) {
                    std::cerr << e.what() << std::endl;
                }
                catch (const std::out_of_range &e) {
                    std::cerr << e.what() << std::endl;
                }
            } else
                length_flag = false;
        } else if (origin_data->at(0) == 104) {// 可变帧长
            if (origin_data->size() == origin_data->at(1) + 6) {
                length_flag = true;

                cArea = origin_data->at(4);

                try {
                    aArea = util::vecToULL(origin_data, 2, length.l_addr);
                }
                catch (const std::invalid_argument &e) {
                    std::cerr << e.what() << std::endl;
                }
                catch (const std::out_of_range &e) {
                    std::cerr << e.what() << std::endl;
                }
            } else
                length_flag = false;
        }

        if (length_flag) {
            // 控制域解析
            string CAStr = util::decToBin(cArea);

            if (CAStr[0] == L'1') {// 平衡模式上行文
                dir = true;
                prm = CAStr[1] == L'1';
                dfc = CAStr[3] == L'1';
            } else {// 平衡模式下行文或非平衡模式下保留位
                if (CAStr[1] == L'1') {// 平衡模式下行报文，非平衡模式下主站只能是启动站，所以也是下行报文
                    prm = true;
                    fcb = CAStr[2] == L'1';
                    fcv = CAStr[3] == L'1';
                } else {// 非平衡模式上行报文
                    acd = CAStr[2] == L'1';
                    dfc = CAStr[3] == L'1';
                }
            }
            fcCode = cArea % 16;

            // FC码验错
            switch (fcCode) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 8:
                case 9:
                case 10:
                case 11:
                    break;
                default: {
                    error_info.append("无效FC功能码 ");
                    break;
                }
            }
        }
    }

    string Packet101::fc2string() const {
        string str;
        switch (fcCode) {
            case 0: {
                if (prm)
                    str = "复位远方链路";
                else {
                    str = "确认： 认可";
                }
                break;
            }
            case 1: {
                if (fcv)
                    str = "确认： 否定认可请求";
                else str = "确认： 否定认可";
                break;
            }
            case 2:
                str = "发送/确认 链路测试功能";
                break;
            case 3:
                str = "发送/确认用户数据";
                break;
            case 4:
                str = "发送/无回答用户数据";
                break;
            case 8: {
                if (prm)
                    str = "访问请求";
                else str = "响应： 用户数据";
                break;
            }
            case 9: {
                if (prm)
                    str = "请求/响应 请求链路状态";
                else str = "响应： 无所请求的用户数据";
                break;
            }
            case 10:
                str = "请求/响应 请求1级用户数据";
                break;
            case 11: {
                if (prm)
                    str = "请求/响应 请求2级用户数据";
                else str = "响应： 链路状态";
                break;
            }
            default:
                str = "无效FC功能码";
        }
        return str;
    }

    int Packet101::select101Type(const string &dataStr, int linkLength, const shared_ptr<vector<int>> &dataVec) {
        auto data = std::make_shared<std::vector<int>>();

        if (dataVec == nullptr) {
            *data = util::splitToInt(dataStr, " ");
        } else data = dataVec;

        if (data->size() < 4 + linkLength) // 长度不足
            return -1;

        if (data->at(0) == 16 and data->back() == 22) {
            int sum = 0;
            for (int index = 1; index < 2 + linkLength; index++)
                sum += data->at(index);
            int CS_ = data->at(2 + linkLength); // 帧校验和
            if (sum % 256 != CS_) // 帧校验和模运算出错，解析失败
                return -1;
            return 1;
        } else if (data->at(0) == 104 and data->back() == 22) {
            // 可变帧长判断
            int check_sum = 0; // 控制域C、地址域A、应用服务数据单元ASDU的字节的八位位组算术和
            for (int index = 4; index <= data->size() - 3; index++) // 从报文第五个字节到倒数第三个字节
                check_sum += data->at(index);
            if (check_sum % 256 != data->at(data->size() - 2))
                return -1;
            return 2;
        } else return -1;
    }

    vector<string> Packet101::getAllTypes() {
        vector<string> vec;
        vec.emplace_back("总召唤报文");
        vec.emplace_back("时钟同步/读取报文");
        vec.emplace_back("复位进程命令报文");
        vec.emplace_back("初始化结束命令报文");
        vec.emplace_back("测试命令报文");
        vec.emplace_back("遥信命令报文");
        vec.emplace_back("遥测命令报文");
        vec.emplace_back("遥控命令报文");
        vec.emplace_back("故障值信息报文");
        vec.emplace_back("文件服务报文");
        vec.emplace_back("电能量召唤报文");
        vec.emplace_back("电能量数据报文");
        vec.emplace_back("切换定值区报文");
        vec.emplace_back("读当前定值区号报文");
        vec.emplace_back("读参数和定值报文");
        vec.emplace_back("写参数和定值报文");
        vec.emplace_back("软件升级报文");
        vec.emplace_back("固定帧长");
        vec.emplace_back("未知报文");
        return vec;
    }

    string Packet101::parse101Type(const string &dataStr, int linkLength, int typeLength) {
        auto data = make_shared<vector<int>>();
        *data = util::splitToInt(dataStr, " ");

        switch (select101Type(dataStr, linkLength, data)) {
            case 1:
                return "固定帧长";
            case 2: {
                return PacketVar::parseVarType(dataStr, linkLength, typeLength, data);
            }
            default:
                return "未知报文";
        }
    }

    int Packet101::parse101TypeInt(const string &dataStr, int linkLength, int typeLength) {
        auto data = make_shared<vector<int>>();
        *data = util::splitToInt(dataStr, " ");

        switch (select101Type(dataStr, linkLength, data)) {
            case 1:
                return -1;
            case 2: {
                return PacketVar::parseVarTypeInt(dataStr, linkLength, typeLength, data);
            }
            default:
                return -4;
        }
    }

    Packet101::Packet101(const Packet101 &rhs)
            : BasePacket(rhs),
              tMode(rhs.tMode),
              cArea(rhs.cArea),
              dir(rhs.dir),
              prm(rhs.prm),
              fcb(rhs.fcb),
              fcv(rhs.fcv),
              acd(rhs.acd),
              dfc(rhs.dfc),
              fcCode(rhs.fcCode) {

    }

    Packet101 &Packet101::operator=(const Packet101 &rhs) {
        if (this == &rhs)
            return *this;

        BasePacket::operator=(rhs);
        tMode = rhs.tMode;
        cArea = rhs.cArea;
        dir = rhs.dir;
        prm = rhs.prm;
        fcb = rhs.fcb;
        fcv = rhs.fcv;
        acd = rhs.acd;
        dfc = rhs.dfc;
        fcCode = rhs.fcCode;
        return *this;
    }

    Packet101::Packet101(const LengthSet &lengthSet, int controlArea, size_t addressArea)
            : BasePacket(lengthSet), cArea(controlArea), aArea(addressArea) {
        tMode = length.mode;

        string CAStr = util::decToBin(cArea);

        if (CAStr[0] == L'1') {// 平衡模式上行文
            dir = true;
            prm = CAStr[1] == L'1';
            dfc = CAStr[3] == L'1';
        } else {// 平衡模式下行文或非平衡模式下保留位
            if (CAStr[1] == L'1') {// 平衡模式下行报文，非平衡模式下主站只能是启动站，所以也是下行报文
                prm = true;
                fcb = CAStr[2] == L'1';
                fcv = CAStr[3] == L'1';
            } else {// 非平衡模式上行报文
                acd = CAStr[2] == L'1';
                dfc = CAStr[3] == L'1';
            }
        }
        fcCode = cArea % 16;
    }

    Packet101::Packet101(const LengthSet &lengthSet, bool dir, bool prm, bool fcb, bool fcv, bool acd, bool dfc,
                         int fc, size_t addressArea)
            : BasePacket(lengthSet),
              dir(dir),
              prm(prm),
              fcb(fcb),
              fcv(fcv),
              acd(acd),
              dfc(dfc),
              fcCode(fc),
              aArea(addressArea) {
        tMode = length.mode;
        if (dir) {
            cArea += 128;
            cArea += (prm ? 64 : 0);
            cArea += (dfc ? 16 : 0);
        } else {
            if (prm) {
                cArea += 64;
                cArea += (fcb ? 32 : 0);
                cArea += (fcv ? 16 : 0);
            } else {
                cArea += (acd ? 32 : 0);
                cArea += (dfc ? 16 : 0);
            }
        }
        cArea += fcCode;
    }

} // PktParse