//
// Created by Astria on 2022/4/25.
//

#include "FileService.h"

#include <memory>
#include "file_service/FileElem.h"

namespace asdu {
    FileService::FileService(unique_ptr<vector<int>> data, const ASDULengthSet &als)
            : BaseASDU(move(data), als) {
        switch (cot) {
            case 5:
            case 6:
            case 7:
                break;
            default:
                errInfo += "传送原因错误";
                break;
        }

        size_t cIndex{static_cast<size_t>(asduLength.type +
                                          asduLength.qualifier +
                                          asduLength.cot +
                                          asduLength.commonAd)};

        cIndex += asduLength.objectAd;

        _aType = data->at(cIndex++);
        _oType = data->at(cIndex++);

        switch (_oType) {
            case 1: {
                size_t dir_id{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                                  data->at(cIndex + 2) * 65536 +
                                                  data->at(cIndex + 1) * 256 +
                                                  data->at(cIndex))};

                int dir_length{data->at(cIndex + 4)};
                cIndex += 5;

                string dir_name{util::intToString(util::vecMid(data, cIndex, dir_length))};

                cIndex += dir_length;

                bool flag{data->at(cIndex++) == 1};

                string start_time{util::parseTime(util::vecMid(*data, cIndex, 7))};

                string end_time{util::parseTime(util::vecMid(*data, cIndex + 7, 7))};
#if __cplusplus >= 201403L
                _dc_imp = make_unique<DirCall>(dir_id, dir_name, flag, start_time, end_time);
#elif __cplusplus < 201403L
                _dc_imp = unique_ptr<DirCall>(new DirCall(dir_id, dir_name, flag, start_time, end_time));
#endif
                _dc_imp->name_length = dir_length;
                break;
            }
            case 2: {
                bool res{data->at(cIndex++) == 0};

                size_t dir_id{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                                  data->at(cIndex + 2) * 65536 +
                                                  data->at(cIndex + 1) * 256 +
                                                  data->at(cIndex))};

                cIndex += 4;

                bool flag{data->at(cIndex++) == 1};
                int file_count{data->at(cIndex++)};

#if __cplusplus >= 201403L
                _dcc_imp = make_unique<DirCallConfirm>(res, dir_id, flag);
#elif __cplusplus < 201403L
                _dcc_imp = unique_ptr<DirCallConfirm>(new DirCallConfirm(res, dir_id, flag));
#endif
                for (int index{1}; index <= file_count; index++) {
                    int file_name_length{data->at(cIndex++)};

                    string file_name{util::intToString(util::vecMid(data, cIndex, file_name_length))};

                    cIndex += file_name_length;

                    int property{data->at(cIndex++)};

                    size_t file_size{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                                         data->at(cIndex + 2) * 65536 +
                                                         data->at(cIndex + 1) * 256 +
                                                         data->at(cIndex))};

                    cIndex += 4;

                    string file_time{util::parseTime(util::vecMid(data, cIndex, 7))};

                    cIndex += 7;

                    FileElem elem(file_name, file_size, 0, file_time, property);
                    elem.name_length = file_name_length;
                    _dcc_imp->add_file(elem);
                }
                break;
            }
            case 3: {
                int file_length{data->at(cIndex++)};
                string file_name = util::intToString(util::vecMid(data, cIndex, file_length));

#if __cplusplus >= 201403L
                _rfa_imp = make_unique<ReadFileActive>(file_name);
#elif __cplusplus < 201403L
                _rfa_imp = unique_ptr<ReadFileActive>(new ReadFileActive(file_name));
#endif
                _rfa_imp->file_length = file_length;
                break;
            }
            case 4: {

                bool res{data->at(cIndex++) == 0};

                int file_length{data->at(cIndex++)};
                string file_name = util::intToString(util::vecMid(data, cIndex, file_length));

                cIndex += file_length;

                size_t id{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                              data->at(cIndex + 2) * 65536 +
                                              data->at(cIndex + 1) * 256 +
                                              data->at(cIndex))};

                cIndex += 4;

                size_t file_size{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                                     data->at(cIndex + 2) * 65536 +
                                                     data->at(cIndex + 1) * 256 +
                                                     data->at(cIndex))};
                FileElem file(file_name, file_size, id);
                file.name_length = file_length;
#if __cplusplus >= 201403L
                _rfac_imp = make_unique<ReadFileActiveConfirm>(res, file);
#elif __cplusplus < 201403L
                _rfac_imp = unique_ptr<ReadFileActiveConfirm>(new ReadFileActiveConfirm(res, file));
#endif
                break;
            }
            case 5: {
                size_t id{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                              data->at(cIndex + 2) * 65536 +
                                              data->at(cIndex + 1) * 256 +
                                              data->at(cIndex))};

                cIndex += 4;

                size_t number{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                                  data->at(cIndex + 2) * 65536 +
                                                  data->at(cIndex + 1) * 256 +
                                                  data->at(cIndex))};

                cIndex += 4;

                bool flag{data->at(cIndex++) == 1};

                string data_str{
                        util::intToString(util::vecMid(data, cIndex, data->size() - cIndex - 1))};
