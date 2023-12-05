#ifndef TROOMSVR_LOG_MESSAGE_PROCESS_H
#define TROOMSVR_LOG_MESSAGE_PROCESS_H

namespace roomsvr {

    class CLogSubMCDProc;

    class CLogMessageProc {
    public:
        CLogMessageProc(CLogSubMCDProc *proc);

        ~CLogMessageProc();

        int ProcMsg(char *pkg, unsigned int pkg_len);

    private:
        CLogSubMCDProc *_proc;
    };

}

#endif //TROOMSVR_LOG_MESSAGE_PROCESS_H
