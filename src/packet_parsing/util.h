//
// Created by Astria on 2022/4/24.
//

#ifndef UTILITY_H
#define UTILITY_H

#include "nlohmann/json.hpp"
#include <string>
#include <sstream>
#include <vector>


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

class util {
public:
    static json posJson(size_t start, size_t end);

    static vector<string> split(const string &, const string &);

    static vector<int> splitToInt(const string &str, const string &delim);

    static string decToHex(int dec, unsigned int width);

    static string decToBin(int dec);

    /**
     * @brief 解析cp56time2a并返回字符串
     * @param time 含7个元素的vector
     * @return 形如yyyy-MM-dd hh:mm:ss.zzz ddd (2022-07-20 14:15:36.320 Wed)
     */
    static string parseTime(const vector<int> &time);

    /**
     * @brief 生成cp56time2a并推入vector
     * @param dest_vec 目标vector
     * @param time_str 期望获取时间字符串格式yyyy-MM-dd hh:mm:ss.zzz ddd (2022-07-20 14:15:36.320 Wed)
     * @enum 星期几用英文缩写表示，占三个字符位 Mon Tue Wed Thu Fri Sat Sun
     */
    static void pushTime(const unique_ptr<vector<int>> &dest_vec, const string &time_str);

    /**
     * @brief 生成cp56time2a并修改vector中对应的7位元素
     * @param dest_vec 目标vector
     * @param start_index 起始位置（包括起始位置）
     * @param time_str 期望获取时间字符串格式yyyy-MM-dd hh:mm:ss.zzz ddd (2022-07-20 14:15:36.320 Wed)
     * @enum 星期几用英文缩写表示，占三个字符位 Mon Tue Wed Thu Fri Sat Sun
     */
    static void replaceTime(const unique_ptr<vector<int>> &dest_vec, size_t start_index, const string &time_str);

    static float parseIeee574(const vector<int> &intData);

    static void pushIeee574(const unique_ptr<vector<int>> &dest_vec, float source);

    static void replaceIeee574(const unique_ptr<vector<int>> &dest_vec, size_t start_index, float source);

    static string intToString(const std::vector<int> &data);

    static vector<int> stringToInt(const string &str);

    static string parseFeature(int feature);

    static string parseQOS(int qos);

    template<typename T>
    static vector<T> vecMid(const vector<T> &vec, size_t pre, size_t len) {
        if (pre >= vec.size() or pre + len > vec.size())
            throw std::out_of_range("position is greater than vector' s size. pre & len ver.");
#if __cplusplus >= 201403L
        auto res = std::make_unique<std::vector<T>>(vec.cbegin() + pre, vec.cbegin() + pre + len);
#elif __cplusplus < 201403L
        auto res = unique_ptr<vector<T>>(new vector<T>(vec.cbegin() + pre, vec.cbegin() + pre + len));
#endif
        return move(*res);
    }

    template<typename T>
    static vector<T> vecMid(const unique_ptr<vector<T>> &vec, size_t pre, size_t len) {
        if (pre >= vec->size() or pre + len > vec->size())
            throw std::out_of_range("position is greater than vector' s size. pre & len ver.");
#if __cplusplus >= 201403L
        auto res = make_unique<std::vector<T>>(vec->cbegin() + pre, vec->cbegin() + pre + len);
#elif __cplusplus < 201403L
        auto res = unique_ptr<vector<T>>(new vector<T>(vec->cbegin() + pre, vec->cbegin() + pre + len));
#endif
        return move(*res);
    }

    template<typename T>
    static vector<T> vecMid(const shared_ptr<vector<T>> &vec, size_t pre, size_t len) {
        if (pre >= vec->size() or pre + len > vec->size())
            throw std::out_of_range("position is greater than vector' s size. pre & len ver.");
#if __cplusplus >= 201403L
        auto res = make_unique<std::vector<T>>(vec->cbegin() + pre, vec->cbegin() + pre + len);
#elif __cplusplus < 201403L
        auto res = unique_ptr<vector<T>>(new vector<T>(vec->cbegin() + pre, vec->cbegin() + pre + len));
#endif
        return move(*res);
    }

    template<typename T>
    static vector<T> vecMid(const vector<T> &vec, size_t pre) {
        if (pre >= vec.size())
            throw std::out_of_range("position is greater than vector' s size. pre ver.");

#if __cplusplus >= 201403L
        auto res = std::make_unique<std::vector<T>>(vec.cbegin() + pre, vec.cend());
#elif __cplusplus < 201403L
        auto res = unique_ptr<vector<T>>(new vector<T>(vec.cbegin() + pre, vec.cend()));
#endif
        return move(*res);
    }

