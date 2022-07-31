//
// Created by Astria on 2022/4/24.
//

#include "util.h"
#include <iostream>

json util::posJson(size_t start, size_t end) {
    json obj = {{"start index", start},
                {"end index",   end}};
    return obj;
}

vector<string> util::split(const string &str, const string &delim) {
    vector<string> elems;

    if (str.empty())
        return elems;

    size_t previous{0ULL};
    size_t current{str.find(delim)};

    while (current != string::npos) {
        if (current > previous) {
            elems.push_back(str.substr(previous, current - previous));
        }
        previous = current + 1;
        current = str.find(delim, previous);
    }
    if (previous != str.size()) {
        elems.push_back(str.substr(previous));
    }
    return elems;
}

vector<int> util::splitToInt(const string &str, const string &delim) {
    vector<int> elems;

    if (str.empty())
        return elems;

    size_t previous{0ULL};
    size_t current{str.find(delim)};

    while (current != string::npos) {
        if (current > previous) {
            try {
                elems.push_back(stoi(str.substr(previous, current - previous), nullptr, 16));
            }
            catch (const std::invalid_argument &e) {
                std::cerr << e.what() << std::endl;
            }
        }
        previous = current + 1;
        current = str.find(delim, previous);
    }
    if (previous != str.size()) {
        try {
            elems.push_back(stoi(str.substr(previous), nullptr, 16));
        }
        catch (const std::invalid_argument &e) {
            std::cerr << e.what();
        }
    }
    return elems;
}

string util::decToHex(int dec, unsigned int width) {
    std::stringstream ss;
    string temp;
    ss << std::hex << dec;
    ss >> temp;
    if (width > temp.size()) {
        string s_0(width - temp.size(), '0');
        temp = s_0 + temp;
    }
    string s = {temp.substr(temp.length() - width, temp.length())};
    return s;
}

string util::decToBin(int dec) {
    if (dec > 255 or dec < 0)
        return " ";
    string result;
    for (int i = 7; i >= 0; i--) {
        result += std::to_string((dec >> i) & 1);
    }
    return result;
}

std::string util::parseTime(const std::vector<int> &time) {
    if (time.size() != 7) {
        return "Time Error";
    }

    float second{static_cast<float>(time.at(1) * 256 + time.at(0)) / 1000};
    int minute{time.at(2) % 64};
    int hour{time.at(3) % 32};
    int wDay{time.at(4) / 32};
    int mDay{time.at(4) % 32};
    int month{time.at(5) % 16};
    int year{time.at(6) % 128 + 2000};

    string day_of_week;
    switch (wDay) {
        case 1:
            day_of_week = "Mon";
            break;
        case 2:
            day_of_week = "Tue";
            break;
        case 3:
            day_of_week = "Wed";
            break;
        case 4:
            day_of_week = "Thu";
            break;
        case 5:
            day_of_week = "Fri";
            break;
        case 6:
            day_of_week = "Sat";
            break;
        case 7:
            day_of_week = "Sun";
            break;
        default:
            day_of_week = "Mon";
            break;
    }

    std::ostringstream os;
    os << std::setfill('0') << std::setw(4) << year << "-"
       << std::setw(2) << month << "-"
       << std::setw(2) << mDay << " "
       << std::setw(2) << hour << ":"
       << std::setw(2) << minute << ":"
       << std::setw(6) << std::setprecision(3) << std::fixed << second << " "
       << std::setw(3) << day_of_week;
    return os.str();
}

float util::parseIeee574(const std::vector<int> &intData) {
    if (intData.size() != 4)
        return 0.0;

    unsigned char value[4];

    value[0] = intData.at(0);
    value[1] = intData.at(1);
    value[2] = intData.at(2);
    value[3] = intData.at(3);

    float res;

    std::memcpy(&res, value, 4);

    return res;
}

string util::intToString(const std::vector<int> &data) {
    string str;
    for (const auto &val: data)
        str += char(val);
    return str;
}

string util::parseFeature(int feature) {
    string str;
    if (feature > 127)
        str += "有后续，";
    else str += "无后续，";
    if ((feature % 4) > 1)
        str += "取消预置，";
    else str += "未用，";
    if (feature % 2 == 1)
        str += "预置";
    else str += "固化";
    return str;
}

string util::parseQOS(int qos) {
    if (qos >= 128)
        return "选择预置参数";
    else return "执行激活参数";
}

void util::pushElems(const unique_ptr<vector<int>> &dest_vec, int length, size_t source) {
    for (int n{0}; n < length; n++)
        dest_vec->push_back(static_cast<int>(source / static_cast<int>(pow(256, n))));
}