#if __cplusplus >= 201403L
                _rfd_imp = make_unique<ReadFileData>(id, number, flag, data_str);
#elif __cplusplus < 201403L
                _rfd_imp = unique_ptr<ReadFileData>(new ReadFileData(id, number, flag, data_str));
#endif
                _rfd_imp->_data_length = static_cast<int>(data->size() - cIndex - 1);
                break;
            }
            case 6: {
                size_t id{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                              data->at(cIndex + 2) * 65536 +
                                              data->at(cIndex + 1) * 256 +
                                              data->at(cIndex))};

                cIndex += 4;

                size_t number{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                                  data->at(cIndex + 2) * 65536 +
                                                  data->at(cIndex + 1) * 256 +
                                                  data->at(cIndex))};
                cIndex += 4;

                bool flag{data->at(cIndex) == 1};
#if __cplusplus >= 201403L
                _rfdc_imp = make_unique<ReadFileDataConfirm>(id, number, flag);
#elif __cplusplus < 201403L
                _rfdc_imp = unique_ptr<ReadFileDataConfirm>(new ReadFileDataConfirm(id, number, flag));
#endif
                break;
            }
            case 7: {
                int file_length{data->at(cIndex++)};
                string file_name{util::intToString(util::vecMid(data, cIndex, file_length))};

                cIndex += file_length;

                size_t id{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                              data->at(cIndex + 2) * 65536 +
                                              data->at(cIndex + 1) * 256 +
                                              data->at(cIndex))};
                cIndex += 4;

                size_t size{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                                data->at(cIndex + 2) * 65536 +
                                                data->at(cIndex + 1) * 256 +
                                                data->at(cIndex))};

                FileElem file(file_name, size, id);
                file.name_length = file_length;
#if __cplusplus >= 201403L
                _wfa_imp = make_unique<WriteFileActive>(file);
#elif __cplusplus < 201403L
                _wfa_imp = unique_ptr<WriteFileActive>(new WriteFileActive(file));
#endif
                break;
            }
            case 8: {
                int result{data->at(cIndex++)};

                int file_length{data->at(cIndex++)};
                string file_name{util::intToString(util::vecMid(data, cIndex, file_length))};

                cIndex += file_length;

                size_t id{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                              data->at(cIndex + 2) * 65536 +
                                              data->at(cIndex + 1) * 256 +
                                              data->at(cIndex))};
                cIndex += 4;

                size_t size{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                                data->at(cIndex + 2) * 65536 +
                                                data->at(cIndex + 1) * 256 +
                                                data->at(cIndex))};

                FileElem file(file_name, size, id);
                file.name_length = file_length;
#if __cplusplus >= 201403L
                _wfac_imp = make_unique<WriteFileActiveConfirm>(result, file);
#elif __cplusplus < 201403L
                _wfac_imp = unique_ptr<WriteFileActiveConfirm>(new WriteFileActiveConfirm(result, file));
#endif
                break;
            }
            case 9: {
                size_t id{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                              data->at(cIndex + 2) * 65536 +
                                              data->at(cIndex + 1) * 256 +
                                              data->at(cIndex))};
                cIndex += 4;

                size_t number{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                                  data->at(cIndex + 2) * 65536 +
                                                  data->at(cIndex + 1) * 256 +
                                                  data->at(cIndex))};
                cIndex += 4;

                bool flag{data->at(cIndex++) == 1};

                string data_str{
                        util::intToString(util::vecMid(data, cIndex, data->size() - cIndex - 1))};
#if __cplusplus >= 201403L
                _wfd_imp = make_unique<WriteFileData>(id, number, flag, data_str);
#elif __cplusplus < 201403L
                _wfd_imp = unique_ptr<WriteFileData>(new WriteFileData(id, number, flag, data_str));
#endif
                _wfd_imp->_data_length = static_cast<int>(data->size() - cIndex - 1);
                break;
            }
            case 10: {
                size_t id{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                              data->at(cIndex + 2) * 65536 +
                                              data->at(cIndex + 1) * 256 +
                                              data->at(cIndex))};
                cIndex += 4;

                size_t number{static_cast<size_t>(data->at(cIndex + 3) * 16777216 +
                                                  data->at(cIndex + 2) * 65536 +
                                                  data->at(cIndex + 1) * 256 +
                                                  data->at(cIndex))};
                cIndex += 4;

                int result{data->at(cIndex)};
#if __cplusplus >= 201403L
                _wfdc_imp = make_unique<WriteFileDataConfirm>(id, number, result);
#elif __cplusplus < 201403L
                _wfdc_imp = unique_ptr<WriteFileDataConfirm>(new WriteFileDataConfirm(id, number, result));
