#ifndef TROOMSVR_MCD_PROC_H
#define TROOMSVR_MCD_PROC_H

#include "tfc_cache_proc.h"
#include "tfc_object.h"
#include "cfg_mgr.h"

namespace roomsvr {

    typedef struct {
        unsigned int _magic_num;
        unsigned int _reqrsp_type;
        unsigned int _flow;
        unsigned int _ip;
        unsigned int _port;
        unsigned int _uin;
        unsigned char _room_type;
        unsigned int _room_id;
        unsigned int _seq;
        unsigned int _broadcast_count;
        unsigned int _other;
    } TUCCDHeader;

    const unsigned BUF_SIZE = 4 * 1024 * 1024 + sizeof(TUCCDHeader);

    class CMCDProc : public tfc::cache::CacheProc {
    public:
        CMCDProc() : _m_buf(nullptr) {
            _m_buf = new char[BUF_SIZE];
            _uccd_header = (TUCCDHeader *) _m_buf;
            _buf = _m_buf + sizeof(TUCCDHeader);

            _last_stat_time = time(nullptr);
            _cur_time = _last_stat_time;

            _last_tick_do_clear_room_num = 0;
        }

        virtual ~CMCDProc() {
            if (_m_buf != nullptr) delete[] _m_buf;
        }

        virtual void run(const std::string &conf_file);

    public:
        CCfgMgr _cfg;

        time_t _cur_time;
        time_t _last_tick_do_clear_room_num;

    protected:
        void init(const std::string &conf_file);

        void init_ipc();

        void init_log();

    protected:
        char *_buf;
        char *_m_buf;
        TUCCDHeader *_uccd_header;

        time_t _last_stat_time;

        TFCObject _tfc_obj;

        tfc::net::CFifoSyncMQ *_mq_ccd_2_mcd, *_mq_mcd_2_ccd;
        tfc::net::CFifoSyncMQ *_mq_mcd_2_submcd;
        tfc::net::CFifoSyncMQ *_mq_mcd_2_log;
    };
}

#endif //TROOMSVR_MCD_PROC_H