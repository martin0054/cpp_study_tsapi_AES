#include "header.h"

Aes_manager::Aes_manager()
{
    acs_handle_ = std::make_unique<ACSHandle_t>();
}

Aes_manager::~Aes_manager()
{
}

bool Aes_manager::OpenACSStream()
{
   ReadServerConfig();

    return true;
}

bool Aes_manager::ReadServerConfig(){
    
    std::cout << "Read Server config start" << std::endl;
      auto configParser = ConfigParser::GetInstance();
    
    try
	{
		std::string fileName = "config.INI";
		if(configParser->readMonitoringDn(fileName)){
			std::cout << "[ERROR] Read config ini file error" << std::endl; 
            return false;
		}

    return true;

	}
	catch (std::exception& e)
	{
		 std::cout << "[ERROR] config.INI file read error : " << e.what() << std::endl;
		 return false;
	}
}