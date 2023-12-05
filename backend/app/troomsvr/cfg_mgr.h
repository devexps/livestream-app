#ifndef TROOMSVR_CFG_MGR_H
#define TROOMSVR_CFG_MGR_H

#include <vector>
#include <string>

#include "tfc_base_config_file.h"
#include "tfc_debug_log.h"

class CCfgMgr {
public:
    CCfgMgr();

    ~CCfgMgr();

    int LoadCfg(const std::string &conf_file);

public:
    TLogPara _log_para;
    int _log_time_format;

    unsigned int _self_external_ip;
    unsigned int _self_internal_ip;

    string _external_ip;
    string _internal_ip;

private:
    unsigned int get_ip_by_if(const char *name);

private:
    std::string _path;
};

#endif //TROOMSVR_CFG_MGR_H
