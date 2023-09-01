#include "header.h"

Aes_manager::Aes_manager()
{
    acs_handle_ = std::make_unique<ACSHandle_t>();
}

Aes_manager::~Aes_manager()
{
}

void Aes_manager::EnumerateServiceNames()
{
	RetCode_t nRetCode = acsEnumServerNames(ST_CSTA, DisplayServerNames, 0);

	if ( nRetCode != ACSPOSITIVE_ACK )
	{
		std::cout << "[ERROR] Error Code: "<< nRetCode;
	}
}

bool Aes_manager::OpenACSStream()
{
	ReadServerConfig();

	EnumerateServiceNames();
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