//
// Created by Astria on 2022/4/25.
//

#include "RemoteControl.h"

namespace asdu {
RemoteControl::RemoteControl(unique_ptr<vector<int>> dataVec, const ASDULengthSet &asduLengthSet)
    : BaseASDU(move(dataVec), asduLengthSet) {
    bSingle = tie == 45;
    size_t cIndex{static_cast<size_t>(asduLength.type +
                                      asduLength.qualifier +
                                      asduLength.cot +
                                      asduLength.commonAd +
                                      asduLength.objectAd)};

    try {
        order = util::vecToLL(dataVec, cIndex, asduLength.qualifier);
        bSelect = order > 127;
        if (bSingle)
            scs = (order % 2) == 1;
        else
            dcs = static_cast<int>(order) % 4;
    } catch (const std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
    }
    catch (const std::out_of_range &e) {
        std::cerr << e.what() << std::endl;
    }

    switch (cot) {
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 44:
    case 45:
    case 46:
    case 47:
        break;
    default:
        errInfo += "传送原因COT错误 ";
        break;
    }
}

shared_ptr<json> RemoteControl::toJson() {
    auto object = std::make_shared<nlohmann::json>();
    (*object)["ASDU报文类型"] = "遥控命令报文";
    switch (tie) {
    case 45:
        (*object)["标识类型"] = "单命令";
        break;
    case 46:
        (*object)["标识类型"] = "双命令";
        break;
    }

    switch (cot) {
    case 6:
        (*object)["传送原因"] = "激活";
        (*object)["报文方向"] = "控制方向";
        break;
    case 8:
        (*object)["传送原因"] = "停止激活";
        (*object)["报文方向"] = "控制方向";
        break;
    case 7:
        (*object)["传送原因"] = "激活确认";
        (*object)["报文方向"] = "监视方向";
        break;
    case 9:
        (*object)["传送原因"] = "停止激活确认";
        (*object)["报文方向"] = "监视方向";
        break;
    case 10:
        (*object)["传送原因"] = "激活终止";
        (*object)["报文方向"] = "监视方向";
        break;
    case 44:
        (*object)["传送原因"] = "未知的类型标识";
        (*object)["报文方向"] = "监视方向";
        break;
    case 45:
        (*object)["传送原因"] = "未知的传动原因";
        (*object)["报文方向"] = "监视方向";
        break;
    case 46:
        (*object)["传送原因"] = "未知的应用服务数据单元公共地址";
        (*object)["报文方向"] = "监视方向";
        break;
    case 47:
        (*object)["传送原因"] = "未知的信息对象地址";
        (*object)["报文方向"] = "监视方向";
        break;
    default:
        (*object)["传送原因"] = "Data Error";
        break;
    }

    (*object)["公共地址"] = cAddr;
    (*object)["遥控信息对象地址"] = oAddr;

    if (bSingle) {
        (*object)["单命令"] = order;
        (*object)["选择/执行"] = (bSelect ? "选择" : "执行");
        (*object)["分/合"] = (scs ? "分" : "合");
    }
    else {
        (*object)["双命令"] = order;
        (*object)["选择/执行"] = (bSelect ? "选择" : "执行");
        if (dcs == 1)
            (*object)["分/合"] = "分";
        else if (dcs == 2)
            (*object)["分/合"] = "合";
        else
            (*object)["分/合"] = "非法控制";
    }

    return object;
}

shared_ptr<json> RemoteControl::getPosition() {
    auto object = make_shared<json>();

    size_t cIndex{static_cast<size_t>(asduLength.startIndex)};

    (*object)["ASDU报文类型"] = util::posJson(cIndex, cIndex + asduLength.type);
    (*object)["标识类型"] = util::posJson(cIndex, cIndex + asduLength.type);

    cIndex += asduLength.type + asduLength.qualifier;
    (*object)["传送原因"] = util::posJson(cIndex, cIndex + asduLength.cot);
    (*object)["报文方向"] = util::posJson(cIndex, cIndex + asduLength.cot);

    cIndex += asduLength.cot;
    (*object)["公共地址"] = util::posJson(cIndex, cIndex + asduLength.commonAd);

    cIndex += asduLength.commonAd;
    (*object)["遥控信息对象地址"] = util::posJson(cIndex, cIndex + asduLength.objectAd);

    cIndex += asduLength.objectAd;
    if (bSingle)
        (*object)["单命令"] = util::posJson(cIndex, cIndex + asduLength.describer);
    else (*object)["双命令"] = util::posJson(cIndex, cIndex + asduLength.describer);

    (*object)["选择/执行"] = util::posJson(cIndex, cIndex + asduLength.describer);
    (*object)["分/合"] = util::posJson(cIndex, cIndex + asduLength.describer);

    return object;
}

bool RemoteControl::isClose() const
{
    if (bSingle)
        return scs;
    else
        return dcs == 2;
}

RemoteControl::RemoteControl(const ASDULengthSet &asduLengthSet)
    : BaseASDU(asduLengthSet) {
    for (int i = 1; i <= asduLength.objectAd + asduLength.qualifier; i++)
        data->push_back(0);

    tie = 45;
    util::replaceElems(data, 0, asduLength.type, tie);
}

RemoteControl::RemoteControl(const ASDULengthSet &asduLengthSet, long long int tie, long long int vsq,
                             long long int cot, size_t commandAddress, size_t objectAddress, long long int order)
    : BaseASDU(asduLengthSet, tie, vsq, cot, commandAddress, objectAddress), order(order) {
    util::pushElems(data, asduLength.qualifier, order);

    bSingle = tie == 45;
    bSelect = order > 127;
    if (bSingle)
        scs = (order % 2) == 1;
    else
        dcs = order % 4;
}

void RemoteControl::set_object_address(size_t objectAddress) {
    oAddr = objectAddress;
    size_t index{static_cast<size_t>(asduLength.type +
                                     asduLength.qualifier +
                                     asduLength.cot +
                                     asduLength.commonAd)};
    util::replaceElems(data, index, asduLength.objectAd, objectAddress);
}

RemoteControl::RemoteControl(const RemoteControl &rhs)
    : BaseASDU(rhs), order(rhs.order), bSingle(rhs.bSingle), bSelect(rhs.bSelect), dcs(rhs.dcs), scs(rhs.scs) {

}

RemoteControl &RemoteControl::operator=(const RemoteControl &rhs) {
    if (this == &rhs)
        return *this;

    BaseASDU::operator=(rhs);
    order = rhs.order;
    bSingle = rhs.bSingle;
    return *this;
}
} // asdu
