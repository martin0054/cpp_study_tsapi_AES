#include "header.h"

template <typename T>
std::shared_ptr<T> Singleton<T>::instance_;

int ServiceOpen()
{
	std::cout << "***********************************************************" << std::endl;
	std::cout << "             AES Server Connect & ARIBO-CTI START           " << std::endl;
	std::cout << "************************************************************" << std::endl;

	auto aesManager = AesManager::GetInstance();

	float fSleep = 0;
	aesManager->StartEvtThread();

	if (aesManager->OpenACSStream())
	{

		//	MonitoringDevice();
	}

	// aesManager->CloseACSStream();

	return 0;
}

std::vector<std::string> splitCommand(std::string command)
{
	std::string strCommmand = command;
	std::vector<std::string> v;
	std::stringstream ss(strCommmand);
	std::string s;

	while (std::getline(ss, s, ' '))
	{
		if (!s.empty())
			v.push_back(s);
	}
	return v;
}
int main(int argc, char *argv[])
{
	try
	{
		std::mutex m2;
		std::lock_guard<std::mutex> gard(m2);
		ServiceOpen();
		std::cout << " service Open" << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "[ERROR] main error : " << e.what() << std::endl;
	}

	std::cout << " while before" << std::endl;

	try
	{
		auto aesManager = AesManager::GetInstance();
		while (true)
		{

			std::string command = " ";
			std::cout << " 명령어를 입력하세요 : ex) Event CallingDn CalledDn  ";
			std::getline(std::cin, command);
			std::vector<std::string> v = splitCommand(command);

			// std::string event = "";
			// std::string callingDn = " ";
			// std::string calledDn = " ";

			
			if (v.size() == 3 && v[0] == "makecall")
			{
				// makecall 2612 901049093096
				std::string event = v[0];
				std::string callingDn = v[1];
				std::string calledDn = v[2];

				std::cout << " Event : " << event << " callingDn : " << callingDn << " , calledDn : " << calledDn << std::endl;
				int rst = aesManager->MakeCall(callingDn, calledDn, " ");

				std::cout << event << " result : " << rst << std::endl;
			}
			else if (v.size() == 2 && v[0] == "drop")
			{
				std::string event = v[0];
				std::string callingDn = v[1];

				auto infoManger = InfoManager::GetInstance();
				int connId = infoManger->getConnIdByCalledDN(callingDn);
				std::cout << event << " : " << callingDn << " | connId  " << connId << std::endl;
				int rst = aesManager->ClearCall(callingDn, connId);

				std::cout << event << " result : " << rst << std::endl;
			}
			// int TransferCall(std::strig callingDn, int oldCallID, int newCallId)

		
			else if  (v.size() == 3 && v[0] == "transe")
			{	
				std::string event = v[0];
				std::string callingDn = v[1];
				std::string calledDn = v[2];

				
				std::cout << " ex)Event CallingDn CalledDn : " << command << std::endl;
			}
		}
	}
	catch (std::exception &e)
	{
		std::cout << "[ERROR] main whiled error : " << e.what() << std::endl;
	}

	return 0;
}
