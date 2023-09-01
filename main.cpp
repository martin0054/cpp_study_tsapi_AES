#include "header.h"

template <typename T> 
std::shared_ptr<T> Singleton<T>::instance_;

 int main(int argc, char* argv[]) {


	std::cout << "Monitor a device start" << std::endl;

	auto configParser = ConfigParser::GetInstance();
	 
   	try
	{
		std::string fileName = "monitorDN.INI";
		if(configParser->readMonitoringDn(fileName)){
			std::cout << "[ERROR] Read Monitoring ini file error" << std::endl; 
		}

		
	}
	catch (std::exception& e)
	{
		 std::cout << "[ERROR] monitorDN.INI file read error : " << e.what() << std::endl;
		 exit(0);
	}

    std::string configDn =configParser->getMonitoringDN();
		std::cout << "Monitor device list : " << configDn << std::endl;

	const char* pRegistDn = NULL;
	const char* pSplitDelimiters = "|";
	pRegistDn = strtok((char*)configDn.c_str(), pSplitDelimiters );

	if(!pRegistDn || pRegistDn == NULL)
		{
			 std::cout << "[ERROR] Try again ini file Input Device Monitoring List" << std::endl;

			return 0;
		}

	 std::cout << "Start Monitor a device request " << std::endl;

   

      return 0;
  }