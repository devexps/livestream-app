#include "tfc_svr_log_mgr.h"
#include "troomsvr_mcd_proc.h"

using namespace roomsvr;

extern "C"
{
tfc::cache::CacheProc *create_app() {
    return new CMCDProc();
}
}

void CMCDProc::run(const std::string &conf_file) {
    init(conf_file);
}

void CMCDProc::init(const std::string &conf_file) {
    int ret = _tfc_obj.init();
    if (ret < 0) {
        DEBUG_P(LOG_FATAL, "CMCDProc::init, tfc object init fail\n");
        DEBUG_P(LOG_FATAL, "CMCDProc::init, svr shutdown!\n");
        assert(false);
    }
    _cfg.LoadCfg(conf_file);

    init_ipc();
    init_log();

    DEBUG_P(LOG_DEBUG, "CMCDProc::init, OK.....\n")
}

void CMCDProc::init_ipc() {
    _mq_ccd_2_mcd = _mqs["mq_ccd_2_mcd"];
    _mq_mcd_2_ccd = _mqs["mq_mcd_2_ccd"];

    assert(_mq_ccd_2_mcd != nullptr);
    assert(_mq_mcd_2_ccd != nullptr);

    _mq_mcd_2_submcd = _mqs["mq_mcd_2_submcd"];

    assert(_mq_mcd_2_submcd != nullptr);

    _mq_mcd_2_log = _mqs["mq_mcd_2_log"];

    assert(_mq_mcd_2_log != nullptr);
}

void CMCDProc::init_log() {
    TLogPara *log_para = &(_cfg._log_para);
    int ret = DEBUG_OPEN(log_para->log_level_,
                         (int *) &_cur_time,
                         log_para->log_type_,
                         _mq_mcd_2_log,
                         _cfg._log_time_format);
    assert(ret >= 0);
}
