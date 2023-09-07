#include "header.h"

ConfigParser::ConfigParser()
{
   std::cout << "ConfigParser constructed" << std::endl; 
}

ConfigParser::~ConfigParser()
{
    std::cout << "ConfigParser destructed" << std::endl;
}




bool ConfigParser::readMonitoringDn(std::string fileName)
{
    boost::property_tree::ptree pt;

    try{
         if (fileName.empty() || fileName.find(".") == std::string::npos)
        {
            std::cout << "[WARN] Check Config File Name" << std::endl; 
            return false;
        }

        boost::property_tree::ini_parser::read_ini(fileName, pt);
        std::string monitorDnFilkey = "Monitor.dn_list";
        
        monitoring_Dn_ = pt.get<std::string>(monitorDnFilkey);

        return true;
    }

     catch(std::exception& e)
    {
         std::cout << "[ERROR] exception Read Monitoring ini file error " ;
         std::cout << e.what();      
     
        return false;
    }
}

std::string ConfigParser::getMonitoringDN()
{
    return monitoring_Dn_;
}


bool ConfigParser::readConfig(std::string fileName)
{
   boost::property_tree::ptree pt;
   
    try{
         if (fileName.empty() || fileName.find(".") == std::string::npos)
        {
            std::cout << "[WARN] Check Config File Name" << std::endl; 
            return false;
        }

        boost::property_tree::ini_parser::read_ini(fileName, pt);
        listen_port_ = pt.get<std::string>("config.aribo_listen_port");
        server_name_ = pt.get<std::string>("config.server_name");
        server_name_sec = pt.get<std::string>("config.server_name_sec");
        login_id_ = pt.get<std::string>("config.login_id");
        login_pwd_ = pt.get<std::string>("config.login_passwd");
        private_data_ = pt.get<std::string>("config.private_data");
        hb_period_ = pt.get<float>("config.hb_period");
        monitor_sleep_ = pt.get<float>("config.monitor_sleep");
        blind_transfer = pt.get<bool>("config.blind_transfer_on");
        tsapi_license_ = pt.get<int>("config.tsapi_license");
    
         return true;
    }
   
    
     catch(std::exception& e)
    {
         std::cout << "[ERROR] Read Config ini file error" << std::endl; 
     
        return false;
    }
}


std::string ConfigParser::getListenPort()
{
    return listen_port_;
}

std::string ConfigParser::getServerName()
{
    return server_name_;
}

std::string ConfigParser::getServerNameSec()
{
    return listen_port_;
}

std::string ConfigParser::getLoginId()
{
    return login_id_;
}

std::string ConfigParser::getLoginPwd()
{
    return login_pwd_;
}

std::string ConfigParser::getPrivateData()
{
    return private_data_;
}

float ConfigParser::getHbPeriod()
{
    return hb_period_;
}

float ConfigParser::getMoniorSleep()
{
    return monitor_sleep_;
}

bool ConfigParser::getBlindTransfer()
{
    return blind_transfer;
}

int ConfigParser::getTsapiLicense()
{
    return tsapi_license_;
}