#endif
                break;
            }
            default:
                errInfo += "文件操作标识错误";
                break;
        }
    }

    shared_ptr<json> FileService::toJson() {
        auto object = make_shared<json>();
        (*object)["ASDU报文类型"] = "文件服务报文";
        switch (cot) {
            case 5:
                (*object)["传送原因"] = "请求/被请求";
                break;
            case 6:
                (*object)["传送原因"] = "激活";
                (*object)["报文方向"] = "控制方向";
                break;
            case 7:
                (*object)["传送原因"] = "激活确认";
                (*object)["报文方向"] = "监视方向";
                break;
            default:
                (*object)["传送原因"] = "Data Error";
                break;
        }
        (*object)["公共地址"] = cAddr;
        (*object)["信息体地址"] = oAddr;
        switch (_aType) {
            case 1:
            case 3:
            case 4:
                (*object)["附加数据包类型"] = "备用";
                break;
            case 2:
                (*object)["附加数据包类型"] = "文件传输";
                break;
            default:
                (*object)["附加数据包类型"] = "Data Error";
                break;
        }
#if __cplusplus >= 201403L
        auto fileInfo = make_unique<json>();
#elif __cplusplus < 201403L
        auto fileInfo = unique_ptr<json>(new json);
#endif
        switch (_oType) {
            case 1: {
                if (!_dc_imp) {
                    errInfo += "文件服务信息对象初始化错误";
                    break;
                }
                (*fileInfo)["操作标识"] = "文件目录召唤";
                (*fileInfo)["目录ID"] = _dc_imp->_id;
                (*fileInfo)["目录名"] = _dc_imp->_name;
                (*fileInfo)["召唤标志"] = (_dc_imp->_flag ? "目录下满足搜索时间段的文件" : "目录下所有文件");
                (*fileInfo)["查询起始时间"] = _dc_imp->_sTime;
                (*fileInfo)["查询终止时间"] = _dc_imp->_eTime;
                break;
            }
            case 2: {
                if (!_dcc_imp) {
                    errInfo += "文件服务信息对象初始化错误";
                    break;
                }
                (*fileInfo)["操作标识"] = "目录召唤确认";
                (*fileInfo)["结果"] = (_dcc_imp->_res ? "成功" : "失败");
                (*fileInfo)["目录ID"] = _dcc_imp->_id;
                (*fileInfo)["后续标志"] = (_dcc_imp->_flag ? "有后续" : "无后续");
                (*fileInfo)["文件数量"] = _dcc_imp->_count;

#if __cplusplus >= 201403L
                auto file_list = make_unique<json>();
#elif __cplusplus < 201403L
                auto file_list = unique_ptr<json>(new json);
#endif
                size_t index{1ULL};
                for (const auto &fElem: *(_dcc_imp->_files)) {
                    json file_elem;
                    file_elem["文件名称"] = fElem.name;
                    file_elem["文件属性"] = fElem._property;
                    file_elem["文件大小"] = fElem._size;
                    file_elem["文件时间"] = fElem._time;

                    (*file_list)["文件" + std::to_string(index++)] = file_elem;
                }

                (*fileInfo)["文件列表"] = *file_list;
                break;
            }
            case 3: {
                if (!_rfa_imp) {
                    errInfo += "文件服务信息对象初始化错误";
                    break;
                }
                (*fileInfo)["操作标识"] = "读文件激活";
                (*fileInfo)["文件名称"] = _rfa_imp->_file;
                break;
            }
            case 4: {
                if (!_rfac_imp) {
                    errInfo += "文件服务信息对象初始化错误";
                    break;
                }
                (*fileInfo)["操作标识"] = "读文件激活确认";
                (*fileInfo)["结果"] = (_rfac_imp->_res ? "成功" : "失败");
                (*fileInfo)["文件名字"] = _rfac_imp->_file.name;
                (*fileInfo)["文件ID"] = _rfac_imp->_file._id;
                (*fileInfo)["文件大小"] = _rfac_imp->_file._size;
                break;
            }
            case 5: {
                if (!_rfd_imp) {
                    errInfo += "文件服务信息对象初始化错误";
                    break;
                }
                (*fileInfo)["操作标识"] = "读文件数据传输";
                (*fileInfo)["文件ID"] = _rfd_imp->_id;
                (*fileInfo)["数据段号"] = _rfd_imp->_number;
                (*fileInfo)["后续标志"] = (_rfd_imp->_flag ? "有后续" : "无后续");
                (*fileInfo)["文件数据"] = _rfd_imp->_data;
                break;
            }
            case 6: {
                if (!_rfdc_imp) {
                    errInfo += "文件服务信息对象初始化错误";
                    break;
                }
                (*fileInfo)["操作标识"] = "读文件数据传输确认";
                (*fileInfo)["文件ID"] = _rfdc_imp->_id;
                (*fileInfo)["数据段号"] = _rfdc_imp->_number;
                (*fileInfo)["结果"] = (_rfdc_imp->_flag ? "有后续" : "无后续");
                break;
            }
            case 7: {
                if (!_wfa_imp) {
                    errInfo += "文件服务信息对象初始化错误";
                    break;
                }
                (*fileInfo)["操作标识"] = "写文件激活";
                (*fileInfo)["文件名字"] = _wfa_imp->_file.name;
                (*fileInfo)["文件ID"] = _wfa_imp->_file._id;
                (*fileInfo)["文件大小"] = _wfa_imp->_file._size;
                break;
            }
            case 8: {
                if (!_wfac_imp) {
                    errInfo += "文件服务信息对象初始化错误";
                    break;
                }
                (*fileInfo)["操作标识"] = "写文件激活确认";
                switch (_wfac_imp->_res) {
                    case 0:
                        (*fileInfo)["结果"] = "成功";
                        break;
                    case 1:
                        (*fileInfo)["结果"] = "未知错误";
                        break;
                    case 3:
                        (*fileInfo)["结果"] = "文件名不支持";
                        break;
                    case 4:
                        (*fileInfo)["结果"] = "长度超范围";
                }
                (*fileInfo)["文件名字"] = _wfac_imp->_file.name;
                (*fileInfo)["文件ID"] = _wfac_imp->_file._id;
                (*fileInfo)["文件大小"] = _wfac_imp->_file._size;
                break;
            }
            case 9: {
                if (!_wfd_imp) {
                    errInfo += "文件服务信息对象初始化错误";
                    break;
                }
                (*fileInfo)["操作标识"] = "写文件数据传输";
                (*fileInfo)["文件ID"] = _wfd_imp->_id;
                (*fileInfo)["数据段号"] = _wfd_imp->_number;
                (*fileInfo)["后续标志"] = (_wfd_imp->_flag ? "有后续" : "无后续");
                (*fileInfo)["文件数据"] = _wfd_imp->_data;
                break;
            }
            case 10: {
                if (!_wfdc_imp) {
                    errInfo += "文件服务信息对象初始化错误";
                    break;
                }
                (*fileInfo)["操作标识"] = "写文件数据传输确认";
                (*fileInfo)["文件ID"] = _wfdc_imp->_id;
                (*fileInfo)["数据段号"] = _wfdc_imp->_number;
                switch (_wfdc_imp->_res) {
                    case 0:
                        (*fileInfo)["结果"] = "成功";
                        break;
                    case 1:
                        (*fileInfo)["结果"] = "未知错误";
                        break;
                    case 2:
                        (*fileInfo)["结果"] = "校验和错误";
                        break;
                    case 3:
                        (*fileInfo)["结果"] = "文件长度不对应";
                        break;
                    case 4:
                        (*fileInfo)["结果"] = "文件ID与激活ID不一致";
                        break;
                    default:
                        break;
                }
                default:
                    errInfo += "文件服务信息对象操作标识符错误";
            }
        }

        (*object)["文件信息"] = *fileInfo;
        return object;
    }

    shared_ptr<json> FileService::getPosition() {
        auto object = make_shared<json>();

        size_t cIndex{static_cast<size_t>(asduLength.startIndex)};
        (*object)["ASDU报文类型"] = util::posJson(cIndex, cIndex + asduLength.type);
        (*object)["标识类型"] = util::posJson(cIndex, cIndex + asduLength.type);

        cIndex += asduLength.type + asduLength.qualifier;
        (*object)["传送原因"] = util::posJson(cIndex, cIndex + asduLength.cot);

        cIndex += asduLength.cot;
        (*object)["公共地址"] = util::posJson(cIndex, cIndex + asduLength.commonAd);

        cIndex += asduLength.commonAd;
        (*object)["信息体地址"] = util::posJson(cIndex, cIndex + asduLength.objectAd);

        cIndex += asduLength.objectAd;
        (*object)["附加数据包类型"] = util::posJson(cIndex++, 1);

        (*object)["操作标识"] = util::posJson(cIndex++, 1);
        size_t mIndex{cIndex};

        auto fPosition = unique_ptr<json>(new json);
        switch (_oType) {
            case 1: {
                if (!_dc_imp)
                    break;

                (*fPosition)["目录ID"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 5;

                (*fPosition)["目录名"] = util::posJson(cIndex, cIndex + _dc_imp->name_length);
                cIndex += _dc_imp->name_length;

                (*fPosition)["召唤标志"] = util::posJson(cIndex, cIndex + 1);
                cIndex++;

                (*fPosition)["查询起始时间"] = util::posJson(cIndex, cIndex + 7);
                cIndex += 7;

                (*fPosition)["查询终止时间"] = util::posJson(cIndex, cIndex + 7);
                cIndex += 7;
                break;
            }
            case 2: {
                if (!_dcc_imp)
                    break;

                (*fPosition)["结果"] = util::posJson(cIndex, cIndex + 1);
                cIndex++;

                (*fPosition)["目录ID"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;

                (*fPosition)["后续标志"] = util::posJson(cIndex, cIndex + 1);
                cIndex++;

                (*fPosition)["文件数量"] = util::posJson(cIndex, cIndex + 1);
                cIndex++;

                size_t mark1{cIndex};
                auto file_list = unique_ptr<json>(new json);
                size_t fIndex{1ULL};
                for (const auto &fElem: *(_dcc_imp->_files)) {
                    size_t mark2{cIndex++};
                    json file_elem;
                    file_elem["文件名称"] = util::posJson(cIndex, cIndex + fElem.name_length);

                    cIndex += fElem.name_length;
                    file_elem["文件属性"] = util::posJson(cIndex, cIndex + 1);

                    cIndex++;
                    file_elem["文件大小"] = util::posJson(cIndex, cIndex + 4);

                    cIndex += 4;
                    file_elem["文件时间"] = util::posJson(cIndex, cIndex + 7);
                    cIndex += 7;

                    (*file_list)["文件" + std::to_string(fIndex++)] = file_elem;
                    (*file_list)["文件" + std::to_string(fIndex++) + "位置"] = util::posJson(mark2, cIndex);
                }

                (*fPosition)["文件列表"] = *file_list;
                (*fPosition)["文件列表位置"] = util::posJson(mark1, cIndex);
                break;
            }
            case 3: {
                if (!_rfa_imp)
                    break;
                cIndex++;
                (*fPosition)["文件名称"] = util::posJson(cIndex, cIndex + _rfa_imp->file_length);
                cIndex += _rfa_imp->file_length;
                break;
            }
            case 4: {
                if (!_rfac_imp)
                    break;

                (*fPosition)["结果"] = util::posJson(cIndex, cIndex + 1);
                cIndex += 2;

                (*fPosition)["文件名字"] = util::posJson(cIndex, cIndex + _rfac_imp->_file.name_length);
                cIndex += _rfac_imp->_file.name_length;

                (*fPosition)["文件ID"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;

                (*fPosition)["文件大小"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;
                break;
            }
            case 5: {
                if (!_rfd_imp)
                    errInfo += "文件服务信息对象初始化错误";

                (*fPosition)["文件ID"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;

                (*fPosition)["数据段号"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;

                (*fPosition)["后续标志"] = util::posJson(cIndex, cIndex + 1);
                cIndex++;

                (*fPosition)["文件数据"] = util::posJson(cIndex, cIndex + _rfd_imp->_data_length);
                cIndex += _rfd_imp->_data_length + 1;
                break;
            }
            case 6: {
                if (!_rfdc_imp)
                    break;

                (*fPosition)["文件ID"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;

                (*fPosition)["数据段号"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;

                (*fPosition)["后续标志"] = util::posJson(cIndex, cIndex + 1);
                cIndex++;
                break;
            }
            case 7: {
                if (!_wfa_imp)
                    break;
                cIndex++;
                (*fPosition)["文件名字"] = util::posJson(cIndex, cIndex + _wfa_imp->_file.name_length);
                cIndex += _wfa_imp->_file.name_length;

                (*fPosition)["文件ID"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;

                (*fPosition)["文件大小"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;
                break;
            }
            case 8: {
                if (!_wfac_imp)
                    break;

                (*fPosition)["结果"] = util::posJson(cIndex, cIndex + 1);
                cIndex += 2;

                (*fPosition)["文件名字"] = util::posJson(cIndex, cIndex + _wfac_imp->_file.name_length);
                cIndex += _wfac_imp->_file.name_length;

                (*fPosition)["文件ID"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;

                (*fPosition)["文件大小"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;
                break;
            }
            case 9: {
                if (!_wfd_imp)
                    break;

                (*fPosition)["文件ID"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;

                (*fPosition)["数据段号"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;

                (*fPosition)["后续标志"] = util::posJson(cIndex, cIndex + 1);
                cIndex++;

                (*fPosition)["文件数据"] = util::posJson(cIndex, _wfd_imp->_data_length);
                cIndex += _wfd_imp->_data_length + 1;
                break;
            }
            case 10: {
                if (!_wfdc_imp)
                    break;

                (*fPosition)["文件ID"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;

                (*fPosition)["数据段号"] = util::posJson(cIndex, cIndex + 4);
                cIndex += 4;

                (*fPosition)["结果"] = util::posJson(cIndex, cIndex + 1);
                cIndex++;
            }
            default:
                break;
        }

        (*object)["文件信息"] = *fPosition;
        (*object)["文件信息位置"] = util::posJson(mIndex, cIndex);
        return object;
    }

    FileService::FileService(const ASDULengthSet &asdu_length_set)
            : BaseASDU(asdu_length_set) {
        for (int i = 1; i <= asduLength.objectAd + 2; i++)
            data->push_back(0);

        tie = 210;
        util::replaceElems(data, 0, asduLength.type, tie);
    }

    FileService::FileService(const ASDULengthSet &asdu_length_set, long long int tie, long long int vsq,
                             long long int cot, size_t command_address, size_t object_address, int addon, int operate)
            : BaseASDU(asdu_length_set, tie, vsq, cot, command_address, object_address), _aType(addon),
              _oType(operate) {
        data->push_back(_aType);
        data->push_back(_oType);
    }

    bool FileService::set_dir_call(const DirCall &dcImp) {
        if (_oType == 1) {
#if __cplusplus >= 201403L
            _dc_imp = make_unique<DirCall>(dcImp);
#elif __cplusplus < 201403L
            _dc_imp = unique_ptr<DirCall>(new DirCall(dcImp));
#endif
            return true;
        } else
            return false;
    }

    bool FileService::set_dir_call(unique_ptr<DirCall> dcImp) {
        if (_oType == 1) {
            _dc_imp = move(dcImp);
            return true;
        } else
            return false;
    }

    bool FileService::set_dir_call_confirm(const DirCallConfirm &dccImp) {
        if (_oType == 2) {
#if __cplusplus >= 201403L
            _dcc_imp = make_unique<DirCallConfirm>(dccImp);
#elif __cplusplus < 201403L
            _dcc_imp = unique_ptr<DirCallConfirm>(new DirCallConfirm(dccImp));
#endif
            return true;
        } else
            return false;
    }

    bool FileService::set_dir_call_confirm(unique_ptr<DirCallConfirm> dccImp) {
        if (_oType == 2) {
            _dcc_imp = move(dccImp);
            return true;
        } else
            return false;
    }

    bool FileService::set_read_active(const ReadFileActive &rfaImp) {
        if (_oType == 3) {
#if __cplusplus >= 201403L
            _rfa_imp = make_unique<ReadFileActive>(rfaImp);
#elif __cplusplus < 201403L
            _rfa_imp = unique_ptr<ReadFileActive>(new ReadFileActive(rfaImp));
#endif
            return true;
        } else
            return false;
    }

    bool FileService::set_read_active(unique_ptr<ReadFileActive> rfaImp) {
        if (_oType == 3) {
            _rfa_imp = move(rfaImp);
            return true;
        } else
            return false;
    }

    bool FileService::set_read_active_confirm(const ReadFileActiveConfirm &rfacImp) {
        if (_oType == 4) {
#if __cplusplus >= 201403L
            _rfac_imp = make_unique<ReadFileActiveConfirm>(rfacImp);
#elif __cplusplus < 201403L
            _rfac_imp = unique_ptr<ReadFileActiveConfirm>(new ReadFileActiveConfirm(rfacImp));
#endif
            return true;
        } else
            return false;
    }

    bool FileService::set_read_active_confirm(unique_ptr<ReadFileActiveConfirm> rfacImp) {
        if (_oType == 4) {
            _rfac_imp = move(rfacImp);
            return true;
        } else
            return false;
    }

    bool FileService::set_read_data(const ReadFileData &rfdImp) {
        if (_oType == 5) {
#if __cplusplus >= 201403L
            _rfd_imp = make_unique<ReadFileData>(rfdImp);
#elif __cplusplus < 201403L
            _rfd_imp = unique_ptr<ReadFileData>(new ReadFileData(rfdImp));
#endif
            return true;
        } else
            return false;
    }

    bool FileService::set_read_data(unique_ptr<ReadFileData> rfdImp) {
        if (_oType == 5) {
            _rfd_imp = move(rfdImp);
            return true;
        } else
            return false;
    }

    bool FileService::set_read_data_confirm(const ReadFileDataConfirm &rfdcImp) {
        if (_oType == 6) {
#if __cplusplus >= 201403L
            _rfdc_imp = make_unique<ReadFileDataConfirm>(rfdcImp);
#elif __cplusplus < 201403L
            _rfdc_imp = unique_ptr<ReadFileDataConfirm>(new ReadFileDataConfirm(rfdcImp));
#endif
            return true;
        } else
            return false;
    }

    bool FileService::set_read_data_confirm(unique_ptr<ReadFileDataConfirm> rfdcImp) {
        if (_oType == 6) {
            _rfdc_imp = move(rfdcImp);
            return true;
        } else
            return false;
    }

    bool FileService::set_write_active(const WriteFileActive &wfaImp) {
        if (_oType == 7) {
#if __cplusplus >= 201403L
            _wfa_imp = make_unique<WriteFileActive>(wfaImp);
#elif __cplusplus < 201403L
            _wfa_imp = unique_ptr<WriteFileActive>(new WriteFileActive(wfaImp));
#endif
            return true;
        } else
            return false;
    }

    bool FileService::set_write_active(unique_ptr<WriteFileActive> wfaImp) {
        if (_oType == 7) {
            _wfa_imp = move(wfaImp);
            return true;
        } else
            return false;
    }

    bool FileService::set_write_active_confirm(const WriteFileActiveConfirm &wfacImp) {
        if (_oType == 8) {
#if __cplusplus >= 201403L
            _wfac_imp = make_unique<WriteFileActiveConfirm>(wfacImp);
#elif __cplusplus < 201403L
            _wfac_imp = unique_ptr<WriteFileActiveConfirm>(new WriteFileActiveConfirm(wfacImp));
#endif
            return true;
        } else
            return false;
    }

    bool FileService::set_write_active_confirm(unique_ptr<WriteFileActiveConfirm> wfacImp) {
        if (_oType == 8) {
            _wfac_imp = move(wfacImp);
            return true;
        } else
            return false;
    }

    bool FileService::set_write_data(const WriteFileData &wfdImp) {
        if (_oType == 9) {
#if __cplusplus >= 201403L
            _wfd_imp = make_unique<WriteFileData>(wfdImp);
#elif __cplusplus < 201403L
            _wfd_imp = unique_ptr<WriteFileData>(new WriteFileData(wfdImp));
#endif
            return true;
        } else
            return false;
    }

    bool FileService::set_write_data(unique_ptr<WriteFileData> wfdImp) {
        if (_oType == 9) {
            _wfd_imp = move(wfdImp);
            return true;
        } else
            return false;
    }

    bool FileService::set_write_data_confirm(const WriteFileDataConfirm &wfdcImp) {
        if (_oType == 10) {
#if __cplusplus >= 201403L
            _wfdc_imp = make_unique<WriteFileDataConfirm>(wfdcImp);
#elif __cplusplus < 201403L
            _wfdc_imp = unique_ptr<WriteFileDataConfirm>(new WriteFileDataConfirm(wfdcImp));
#endif
            return true;
        } else
            return false;
    }

    bool FileService::set_write_data_confirm(unique_ptr<WriteFileDataConfirm> wfdcImp) {
        if (_oType == 10) {
            _wfdc_imp = move(wfdcImp);
            return true;
        } else
            return false;
    }

    FileService::FileService(const FileService &rhs) : BaseASDU(rhs), _aType(rhs._aType), _oType(rhs._oType) {
        switch (_oType) {
            case 1: {
                if (rhs._dc_imp) {
#if __cplusplus >= 201403L
                    _dc_imp = make_unique<DirCall>(*rhs._dc_imp);
#elif __cplusplus < 201403L
                    _dc_imp = unique_ptr<DirCall>(new DirCall(*rhs._dc_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _dc_imp in nullptr." << std::endl;
                break;
            }
            case 2: {
                if (rhs._dcc_imp) {
#if __cplusplus >= 201403L
                    _dcc_imp = make_unique<DirCallConfirm>(*rhs._dcc_imp);
#elif __cplusplus < 201403L
                    _dcc_imp = unique_ptr<DirCallConfirm>(new DirCallConfirm(*rhs._dcc_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _dcc_imp in nullptr." << std::endl;
                break;
            }
            case 3: {
                if (rhs._rfa_imp) {
#if __cplusplus >= 201403L
                    _rfa_imp = make_unique<ReadFileActive>(*rhs._rfa_imp);
#elif __cplusplus < 201403L
                    _rfa_imp = unique_ptr<ReadFileActive>(new ReadFileActive(*rhs._rfa_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _rfa_imp in nullptr." << std::endl;
                break;
            }
            case 4: {
                if (rhs._rfac_imp) {
#if __cplusplus >= 201403L
                    _rfac_imp = make_unique<ReadFileActiveConfirm>(*rhs._rfac_imp);
#elif __cplusplus < 201403L
                    _rfac_imp = unique_ptr<ReadFileActiveConfirm>(new ReadFileActiveConfirm(*rhs._rfac_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _rfac_imp in nullptr." << std::endl;
                break;
            }
            case 5: {
                if (rhs._rfd_imp) {
#if __cplusplus >= 201403L
                    _rfd_imp = make_unique<ReadFileData>(*rhs._rfd_imp);
#elif __cplusplus < 201403L
                    _rfd_imp = unique_ptr<ReadFileData>(new ReadFileData(*rhs._rfd_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _rfd_imp in nullptr." << std::endl;
                break;
            }
            case 6: {
                if (rhs._rfdc_imp) {
#if __cplusplus >= 201403L
                    _rfdc_imp = make_unique<ReadFileDataConfirm>(*rhs._rfdc_imp);
#elif __cplusplus < 201403L
                    _rfdc_imp = unique_ptr<ReadFileDataConfirm>(new ReadFileDataConfirm(*rhs._rfdc_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _rfdc_imp in nullptr." << std::endl;
                break;
            }
            case 7: {
                if (rhs._wfa_imp) {
#if __cplusplus >= 201403L
                    _wfa_imp = make_unique<WriteFileActive>(*rhs._wfa_imp);
#elif __cplusplus < 201403L
                    _wfa_imp = unique_ptr<WriteFileActive>(new WriteFileActive(*rhs._wfa_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _wfa_imp in nullptr." << std::endl;
                break;
            }
            case 8: {
                if (rhs._wfac_imp) {
#if __cplusplus >= 201403L
                    _wfac_imp = make_unique<WriteFileActiveConfirm>(*rhs._wfac_imp);
#elif __cplusplus < 201403L
                    _wfac_imp = unique_ptr<WriteFileActiveConfirm>(new WriteFileActiveConfirm(*rhs._wfac_imp));
#endif

                }
                else
                    std::cerr << "FileService(const FileService&): _wfac_imp in nullptr." << std::endl;
                break;
            }
            case 9: {
                if (rhs._wfd_imp) {
#if __cplusplus >= 201403L
                    _wfd_imp = make_unique<WriteFileData>(*rhs._wfd_imp);
#elif __cplusplus < 201403L
                    _wfd_imp = unique_ptr<WriteFileData>(new WriteFileData(*rhs._wfd_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _wfd_imp in nullptr." << std::endl;
                break;
            }
            case 10: {
                if (rhs._wfdc_imp) {
#if __cplusplus >= 201403L
                    _wfdc_imp = make_unique<WriteFileDataConfirm>(*rhs._wfdc_imp);
#elif __cplusplus < 201403L
                    _wfdc_imp = unique_ptr<WriteFileDataConfirm>(new WriteFileDataConfirm(*rhs._wfdc_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _wfdc_imp in nullptr." << std::endl;
                break;
            }
            default:
                break;
        }
    }

    FileService &FileService::operator=(const FileService &rhs) {
        if (this == &rhs)
            return *this;

        BaseASDU::operator=(rhs);
        _aType = rhs._aType;
        _oType = rhs._oType;
        switch (_oType) {
            case 1: {
                if (rhs._dc_imp) {
#if __cplusplus >= 201403L
                    _dc_imp = make_unique<DirCall>(*rhs._dc_imp);
#elif __cplusplus < 201403L
                    _dc_imp = unique_ptr<DirCall>(new DirCall(*rhs._dc_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _dc_imp in nullptr." << std::endl;
                break;
            }
            case 2: {
                if (rhs._dcc_imp) {
#if __cplusplus >= 201403L
                    _dcc_imp = make_unique<DirCallConfirm>(*rhs._dcc_imp);
#elif __cplusplus < 201403L
                    _dcc_imp = unique_ptr<DirCallConfirm>(new DirCallConfirm(*rhs._dcc_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _dcc_imp in nullptr." << std::endl;
                break;
            }
            case 3: {
                if (rhs._rfa_imp) {
#if __cplusplus >= 201403L
                    _rfa_imp = make_unique<ReadFileActive>(*rhs._rfa_imp);
#elif __cplusplus < 201403L
                    _rfa_imp = unique_ptr<ReadFileActive>(new ReadFileActive(*rhs._rfa_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _rfa_imp in nullptr." << std::endl;
                break;
            }
            case 4: {
                if (rhs._rfac_imp) {
#if __cplusplus >= 201403L
                    _rfac_imp = make_unique<ReadFileActiveConfirm>(*rhs._rfac_imp);
#elif __cplusplus < 201403L
                    _rfac_imp = unique_ptr<ReadFileActiveConfirm>(new ReadFileActiveConfirm(*rhs._rfac_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _rfac_imp in nullptr." << std::endl;
                break;
            }
            case 5: {
                if (rhs._rfd_imp) {
#if __cplusplus >= 201403L
                    _rfd_imp = make_unique<ReadFileData>(*rhs._rfd_imp);
#elif __cplusplus < 201403L
                    _rfd_imp = unique_ptr<ReadFileData>(new ReadFileData(*rhs._rfd_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _rfd_imp in nullptr." << std::endl;
                break;
            }
            case 6: {
                if (rhs._rfdc_imp) {
#if __cplusplus >= 201403L
                    _rfdc_imp = make_unique<ReadFileDataConfirm>(*rhs._rfdc_imp);
#elif __cplusplus < 201403L
                    _rfdc_imp = unique_ptr<ReadFileDataConfirm>(new ReadFileDataConfirm(*rhs._rfdc_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _rfdc_imp in nullptr." << std::endl;
                break;
            }
            case 7: {
                if (rhs._wfa_imp) {
#if __cplusplus >= 201403L
                    _wfa_imp = make_unique<WriteFileActive>(*rhs._wfa_imp);
#elif __cplusplus < 201403L
                    _wfa_imp = unique_ptr<WriteFileActive>(new WriteFileActive(*rhs._wfa_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _wfa_imp in nullptr." << std::endl;
                break;
            }
            case 8: {
                if (rhs._wfac_imp) {
#if __cplusplus >= 201403L
                    _wfac_imp = make_unique<WriteFileActiveConfirm>(*rhs._wfac_imp);
#elif __cplusplus < 201403L
                    _wfac_imp = unique_ptr<WriteFileActiveConfirm>(new WriteFileActiveConfirm(*rhs._wfac_imp));
#endif

                }
                else
                    std::cerr << "FileService(const FileService&): _wfac_imp in nullptr." << std::endl;
                break;
            }
            case 9: {
                if (rhs._wfd_imp) {
#if __cplusplus >= 201403L
                    _wfd_imp = make_unique<WriteFileData>(*rhs._wfd_imp);
#elif __cplusplus < 201403L
                    _wfd_imp = unique_ptr<WriteFileData>(new WriteFileData(*rhs._wfd_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _wfd_imp in nullptr." << std::endl;
                break;
            }
            case 10: {
                if (rhs._wfdc_imp) {
#if __cplusplus >= 201403L
                    _wfdc_imp = make_unique<WriteFileDataConfirm>(*rhs._wfdc_imp);
#elif __cplusplus < 201403L
                    _wfdc_imp = unique_ptr<WriteFileDataConfirm>(new WriteFileDataConfirm(*rhs._wfdc_imp));
#endif
                }
                else
                    std::cerr << "FileService(const FileService&): _wfdc_imp in nullptr." << std::endl;
                break;
            }
            default:
                break;
        }
        return *this;
    }
} // asdu
