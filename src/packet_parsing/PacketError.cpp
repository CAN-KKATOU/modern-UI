//
// Created by Astria on 2022/4/26.
//

#include "PacketError.h"

namespace PktParse {
    shared_ptr<json> PacketError::toJson() {
        auto object = make_shared<json>();
        (*object)["报文类型"] = "未知报文";
        return object;
    }

    shared_ptr<json> PacketError::getPosition() {
        auto object = make_shared<json>();
        (*object)["报文类型"] = util::posJson(0, origin_data->size());
        return object;
    }
} // Message