void util::pushElems(const unique_ptr<vector<int>> &dest_vec, int length, long long int source) {
    for (int n{0}; n < length; n++)
        dest_vec->push_back(static_cast<int>(source / static_cast<int>(pow(256, n))));
}

void util::pushElems(const unique_ptr<vector<int>> &dest_vec, int length, long source) {
    for (int n{0}; n < length; n++)
        dest_vec->push_back(static_cast<int>(source / static_cast<int>(pow(256, n))));
}

void util::pushElems(const unique_ptr<vector<int>> &dest_vec, int length, int source) {
    for (int n{0}; n < length; n++)
        dest_vec->push_back(static_cast<int>(source / static_cast<int>(pow(256, n))));
}

void util::replaceElems(const unique_ptr<vector<int>> &dest_vec, size_t start_index, int length, size_t source) {
    if (start_index >= dest_vec->size() or start_index + length > dest_vec->size())
        throw std::out_of_range("position is greater than vector' s size. replace ull ver.");

    for (int n{0}; n < length; n++)
        dest_vec->at(start_index + n) = static_cast<int>(source / static_cast<int>(pow(256, n)));
}

void
util::replaceElems(const unique_ptr<vector<int>> &dest_vec, size_t start_index, int length, long long source) {
    if (start_index >= dest_vec->size() or start_index + length > dest_vec->size())
        throw std::out_of_range("position is greater than vector' s size. replace ll ver.");

    for (int n{0}; n < length; n++)
        dest_vec->at(start_index + n) = static_cast<int>(source / static_cast<int>(pow(256, n)));
}

void util::replaceElems(const unique_ptr<vector<int>> &dest_vec, size_t start_index, int length, long source) {
    if (start_index >= dest_vec->size() or start_index + length > dest_vec->size())
        throw std::out_of_range("position is greater than vector' s size. replace long ver.");

    for (int n{0}; n < length; n++)
        dest_vec->at(start_index + n) = static_cast<int>(source / static_cast<int>(pow(256, n)));
}

void util::replaceElems(const unique_ptr<vector<int>> &dest_vec, size_t start_index, int length, int source) {
    if (start_index >= dest_vec->size() or start_index + length > dest_vec->size())
        throw std::out_of_range("position is greater than vector' s size. replace int ver.");

    for (int n{0}; n < length; n++)
        dest_vec->at(start_index + n) = static_cast<int>(source / static_cast<int>(pow(256, n)));
}

size_t util::vecToULL(const unique_ptr<vector<int>> &source_vec, size_t start_index, int length) {
    if (start_index >= source_vec->size() or start_index + length > source_vec->size())
        throw std::out_of_range("position is greater than vector' s size. to ull ver.");

    size_t res{0};
    for (int index{0}; index < length; index++)
        res += static_cast<size_t>(source_vec->at(index + start_index) * pow(256, index));
    return res;
}

long long util::vecToLL(const unique_ptr<vector<int>> &source_vec, size_t start_index, int length) {
    if (start_index >= source_vec->size() or start_index + length > source_vec->size())
        throw std::out_of_range("position is greater than vector' s size. to ll ver.");

    long long res{0};
    for (int index{0}; index < length; index++)
        res += static_cast<long long>(source_vec->at(index + start_index) * pow(256, index));
    return res;
}

long long util::vecToLong(const unique_ptr<vector<int>> &source_vec, size_t start_index, int length) {
    if (start_index >= source_vec->size() or start_index + length > source_vec->size())
        throw std::out_of_range("position is greater than vector' s size. to long ver.");

    long res{0};
    for (int index{0}; index < length; index++)
        res += static_cast<long>(source_vec->at(index + start_index) * pow(256, index));
    return res;
}

int util::vecToInt(const unique_ptr<vector<int>> &source_vec, size_t start_index, int length) {
    if (start_index >= source_vec->size() or start_index + length > source_vec->size())
        throw std::out_of_range("position is greater than vector' s size. to int ver.");

    int res{0};
    for (int index{0}; index < length; index++)
        res += static_cast<int>(source_vec->at(index + start_index) * pow(256, index));
    return res;
}

size_t util::vecToULL(const std::shared_ptr<std::vector<int> > &source_vec, size_t start_index, int length) {
    if (start_index >= source_vec->size() or start_index + length > source_vec->size())
        throw std::out_of_range("position is greater than vector' s size. to ull ver.");

    size_t res{0};
    for (int index{0}; index < length; index++)
        res += static_cast<size_t>(source_vec->at(index + start_index) * pow(256, index));
    return res;
}

