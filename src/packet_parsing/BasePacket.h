//
// Created by Astria on 2022/4/24.
//

#ifndef BASE_PACKET_H
#define BASE_PACKET_H

#include <memory>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "nlohmann/json.hpp"
#include "util.h"

namespace PktParse {

    using std::make_shared;
    using std::move;
    using std::string;
    using std::vector;
    using std::unique_ptr;
    using std::shared_ptr;
    using nlohmann::json;

#if __cplusplus >= 201403L
    using std::make_unique;
#endif

    /**
     * @brief 报文解析时，定义每个数据段所占字节长度的结构体
     * @version 2022-5-26
     */
    class LengthSet {
    public:
        bool mode{false};    // true平衡模式，false非平衡模式
        int l_addr{2};       // 链路地址
        int type{1};         // 类型标识符
        int c_addr{2};       // 公共地址
        int cot{2};          // 传送原因
        int o_addr{2};       // 对象地址（信息体地址）
        int qualifier{1};    // 限定词
        int describer{1};    // 描述词
        int sn{2};           // 定值

        LengthSet() = default;

        LengthSet(bool, int, int, int, int, int, int, int, int);

        LengthSet &operator=(const LengthSet &);

        bool operator==(const LengthSet &) const;

        bool operator!=(const LengthSet &) const;
    };

    // 报文解析协议枚举
    enum class AnalysisMode {
        DEFAULT_101 = 1,
        DEFAULT_104 = 2,
        SOUTH_101 = 3,
        SOUTH_104 = 4,
    };

    // 104U格式报文命令枚举
    enum class Packet104UOrder {
        TESTFR = 1,
        STOPDT = 2,
        STARTDT = 3,
    };

    /**
     * @brief 报文数据类的抽象基类
     * @author GaaTou Can
     * @version 2022-5-26
     */
    class BasePacket {
    protected:
        unique_ptr<vector<int>> origin_data; // 原始16进制报文数据
        string error_info;                   // 错误信息
        LengthSet length;                    // 报文长度配置
        bool length_flag{false};             // 长度正确标志位
        string time_str;                     // 报文收发时间

    public:
        explicit BasePacket(const LengthSet &lengthSet) : length(lengthSet) {
#if __cplusplus >= 201403L
            origin_data = make_unique<vector<int>>();
#elif __cplusplus < 201403L
            origin_data = unique_ptr<vector<int>>(new vector<int>);
#endif
            length_flag = true; // 非解析构造下长度判断应保持为true
        }

        BasePacket(const string &dataStr, const LengthSet &lengthSet) : length(lengthSet) {
#if __cplusplus >= 201403L
            origin_data = make_unique<vector<int>>();
#elif __cplusplus < 201403L
            origin_data = unique_ptr<vector<int>>(new vector<int>);
#endif
            *origin_data = util::splitToInt(dataStr, " ");
        }

        BasePacket(const BasePacket &rhs);

        virtual ~BasePacket() = default;

        BasePacket &operator=(const BasePacket &rhs);

        /**
         * @brief 设置报文发送时间
         * @param timeStr
         */
        void set_time_str(const string &timeStr);

        /**
         * @brief 返回报文类型标识符
         * @version 20220729
         * @author Gaatou Can
         * @return >0 可变帧长或I格式报文
         * @return -1 固定帧长报文
         * @return -2 S格式报文
         * @return -3 U格式报文
         * @return -4 错误报文
         */
        virtual long long get_tie() const = 0;

        /**
         * @brief 返回报文限定标识符，不存在则返回-1
         * @version 2022-7-12
         * @author Gaatou Can
         * @return asdu的限定标识符
         */
        virtual long long get_vsq() const = 0;

        /**
         * @brief 返回报文传送原因，不存在则返回-1
         * @version 2022-7-12
         * @author Gaatou Can
         * @return asdu的传送原因
         */
        virtual long long get_cot() const = 0;

        /**
         * @brief 如果解析存在已知错误，则返回错误信息，否则返回空字符串
         * @return std::string
         * @version 2022-5-23
         * @author Gaatou Can
         */
        string get_error_info() const { return error_info; }

        /**
         * @brief 将报文解析为JSON结构体
         * @return JSON结构体的共享智能指针
         * @version 2022-5-26
         * @author Gaatou Can
         */
        virtual shared_ptr<json> toJson() = 0;

        /**
         * @brief 对应to_json()的结构体中报文信息在原始报文中字节的位置 每个段位置包含"start index"和"end index"两个键
         * @return JSON结构体的共享指针
         * @version 2022-5-26
         * @author Gaatou Can
         */
        virtual shared_ptr<json> getPosition() = 0;

        /**
         * @brief 若报文存在类型，则返回对应类型， 否则返回‘未知报文’
         * @return QString
         * @version 2022-5-23
         * @author Gaatou Can
         */
        virtual string getType() const = 0;

        /**
        * @brief 如果可以，返回报文不计起始字符、结束字符、校验和.etc的有效长度，否则返回读取到的字节总数
        * @return size_t(unsigned long long)
        * @version 2022-5-23
        * @author Gaatou Can
        */
        virtual size_t pktLength() const = 0;

        /**
         * @brief 将方法to_json()的JSON对象打包为string类型
         * @param format 是否格式化JSON字符串
         * @return std::string
         * @version 2022-5-23
         * @author Gaatou Can
         */
        string toString(bool format = false);

        /**
        * @brief 将方法get_position()的JSON对象打包为string类型
        * @return std::string
        * @version 2022-5-23
        * @author Gaatou Can
        */
        string getPositionStr(bool format = false);

        /**
         * @brief 返回vector容器封装的报文数据
         * @return vector<int>
         * @author Gaatou Can
         * @version 20220729
         */
        vector<int> get_origin_data() { return *origin_data; }

        /**
         * @brief 返回16进制字符串格式报文数据
         * @return string
         * @author Gaatou Can
         * @version 20220729
         */
        string toPkt();

        /**
         * @brief 重载std::cout输出
         * @param cout
         * @param basePacket
         * @return
         */
        friend std::ostream& operator<<(std::ostream &cout, BasePacket& basePacket);
    };

} // PktParse

#endif // BASE_PACKET_H
