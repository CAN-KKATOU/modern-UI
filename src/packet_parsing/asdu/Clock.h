//
// Created by Astria on 2022/4/25.
//

#ifndef CLOCK_H
#define CLOCK_H

#include "BaseASDU.h"

namespace asdu {

    class Clock : public BaseASDU {
    private:
        string timeStr;  // yyyy-MM-dd hh:mm:ss.zzz ddd

    public:
        explicit Clock(const ASDULengthSet& asdu_length_set);

        Clock(const ASDULengthSet& asdu_length_set,
              long long tie,
              long long vsq,
              long long cot,
              size_t command_address,
              size_t object_address,
              const string& time_str);

        Clock(unique_ptr<vector<int>> data, const ASDULengthSet &als);

        Clock(const Clock& rhs);

        ~Clock() override = default;

        void set_object_address(long long object_address);

        void set_time(const string& time);

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        string get_clock() const { return timeStr; }

        Clock& operator=(const Clock& rhs);
    };

} // asdu

#endif //CLOCK_H