long long util::vecToLL(const std::shared_ptr<std::vector<int> > &source_vec, size_t start_index, int length) {
    if (start_index >= source_vec->size() or start_index + length > source_vec->size())
        throw std::out_of_range("position is greater than vector' s size. to ll ver.");

    long long res{0};
    for (int index{0}; index < length; index++)
        res += static_cast<long long>(source_vec->at(index + start_index) * pow(256, index));
    return res;
}

long long util::vecToLong(const std::shared_ptr<std::vector<int> > &source_vec, size_t start_index, int length) {
    if (start_index >= source_vec->size() or start_index + length > source_vec->size())
        throw std::out_of_range("position is greater than vector' s size. to long ver.");

    long res{0};
    for (int index{0}; index < length; index++)
        res += static_cast<long>(source_vec->at(index + start_index) * pow(256, index));
    return res;
}

int util::vecToInt(const std::shared_ptr<std::vector<int> > &source_vec, size_t start_index, int length) {
    if (start_index >= source_vec->size() or start_index + length > source_vec->size())
        throw std::out_of_range("position is greater than vector' s size. to int ver.");

    int res{0};
    for (int index{0}; index < length; index++)
        res += static_cast<int>(source_vec->at(index + start_index) * pow(256, index));
    return res;
}

void util::pushTime(const unique_ptr<vector<int>> &dest_vec, const string &time_str) {
    int year{stoi(time_str.substr(0, 4), nullptr, 10) - 2000};
    int mouth{stoi(time_str.substr(5, 2), nullptr, 10)};
    int mDay{stoi(time_str.substr(8, 2), nullptr, 10)};
    int hour{stoi(time_str.substr(11, 2), nullptr, 10)};
    int min{stoi(time_str.substr(14, 2), nullptr, 10)};
    int second{stoi(time_str.substr(17, 2) + time_str.substr(20, 3), nullptr, 10)};

    int wDay{0};
    string wDay_str = time_str.substr(24, 3);

    if (wDay_str == "Mon")
        wDay = 1;
    else if (wDay_str == "Tue")
        wDay = 2;
    else if (wDay_str == "Wed")
        wDay = 3;
    else if (wDay_str == "Thu")
        wDay = 4;
    else if (wDay_str == "Fri")
        wDay = 5;
    else if (wDay_str == "Sat")
        wDay = 6;
    else if (wDay_str == "Sun")
        wDay = 7;

    dest_vec->push_back(second % 256);          // 毫秒低位
    dest_vec->push_back(second / 256);          // 毫秒高位
    dest_vec->push_back(min);                   // IV位默认为0，即有效
    dest_vec->push_back(hour);                  // SU位默认为0，即标准时间
    dest_vec->push_back(wDay * 32 + mDay);
    dest_vec->push_back(mouth);
    dest_vec->push_back(year);
}

void util::replaceTime(const unique_ptr<vector<int>> &dest_vec, size_t start_index, const string &time_str) {
    int year{stoi(time_str.substr(0, 4), nullptr, 10) - 2000};
    int mouth{stoi(time_str.substr(5, 2), nullptr, 10)};
    int mDay{stoi(time_str.substr(8, 2), nullptr, 10)};
    int hour{stoi(time_str.substr(11, 2), nullptr, 10)};
    int min{stoi(time_str.substr(14, 2), nullptr, 10)};
    int second{stoi(time_str.substr(17, 2) + time_str.substr(20, 3), nullptr, 10)};

    int wDay{0};
    string wDay_str = time_str.substr(24, 3);

    if (wDay_str == "Mon")
        wDay = 1;
    else if (wDay_str == "Tue")
        wDay = 2;
    else if (wDay_str == "Wed")
        wDay = 3;
    else if (wDay_str == "Thu")
        wDay = 4;
    else if (wDay_str == "Fri")
        wDay = 5;
    else if (wDay_str == "Sat")
        wDay = 6;
    else if (wDay_str == "Sun")
        wDay = 7;

    size_t index = start_index;

    dest_vec->at(index++) = second % 256;          // 毫秒低位
    dest_vec->at(index++) = second / 256;          // 毫秒高位
    dest_vec->at(index++) = min;                   // IV位默认为0，即有效
    dest_vec->at(index++) = hour;                  // SU位默认为0，即标准时间
    dest_vec->at(index++) = wDay * 32 + mDay;
    dest_vec->at(index++) = mouth;
    dest_vec->at(index++) = year;
}

void util::pushIeee574(const unique_ptr<vector<int>> &dest_vec, float source) {
    unsigned char value[4];
    memcpy(value, &source, 4);
    for (const auto &val: value)
        dest_vec->push_back(static_cast<int>(val));
}

