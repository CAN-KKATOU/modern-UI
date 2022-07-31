//
// Created by Astria on 2022/4/26.
//

#ifndef PACKET_ERROR_H
#define PACKET_ERROR_H

#include <iostream>
#include "BasePacket.h"

namespace PktParse {

    class PacketError : public BasePacket {
    public:
        PacketError() = delete;
        PacketError(const string& data, const LengthSet& ls) : BasePacket(data, ls) {
            std::cout << "Error BasePacket" << std::endl;
        }
        ~PacketError() override = default;

        long long get_tie() const override { return -4; }

        long long get_vsq() const override { return -1; }

        long long get_cot() const override { return -1; }

        shared_ptr<json> toJson() override;
        shared_ptr<json> getPosition() override;
        string getType() const override { return "未知报文"; }
        size_t pktLength() const override { return origin_data->size(); }
    };

} // Message

#endif //PACKET_ERROR_H
