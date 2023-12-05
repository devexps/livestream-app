#include "log_message_process.h"
#include "troomsvr_submcd_log_proc.h"

using namespace roomsvr;

CLogMessageProc::CLogMessageProc(CLogSubMCDProc *proc) : _proc(proc) {
}

CLogMessageProc::~CLogMessageProc() {
}

int CLogMessageProc::ProcMsg(char *pkg, unsigned int pkg_len) {
    if (pkg_len >= 1024) {
        return -1;
    }
    char logStr[1024] = {0};
    memset(logStr, 0, 1024);
    memcpy(logStr, pkg + 3, pkg_len - 4);
    DEBUG_SUBMCD_NO_TIME(_proc->_cur_time, logStr);
    return 0;
}