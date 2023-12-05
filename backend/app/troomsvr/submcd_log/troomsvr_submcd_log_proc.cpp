#include "tfc_svr_utils.h"
#include "troomsvr_submcd_log_proc.h"

using namespace roomsvr;

void disp_mcd(void *priv) {
    auto *app = (CLogSubMCDProc *) priv;
    app->dispatch_mcd();
}

void disp_submcd(void *priv) {
    auto *app = (CLogSubMCDProc *) priv;
    app->dispatch_submcd();
}

extern "C"
{
tfc::cache::CacheProc *create_app() {
    return new CLogSubMCDProc();
}
}

void CLogSubMCDProc::run(const std::string &conf_file) {
    init(conf_file);

    add_mq_2_epoll(_mq_mcd_2_log, disp_mcd, this);
    add_mq_2_epoll(_mq_submcd_2_log, disp_submcd, this);

    DEBUG_SUBMCD_P(LOG_DEBUG, "CLogSubMCDProc::run, troomsvr_submcd_log started!\n");

    _ll_now_Tick = get_tick_count(_cur_tv_time);
    _cur_time = _cur_tv_time.tv_sec;

    unsigned int loop_time_count = 0;
    unsigned int check_flags_count = 0;
    for (;;) {
        check_flags_count++;

        if (check_flags_count >= 100) {
            check_flags_count = 0;
            if (check_flags() > 0) {
                DEBUG_SUBMCD_P(LOG_DEBUG, "CLogSubMCDProc::run, recv signal, quit!\n");
                break;
            }
        }
        loop_time_count++;

        if (loop_time_count >= 500) {
            loop_time_count = 0;
            dispatch_timeout();
        }
        run_epoll_4_mq();
    }
}

void CLogSubMCDProc::init(const string &conf_file) {
    int ret;
    ret = _tfc_obj.init();
    if (ret < 0) {
        DEBUG_SUBMCD_P(LOG_FATAL, "CLogSubMCDProc::init, tfc object init fail\n");
        DEBUG_SUBMCD_P(LOG_FATAL, "CLogSubMCDProc::init, svr shutdown!.\n");
        assert(false);
    }
    _cfg.LoadCfg("../etc/troomsvr_mcd.conf");

    init_log();
    init_ipc();

    signal(SIGUSR1, sigusr1_handle);
    signal(SIGUSR2, sigusr2_handle);
}

void CLogSubMCDProc::init_log() {
    TLogPara *log_para = &(_cfg._log_para);
    int ret = TFC_DEBUG_OPEN(log_para->log_level_, log_para->log_type_,
                             log_para->path_, log_para->name_prefix_,
                             log_para->max_file_size_, log_para->max_file_no_);
    assert(ret >= 0);
}

void CLogSubMCDProc::init_ipc() {
    _mq_mcd_2_log = _mqs["mq_mcd_2_log"];
    _mq_submcd_2_log = _mqs["mq_submcd_2_log"];

    assert(_mq_mcd_2_log != nullptr);
    assert(_mq_submcd_2_log != nullptr);
}

int CLogSubMCDProc::check_flags() {
    if (check_flag.is_flag_set(FLG_CTRL_QUIT)) {
        check_flag.clear_flag();
        return 1;
    }
    return 0;
}

void CLogSubMCDProc::dispatch_timeout() {
    _ll_now_Tick = get_tick_count(_cur_tv_time);
    _cur_time = _cur_tv_time.tv_sec;
}

void CLogSubMCDProc::dispatch_mcd() {
    int ret;
    unsigned int data_len, flow;
    unsigned int max_count = 0;
    do {
        max_count++;
        if (max_count >= 1000) {
            break;;
        }
        ret = _mq_mcd_2_log->try_dequeue(_m_buf, BUF_SIZE, data_len, flow);
        if (ret < 0) {
            DEBUG_SUBMCD_P(LOG_ERROR, "CLogSubMCDProc::dispatch_mcd, dequeue msg failed!\n");
            break;;
        }
        if (data_len <= 0) {
            break;
        }
        int pkg_len = 0;
        for (unsigned int len = 0; len < data_len; len += pkg_len) {
            //pkg_len = usLen + 0x0b + sBody + 0x03;
            pkg_len = req_complete_func(_m_buf + len, data_len - len);
            if (pkg_len > 0) {
                process_msg(_m_buf + len, pkg_len);
            } else {
                DEBUG_SUBMCD_P(LOG_ERROR, "CLogSubMCDProc::dispatch_mcd, pkg_len format wrong\n");
                break;
            }
        }
    } while (ret == 0 && data_len > 0);

    DEBUG_SUBMCD_END();
}

void CLogSubMCDProc::dispatch_submcd() {
    int ret;
    unsigned int data_len, flow;
    unsigned int max_count = 0;
    do {
        max_count++;
        if (max_count >= 100) {
            break;;
        }
        ret = _mq_submcd_2_log->try_dequeue(_m_buf, BUF_SIZE, data_len, flow);
        if (ret < 0) {
            DEBUG_SUBMCD_P(LOG_ERROR, "CLogSubMCDProc::dispatch_submcd, dequeue msg failded!\n");
            break;
        }
        if (data_len <= 0) {
            break;
        }
        int pkg_len = 0;
        for (unsigned int iLen = 0; iLen < data_len; iLen += pkg_len) {
            //pkg_len = usLen + 0x0b + sBody + 0x03;
            pkg_len = req_complete_func(_m_buf + iLen, data_len - iLen);
            if (pkg_len > 0) {
                process_msg(_m_buf + iLen, pkg_len);
            } else {
                DEBUG_SUBMCD_P(LOG_ERROR, "CLogSubMCDProc::dispatch_submcd, pkg_len format wrong\n");
                break;
            }
        }
    } while (ret == 0 && data_len > 0);

    DEBUG_SUBMCD_END();
}

int CLogSubMCDProc::req_complete_func(const void *data, unsigned data_len) {
    if (data_len <= 0) {
        return 0;
    }
    const char *p = (char *) data;

    unsigned short pkg_len = ntohs(*(unsigned short *) (p));

    if ((p[2] != 0x0b)) {
        return -1;
    }
    if (data_len >= pkg_len) {
        if (p[pkg_len - 1] != 0x03) {
            return -1;
        } else {
            return pkg_len;
        }
    } else {
        return 0;
    }
}

void CLogSubMCDProc::process_msg(char *pkg, unsigned int pkg_len) {
    _log_message_proc.ProcMsg(pkg, pkg_len);
}