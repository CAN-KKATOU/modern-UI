//
// Created by Astria on 2022/7/6.
//

#include "SouthFixed.h"

namespace PktParse {
    SouthFixed::SouthFixed(const string &dataStr, const LengthSet &lengthSet)
            : South101(dataStr, lengthSet) {

    }

    shared_ptr<json> SouthFixed::toJson() {
        auto object = make_shared<json>();

        if (!time_str.empty())
            (*object)["报文时间"] = time_str;

        if (length_flag) {
            (*object)["报文类型"] = "101固定帧长报文";
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
        } else
            (*object)["解析错误"] = "报文长度错误";
        return object;
    }

    shared_ptr<json> SouthFixed::getPosition() {
        auto object = make_shared<json>();

        if (length_flag) {
            int index = 0;
            (*object)["报文类型"] = util::posJson(index, index + 1);
            index++;

            if (dir) {// 平衡模式上行文
                (*object)["方向"] = util::posJson(index, index + 1);
                if (prm)
                    (*object)["来源"] = util::posJson(index, index + 1);
                else
                    (*object)["来源"] = util::posJson(index, index + 1);
                if (dfc)
                    (*object)["从动站接收后续报文"] = util::posJson(index, index + 1);
                else
                    (*object)["从动站接收后续报文"] = util::posJson(index, index + 1);
            } else {
                // 平衡模式下行文或非平衡模式下保留位
                if (prm) {// 下行报文
                    (*object)["方向"] = util::posJson(index, index + 1);
                    (*object)["来源"] = util::posJson(index, index + 1);
                    if (fcv) {
                        (*object)["帧计数位有效"] = util::posJson(index, index + 1);
                        if (fcb)
                            (*object)["帧计数位"] = util::posJson(index, index + 1);
                        else
                            (*object)["帧计数位"] = util::posJson(index, index + 1);
                    }
                    (*object)["帧计数位有效"] = util::posJson(index, index + 1);
                } else {// 非平衡模式上行报文
                    (*object)["方向"] = util::posJson(index, index + 1);
                    (*object)["来源"] = util::posJson(index, index + 1);
                    if (acd)
                        (*object)["配电终端1级数据等待访问"] = util::posJson(index, index + 1);
                    else
                        (*object)["配电终端1级数据等待访问"] = util::posJson(index, index + 1);
                    if (dfc)
                        (*object)["从动站可以接收后续报文"] = util::posJson(index, index + 1);
                    else
                        (*object)["从动站可以接收后续报文"] = util::posJson(index, index + 1);
                }
            }
            (*object)["FC功能码"] = util::posJson(index, index + 1);
            index++;
            (*object)["地址"] = util::posJson(index, index + length.l_addr);
        } else
            (*object)["解析错误"] = util::posJson(0, origin_data->size() - 1);
        return object;
    }

    SouthFixed::SouthFixed(const LengthSet &lengthSet) : South101(lengthSet) {
        origin_data->push_back(16);
        origin_data->push_back(cArea);
        util::pushElems(origin_data, length.l_addr, aArea);

        int parity{0};
        for (int index{1}; index <= 1 + length.l_addr; index++)
            parity += origin_data->at(index);
        origin_data->push_back(parity % 256);

        origin_data->push_back(22);
    }

    SouthFixed::SouthFixed(const LengthSet &lengthSet, int controlArea, size_t addressArea)
            : South101(lengthSet, controlArea, addressArea) {
        origin_data->push_back(16);
        origin_data->push_back(cArea);
        util::pushElems(origin_data, length.l_addr, aArea);

        int parity{0};
        for (int index{1}; index <= 1 + length.l_addr; index++)
            parity += origin_data->at(index);
        origin_data->push_back(parity % 256);

        origin_data->push_back(22);
    }

    SouthFixed::SouthFixed(const LengthSet &lengthSet, bool dir, bool prm, bool fcb, bool fcv, bool acd, bool dfc,
                           int fc, size_t addressArea)
            : South101(lengthSet, dir, prm, fcb, fcv, acd, dfc, fc, addressArea) {
        origin_data->push_back(16);
        origin_data->push_back(cArea);
        util::pushElems(origin_data, length.l_addr, aArea);

        int parity{0};
        for (int index{1}; index <= 1 + length.l_addr; index++)
            parity += origin_data->at(index);

        origin_data->push_back(parity % 256);
        origin_data->push_back(22);
    }

    SouthFixed &SouthFixed::operator=(const SouthFixed &rhs) {
        if (this == &rhs)
            return *this;

        South101::operator=(rhs);
        return *this;
    }

    SouthFixed::SouthFixed(const SouthFixed &rhs) : South101(rhs) {

    }
} // PktParse