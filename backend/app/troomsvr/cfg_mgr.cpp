#include <net/if.h>
#include <sys/ioctl.h>

#include "cfg_mgr.h"
#include "tfc_base_str.h"

CCfgMgr::CCfgMgr() = default;

CCfgMgr::~CCfgMgr() = default;

int CCfgMgr::LoadCfg(const std::string &conf_file) {
    _path = conf_file;

    tfc::base::CFileConfig page;
    page.Init(_path);

    _log_para.log_level_ = tfc::base::from_str<int>(page["root\\log\\log_level"]);
    _log_para.log_type_ = tfc::base::from_str<int>(page["root\\log\\log_type"]);
    _log_para.path_ = page["root\\log\\path"];
    _log_para.name_prefix_ = page["root\\log\\name_prefix"];
    _log_para.max_file_size_ = tfc::base::from_str<int>(page["root\\log\\max_file_size"]);
    _log_para.max_file_no_ = tfc::base::from_str<int>(page["root\\log\\max_file_no"]);

    _self_external_ip = get_ip_by_if("eth0");
    _self_internal_ip = get_ip_by_if("eth1");

    struct in_addr addr_external{};
    addr_external.s_addr = _self_external_ip;
    const char *external_ip_str = inet_ntoa(addr_external);

    _external_ip = external_ip_str;

    struct in_addr addr_internal{};
    addr_internal.s_addr = _self_internal_ip;
    const char *internal_ip_str = inet_ntoa(addr_internal);

    _internal_ip = internal_ip_str;

    printf("Server External ip: %s, Internal ip: %s\n", _external_ip.c_str(), _internal_ip.c_str());

    return 0;
}

unsigned int CCfgMgr::get_ip_by_if(const char *name) {
    int fd;
    unsigned int interface;
    struct ifreq buf[10];
    struct ifconf ifc{};
    unsigned ip = 0;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0) {
        ifc.ifc_len = sizeof(buf);
        ifc.ifc_buf = (caddr_t) buf;
        if (!ioctl(fd, SIOCGIFCONF, (char *) &ifc)) {
            interface = ifc.ifc_len / sizeof(struct ifreq);
            while (interface-- > 0) {
                if (strcmp(buf[interface].ifr_name, name) == 0) {
                    if (!(ioctl(fd, SIOCGIFADDR, (char *) &buf[interface])))
                        ip = (unsigned int) ((struct sockaddr_in *) (&buf[interface].ifr_addr))->sin_addr.s_addr;
                    break;
                }
            }
        }
        close(fd);
    }
    return ip;
}