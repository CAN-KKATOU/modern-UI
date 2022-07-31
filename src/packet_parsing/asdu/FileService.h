//
// Created by Astria on 2022/4/25.
//

#ifndef FILE_SERVICE_H
#define FILE_SERVICE_H

#include "BaseASDU.h"

#include "file_service/DirCall.h"
#include "file_service/DirCallConfirm.h"
#include "file_service/ReadFileActive.h"
#include "file_service/ReadFileActiveConfirm.h"
#include "file_service/ReadFileData.h"
#include "file_service/ReadFileDataConfirm.h"
#include "file_service/WriteFileActive.h"
#include "file_service/WriteFileActiveConfirm.h"
#include "file_service/WriteFileData.h"
#include "file_service/WriteFileDataConfirm.h"

namespace asdu {
    class FileService : public BaseASDU {
    private:
        int _aType{0};   // 附加数据包类型
        int _oType{0};   // 操作标识符

        unique_ptr<DirCall> _dc_imp;
        unique_ptr<DirCallConfirm> _dcc_imp;
        unique_ptr<ReadFileActive> _rfa_imp;
        unique_ptr<ReadFileActiveConfirm> _rfac_imp;
        unique_ptr<ReadFileData> _rfd_imp;
        unique_ptr<ReadFileDataConfirm> _rfdc_imp;
        unique_ptr<WriteFileActive> _wfa_imp;
        unique_ptr<WriteFileActiveConfirm> _wfac_imp;
        unique_ptr<WriteFileData> _wfd_imp;
        unique_ptr<WriteFileDataConfirm> _wfdc_imp;

    public:
        explicit FileService(const ASDULengthSet& asdu_length_set);

        FileService(const ASDULengthSet &asdu_length_set,
                    long long tie,
                    long long vsq,
                    long long cot,
                    size_t command_address,
                    size_t object_address,
                    int addon,
                    int operate
                    );

        FileService(unique_ptr<vector<int>> data, const ASDULengthSet &als);

        FileService(const FileService& rhs);

        ~FileService() override = default;

        shared_ptr<json> toJson() override;

        shared_ptr<json> getPosition() override;

        FileService& operator=(const FileService& rhs);

        bool set_dir_call(const DirCall& dcImp);
        bool set_dir_call(unique_ptr<DirCall> dcImp);

        bool set_dir_call_confirm(const DirCallConfirm& dccImp);
        bool set_dir_call_confirm(unique_ptr<DirCallConfirm> dccImp);

        bool set_read_active(const ReadFileActive& rfaImp);
        bool set_read_active(unique_ptr<ReadFileActive> rfaImp);

        bool set_read_active_confirm(const ReadFileActiveConfirm& rfacImp);
        bool set_read_active_confirm(unique_ptr<ReadFileActiveConfirm> rfacImp);

        bool set_read_data(const ReadFileData& rfdImp);
        bool set_read_data(unique_ptr<ReadFileData> rfdImp);

        bool set_read_data_confirm(const ReadFileDataConfirm& rfdcImp);
        bool set_read_data_confirm(unique_ptr<ReadFileDataConfirm> rfdcImp);

        bool set_write_active(const WriteFileActive& wfaImp);
        bool set_write_active(unique_ptr<WriteFileActive> wfaImp);

        bool set_write_active_confirm(const WriteFileActiveConfirm& wfacImp);
        bool set_write_active_confirm(unique_ptr<WriteFileActiveConfirm> wfacImp);

        bool set_write_data(const WriteFileData& wfdImp);
        bool set_write_data(unique_ptr<WriteFileData> wfdImp);

        bool set_write_data_confirm(const WriteFileDataConfirm& wfdcImp);
        bool set_write_data_confirm(unique_ptr<WriteFileDataConfirm> wfdcImp);
    };

} // asdu

#endif //FILE_SERVICE_H