    template<typename T>
    static vector<T> vecMid(const unique_ptr<vector<T>> &vec, size_t pre) {
        if (pre >= vec->size())
            throw std::out_of_range("position is greater than vector' s size. pre ver.");

#if __cplusplus >= 201403L
        auto res = std::make_unique<std::vector<T>>(vec->cbegin() + pre, vec->cend());
#elif __cplusplus < 201403L
        auto res = unique_ptr<vector<T>>(new vector<T>(vec->cbegin() + pre, vec->cend()));
#endif
        return move(*res);
    }

    template<typename T>
    static vector<T> vecMid(const shared_ptr<vector<T>> &vec, size_t pre) {
        if (pre >= vec->size())
            throw std::out_of_range("position is greater than vector' s size. pre ver.");

#if __cplusplus >= 201403L
        auto res = std::make_unique<std::vector<T>>(vec->cbegin() + pre, vec->cend());
#elif __cplusplus < 201403L
        auto res = unique_ptr<vector<T>>(new vector<T>(vec->cbegin() + pre, vec->cend()));
#endif
        return move(*res);
    }

    // 以下函数用于将vector中多个整数转化为一个整数，遵循低位在前，高位在后

    static size_t vecToULL(const unique_ptr<vector<int>> &source_vec, size_t start_index, int length);

    static long long vecToLL(const unique_ptr<vector<int>> &source_vec, size_t start_index, int length);

    static long long vecToLong(const unique_ptr<vector<int>> &source_vec, size_t start_index, int length);

    static int vecToInt(const unique_ptr<vector<int>> &source_vec, size_t start_index, int length);

    static size_t vecToULL(const shared_ptr<vector<int>> &source_vec, size_t start_index, int length);

    static long long vecToLL(const shared_ptr<vector<int>> &source_vec, size_t start_index, int length);

    static long long vecToLong(const shared_ptr<vector<int>> &source_vec, size_t start_index, int length);

    static int vecToInt(const shared_ptr<vector<int>> &source_vec, size_t start_index, int length);

    /**
     * @brief 将vector<int>中指定一段元素按顺序转为16进制字符串,包括起始位，直到起始位加长度-1;
     * @param source_vec 源容器
     * @param start_index 起始位置
     * @param length 长度
     * @return 16进制字符串
     */
    static string vecToHexStr(const unique_ptr<vector<int>> &source_vec, size_t start_index, int length);

    static string vecToHexStr(const shared_ptr<vector<int>> &source_vec, size_t start_index, int length);

    // 以下函数用于将一个整数根据长度配置拆分并添加到vector尾部，遵循低位在前，高位在后

    static void pushElems(const unique_ptr<vector<int>> &dest_vec, int length, size_t source);

    static void pushElems(const unique_ptr<vector<int>> &dest_vec, int length, long long source);

    static void pushElems(const unique_ptr<vector<int>> &dest_vec, int length, long source);

    static void pushElems(const unique_ptr<vector<int>> &dest_vec, int length, int source);

    static void pushElems(const shared_ptr<vector<int>> &dest_vec, int length, size_t source);

    static void pushElems(const shared_ptr<vector<int>> &dest_vec, int length, long long source);

    static void pushElems(const shared_ptr<vector<int>> &dest_vec, int length, long source);

    static void pushElems(const shared_ptr<vector<int>> &dest_vec, int length, int source);

    static void pushString(const unique_ptr<vector<int>> &dest_vec, const string &str);

    static void pushString(const shared_ptr<vector<int>> &dest_vec, const string &str);


    // 以下函数用于将一个整数根据长度配置拆分并根据起始位置修改vector中的数据，遵循低位在前，高位在后

    static void
    replaceElems(const unique_ptr<vector<int>> &dest_vec, size_t start_index, int length, size_t source);

    static void
    replaceElems(const unique_ptr<vector<int>> &dest_vec, size_t start_index, int length, long long source);

    static void replaceElems(const unique_ptr<vector<int>> &dest_vec, size_t start_index, int length, long source);

    static void replaceElems(const unique_ptr<vector<int>> &dest_vec, size_t start_index, int length, int source);

    static void
    replaceElems(const shared_ptr<vector<int>> &dest_vec, size_t start_index, int length, size_t source);

    static void
    replaceElems(const shared_ptr<vector<int>> &dest_vec, size_t start_index, int length, long long source);

    static void replaceElems(const shared_ptr<vector<int>> &dest_vec, size_t start_index, int length, long source);

    static void replaceElems(const shared_ptr<vector<int>> &dest_vec, size_t start_index, int length, int source);
};

#endif //UTILITY_H
