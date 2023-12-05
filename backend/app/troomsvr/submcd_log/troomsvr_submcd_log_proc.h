#ifndef TROOMSVR_TROOMSVR_SUBMCD_LOG_PROC_H
#define TROOMSVR_TROOMSVR_SUBMCD_LOG_PROC_H

#include "tfc_svr_proc.h"
#include "tfc_cache_proc.h"
#include "tfc_object.h"
#include "../cfg_mgr.h"
#include "log_message_process.h"

namespace roomsvr {

    const unsigned BUF_SIZE = 4 * 1024 * 1024 + sizeof(TUCCDHeader);

    class CLogSubMCDProc : public tfc::cache::CacheProc {
    public:
        CLogSubMCDProc()
                : _log_message_proc(this), _m_buf(nullptr) {
            _m_buf = new char[BUF_SIZE];
            _uccd_header = (TUCCDHeader *) _m_buf;
            _buf = _m_buf + sizeof(TUCCDHeader);

            _cur_time = time(nullptr);
        }

        virtual ~CLogSubMCDProc() {
            if (_m_buf != nullptr) delete[] _m_buf;
        }

        virtual void run(const std::string &conf_file);

        void dispatch_mcd();

        void dispatch_submcd();

    public:
        CLogMessageProc _log_message_proc;
        CCfgMgr _cfg;

        time_t _cur_time;
        timeval _cur_tv_time;
        unsigned long long _ll_now_Tick;

    protected:
        void init(const std::string &conf_file);

        void init_log();

        void init_ipc();

        int check_flags();

        void dispatch_timeout();

        int req_complete_func(const void *data, unsigned data_len);

        void process_msg(char *pkg, unsigned int pkg_len);

    protected:
        char *_buf;
        char *_m_buf;
        TUCCDHeader *_uccd_header;

        TFCObject _tfc_obj;

        tfc::net::CFifoSyncMQ *_mq_mcd_2_log;
        tfc::net::CFifoSyncMQ *_mq_submcd_2_log;


    };

    static CCheckFlag check_flag;

    static void sigusr1_handle(int sig_val) {
        check_flag.set_flag(FLG_CTRL_RELOAD);
        signal(SIGUSR1, sigusr1_handle);
    }

    static void sigusr2_handle(int sig_val) {
        check_flag.set_flag(FLG_CTRL_QUIT);
        signal(SIGUSR2, sigusr2_handle);
    }
}

#endif //TROOMSVR_TROOMSVR_SUBMCD_LOG_PROC_H
