#pragma once

//#include "type.h"
#include <sys/types.h>
#include <stdint.h>

#include <string>

#include <boost/version.hpp>

#if BOOST_VERSION >= 105000
#define TIME_UTC TIME_UTC_
#endif

class config_file_property
{
public:
    config_file_property(){}

    uint16_t get_http_port() const
    {
        return m_http_port;
    }
    void set_http_port(uint16_t http_port)
    {
        m_http_port = http_port;
    }

    std::string& get_http_host()
    {
        return m_http_host;
    }
    void set_http_host(std::string http_host)
    {
        m_http_host = http_host;
    }

private:
    std::string m_http_host;
    uint16_t    m_http_port;
};


