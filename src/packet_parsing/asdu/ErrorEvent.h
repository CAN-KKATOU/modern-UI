//
// Created by Astria on 2022/4/25.
//

#ifndef ERROR_EVENT_H
#define ERROR_EVENT_H

#include "BaseASDU.h"

namespace asdu {
    struct ErrorInfoElem {
        size_t address{0ULL};
        long long value{0LL};
        string time;
    };

    struct ErrorNvaElem {
        size_t address{0ULL};
        int value{0};
    };

    struct ErrorFloatElem {
        size_t address{0ULL};
        float value{0.0};
    };

    class ErrorEvent : public BaseASDU {
    private:
        int _iCount{0};         // 故障遥信个数
        int _tCount{0};         // 故障遥测个数
        long long _iType{0LL};  // 故障遥信类型
        long long _tType{0LL};  // 故障遥测类型
        bool _isFloat{false};   // 故障遥测是否为ieee574
        unique_ptr<vector<ErrorInfoElem>> _iVec;
        unique_ptr<vector<ErrorNvaElem>> _nVec;
        unique_ptr<vector<ErrorFloatElem>> _fVec;

    public:
        explicit ErrorEvent(const ASDULengthSet &asdu_length_set);

        ErrorEvent(const ASDULengthSet &asdu_length_set,
                   long long tie,
                   long long vsq,
                   long long cot,
                   size_t command_address);

        ErrorEvent(unique_ptr<vector<int>> data, const ASDULengthSet &als);

        ErrorEvent(const ErrorEvent& rhs);

        ~ErrorEvent() override = default;

        /**
         * @brief 设置故障遥信数据的个数和类型
         * @param count
         * @param type
         * @author Gaatou Can
         * @version 20220721
         */
        void set_info_count_type(int count, long long type);

        void add_info_elem(const ErrorInfoElem &elem);

        void add_info_elems(const vector<ErrorInfoElem> &elems);

        void add_info_elems(const shared_ptr<vector<ErrorInfoElem>> &elems);

        /**
         * @brief 设置故障遥测数据的个数和类型
         * @brief 请在遥信数据添加完成后调用
         * @param count
         * @param type
         * @author Gaatou Can
         * @version 20220721
         */
        void set_test_count_type(int count, long long type);

        bool add_value_elem(const ErrorNvaElem &elem);

        bool add_value_elems(const vector<ErrorNvaElem> &elems);

        bool add_value_elems(const shared_ptr<vector<ErrorNvaElem>> &elems);

        bool add_float_elem(const ErrorFloatElem &elem);

        bool add_float_elems(const vector<ErrorFloatElem> &elems);

        bool add_float_elems(const shared_ptr<vector<ErrorFloatElem>> &elems);

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        ErrorEvent& operator=(const ErrorEvent& rhs);
    };

} // asdu

#endif //ERROR_EVENT_H
