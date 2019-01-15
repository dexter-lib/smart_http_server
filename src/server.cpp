//============================================================================
// Name        : common_http_server.cpp
// Author      : zhangpeng <polygontech.cn@gmail.com>
// Version     :
// Copyright   : Copyright © 2019 <polygontech.com.cn> All rights reserved.
// Description : A common http server for c plus plus
//============================================================================
#include <stdint.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <string>

#include <boost/enable_shared_from_this.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>

#include "log.h"
#include "config_file_property.h"
#include "http_server.h"
#include "logic_handle.h"


//#include "watch.h"


using namespace std;

typedef std::vector<std::string> strings_t;

inline void separate_tokens(std::string const& wholestr, strings_t& tokens, std::string const& sepchars = std::string(" \t"))
{
    boost::char_separator<char> sep(sepchars.c_str());
    boost::tokenizer<boost::char_separator<char> > tok(wholestr, sep);
    tokens.clear();
    std::copy(tok.begin(), tok.end(), std::back_inserter(tokens));
}

pair<string, string> at_option_parser(string const& s)
{
    if ('@' == s[0])
    {
        return std::make_pair(string("config"), s.substr(1));
    }
    else
    {
        return pair<string, string>();
    }
}

int main(int argc, char *argv[])
{
    std::string conf_path;
    std::string conf_file;

    std::string http_host = "0.0.0.0";
    uint16_t    http_port = 8088;

    int32_t     log_level;
    std::string log_file_name;

    namespace po = boost::program_options;

    namespace po = boost::program_options;

    po::options_description http("http options");
    http.add_options()
        ("http_host,hh", po::value<string>(&http_host), "set http_host")
        ("http_port,p", po::value<uint16_t>(&http_port), "set http_port");
    po::options_description general("general options");
    general.add_options()
        ("help,H", "produce help message")
        ("log_file_name,l", po::value<string>(&log_file_name)->default_value("common_http_server.log"), "set log filename")
        ("log_level,e", po::value<int32_t>(&log_level)->default_value(LOG_LEVEL_TRACE), "set max log level (7 - 0)");

    po::options_description all("All options");


    if (argc >= 2)
    {
        conf_path = argv[1];
    }
    else
    {
        conf_path = "./common_http_server.conf";
    }

    po::options_description config("config options");
    config.add_options()
        ("config", po::value<string>(&conf_file)->default_value(conf_path),
        "set config file, specified with '@name' too");

    all.add(general).add(config).add(http);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(all).extra_parser(at_option_parser).run(), vm);

    if (vm.count("config"))
    {
        std::string conf_name = vm["config"].as<string>();
        ifstream ifs_config(conf_name.c_str());

        if (! ifs_config)
        {
            cerr << "could not open the configure file" << endl;
            return 1;
        }

        stringstream ss_config;
        ss_config << ifs_config.rdbuf();

        strings_t args;
        separate_tokens(ss_config.str(), args, " \r\n");
        po::store(po::command_line_parser(args).options(all).run(), vm);
    }
    po::notify(vm);

    //global::watch::reset(log_file_name, log_level);

	SET_LOG_NAME(log_file_name.c_str());
	SET_LOG_LEVEL((log_level_t)log_level);
	SET_LOG_SIZE(100);

    boost::shared_ptr<config_file_property> config_property(new config_file_property);

    config_property->set_http_host(http_host);
    config_property->set_http_port(http_port);


    boost::shared_ptr<http::server::httpserver> http_server(new http::server::httpserver(config_property->get_http_host(),
            boost::lexical_cast<std::string>(config_property->get_http_port())));

    /**
     * Modified your implement here
     */
    http::server::common_logic_handle *handle = new logic_handle();
    http_server->set_logic_handle(handle);
    /**
     * Finish
     */

    http_server->start();

    boost::thread http_thread = boost::thread(boost::bind(&boost::asio::io_service::run, http_server->get_io_service()));

    LOG_INFO("Start http server , bind: %s:%u", http_host.c_str(), http_port);

    http_thread.join();

    if(handle)
    {
    	delete handle;
    	handle = NULL;
    }
	return 0;
}