void util::replaceIeee574(const unique_ptr<vector<int>> &dest_vec, size_t start_index, float source) {
    unsigned char value[4];
    memcpy(value, &source, 4);
    size_t index = start_index;
    for (const auto &val: value)
        dest_vec->at(index++) = static_cast<int>(val);
}

string util::vecToHexStr(const unique_ptr<vector<int>> &source_vec, size_t start_index, int length) {
    if (start_index >= source_vec->size() or start_index + length > source_vec->size())
        throw std::out_of_range("position is greater than vector' s size. to int ver.");

    string res;
    for (int index{static_cast<int>(start_index)}; index < start_index + length; index++)
        res += util::decToHex(source_vec->at(index), 2) + " ";
    return res;
}

void util::pushString(const unique_ptr<vector<int>> &dest_vec, const string &str) {
    vector<int> temp{util::splitToInt(str, " ")};
    dest_vec->insert(dest_vec->end(), temp.begin(), temp.end());
}

vector<int> util::stringToInt(const string &str) {
    vector<int> res;

    for (const auto &val: str)
        res.push_back(static_cast<int>(val));

    return res;
}

string util::vecToHexStr(const shared_ptr<vector<int>> &source_vec, size_t start_index, int length) {
    if (start_index >= source_vec->size() or start_index + length > source_vec->size())
        throw std::out_of_range("position is greater than vector' s size. to int ver.");

    string res;
    for (int index{static_cast<int>(start_index)}; index < start_index + length; index++)
        res += util::decToHex(source_vec->at(index), 2) + " ";
    return res;
}

void util::pushElems(const shared_ptr<vector<int>> &dest_vec, int length, size_t source) {
    for (int n{0}; n < length; n++)
        dest_vec->push_back(static_cast<int>(source / static_cast<int>(pow(256, n))));
}

void util::pushElems(const shared_ptr<vector<int>> &dest_vec, int length, long long int source) {
    for (int n{0}; n < length; n++)
        dest_vec->push_back(static_cast<int>(source / static_cast<int>(pow(256, n))));
}

void util::pushElems(const shared_ptr<vector<int>> &dest_vec, int length, long source) {
    for (int n{0}; n < length; n++)
        dest_vec->push_back(static_cast<int>(source / static_cast<int>(pow(256, n))));
}

void util::pushElems(const shared_ptr<vector<int>> &dest_vec, int length, int source) {
    for (int n{0}; n < length; n++)
        dest_vec->push_back(static_cast<int>(source / static_cast<int>(pow(256, n))));
}

void util::pushString(const shared_ptr<vector<int>> &dest_vec, const string &str) {
    vector<int> temp{util::splitToInt(str, " ")};
    dest_vec->insert(dest_vec->end(), temp.begin(), temp.end());
}

void util::replaceElems(const shared_ptr<vector<int>> &dest_vec, size_t start_index, int length, size_t source) {
    if (start_index >= dest_vec->size() or start_index + length > dest_vec->size())
        throw std::out_of_range("position is greater than vector' s size. replace ull ver.");

    for (int n{0}; n < length; n++)
        dest_vec->at(start_index + n) = static_cast<int>(source / static_cast<int>(pow(256, n)));
}

void util::replaceElems(const shared_ptr<vector<int>> &dest_vec, size_t start_index, int length, long long int source) {
    if (start_index >= dest_vec->size() or start_index + length > dest_vec->size())
        throw std::out_of_range("position is greater than vector' s size. replace ull ver.");

    for (int n{0}; n < length; n++)
        dest_vec->at(start_index + n) = static_cast<int>(source / static_cast<int>(pow(256, n)));
}

void util::replaceElems(const shared_ptr<vector<int>> &dest_vec, size_t start_index, int length, long source) {
    if (start_index >= dest_vec->size() or start_index + length > dest_vec->size())
        throw std::out_of_range("position is greater than vector' s size. replace ull ver.");

    for (int n{0}; n < length; n++)
        dest_vec->at(start_index + n) = static_cast<int>(source / static_cast<int>(pow(256, n)));
}

void util::replaceElems(const shared_ptr<vector<int>> &dest_vec, size_t start_index, int length, int source) {
    if (start_index >= dest_vec->size() or start_index + length > dest_vec->size())
        throw std::out_of_range("position is greater than vector' s size. replace ull ver.");

    for (int n{0}; n < length; n++)
        dest_vec->at(start_index + n) = static_cast<int>(source / static_cast<int>(pow(256, n)));
}
