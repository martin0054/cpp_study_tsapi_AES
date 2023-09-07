#include "header.h"

template <typename T>
std::shared_ptr<T> Singleton<T>::instance_;

pthread_t p_thread;

std::map<int, char *> serviceNameMap;
AesManager::AesManager()
{
	std::mutex monitoringDevice_mutex;
	std::cout << "AesManager constructed" << std::endl;
	nOpenStreamInvokeID_ = 0;
	nCloseStreamInvokeID_ = 0;
	acs_handle_ = 0;
	// nStartMonitorInvokeID_ =0;
}

AesManager::~AesManager()
{
	std::cout << "AesManager destructed" << std::endl;
}

// THREAD
void *AesManager::RecvEventThread(void *arg)
{
	std::cout << "RecvEventThread()" << std::endl;
	AesManager *aes = reinterpret_cast<AesManager *>(arg);
	aes->RecvThread();
	return 0;
}
void AesManager::RecvThread()
{
	std::cout << "RecvThread()" << std::endl;
	const int APP_DEF_DEFAULT_BUFFER_SIZE = 2048;
	unsigned short usEventBufSize = APP_DEF_DEFAULT_BUFFER_SIZE;
	CSTAEvent_t *cstaEvent = NULL;
	unsigned short usNumEvents = 1;

	while (true)
	{
		int error;
		ATTPrivateData_t privateData;
		privateData.length = sizeof(privateData.data);

		if (cstaEvent != NULL)
		{
			free(cstaEvent);
		}
		cstaEvent = (CSTAEvent_t *)malloc((size_t)usEventBufSize);

		RetCode_t nRetCode;

		std::mutex acsGetEventBlock_mutex;
		std::lock_guard<std::mutex> acsGetEventBlockGard(acsGetEventBlock_mutex);
		// pthread_mutex_lock(&notiMutex);
		nRetCode =
			acsGetEventBlock(
				acs_handle_,
				(void *)cstaEvent,
				&usEventBufSize,
				(PrivateData_t *)&privateData,
				&usNumEvents);
		// pthread_mutex_unlock(&notiMutex);

		if (nRetCode != ACSPOSITIVE_ACK)
		{
			if (nRetCode == ACSERR_BADHDL)
			{
				std::cout << "DEBUG  : nRetCode = ACSERR_BADHDL " << std::endl;
				sleep(1);
			}
			else if (nRetCode == ACSERR_NOSERVER)
			{
				std::cout << "DEBUG  : nRetCode = ACSERR_NOSERVER " << std::endl;
				continue;
			}
			else if (nRetCode == ACSERR_STREAM_FAILED)
			{
				std::cout << "DEBUG  : nRetCode = ACSERR_STREAM_FAILED " << std::endl;
				continue;
			}
			else if (nRetCode == ACSERR_UBUFSMALL)
			{
				std::cout << "DEBUG  : nRetCode = ACSERR_UBUFSMALL " << std::endl;
				continue;
			}
			else if (nRetCode == ACSERR_NOMESSAGE)
			{
				std::cout << "DEBUG  : nRetCode = ACSERR_NOMESSAGE " << std::endl;
				usleep(1);
				continue;
			}
			else
			{
				std::cout << "DEBUG  : [Error] nRetCode : " << nRetCode << std::endl;
				continue;
			}
		}
		else
		{ // 정상
			switch (cstaEvent->eventHeader.eventClass)
			{
				case ACSCONFIRMATION:
				{
					switch (cstaEvent->eventHeader.eventType)
					{

					case ACS_OPEN_STREAM_CONF:
					{
						std::cout << " nOpenStreamInvokeID_ : " << nOpenStreamInvokeID_ << " invokeID : " << cstaEvent->event.acsConfirmation.invokeID << std::endl;

						std::cout << "  ACSPOSITIVE_ACK/ACSCONFIRMATION/ ACS_OPEN_STREAM_CONF : " << cstaEvent->eventHeader.eventType << std::endl;
						StartMoniringDevice();
						break;
					}
					case ACS_CLOSE_STREAM_CONF:
					{
						std::cout << "  ACSPOSITIVE_ACK/ACSCONFIRMATION/ACS_CLOSE_STREAM_CONF : " << cstaEvent->eventHeader.eventType << std::endl;
						break;
					}
					case ACS_UNIVERSAL_FAILURE_CONF:
					{
						std::cout << "  ACSPOSITIVE_ACK/ACSCONFIRMATION/ACS_UNIVERSAL_FAILURE_CONF : " << cstaEvent->eventHeader.eventType
								<< " invoke ID " << cstaEvent->event.acsConfirmation.invokeID << " error  " << cstaEvent->event.acsConfirmation.u.failureEvent.error << std::endl;
						break;
					}
					default:
					{
						std::cout << "DEBUG  : [Error]  Event Type1: " << cstaEvent->eventHeader.eventType << std::endl;
					}
					}
					break;
				}
				case CSTACONFIRMATION:
				{
					switch (cstaEvent->eventHeader.eventType)
					{	
						case CSTA_MONITOR_CONF:
						{
							int invokeID = cstaEvent->event.cstaConfirmation.invokeID;

							std::cout << "/ monitorCrossRefID : " << cstaEvent->event.cstaConfirmation.u.monitorStart.monitorCrossRefID
									<< "/ invokeID : " << invokeID << std::endl;

							auto infoManger = InfoManager::GetInstance();

							// infoManger->setResMonitoringInvokeId(invokeID);

							infoManger->getDeviceDnByInvokeId(invokeID);

							break;
						}
						case CSTA_UNIVERSAL_FAILURE_CONF:
						{
							std::cout << "  ACSPOSITIVE_ACK/CSTACONFIRMATION/CSTA_UNIVERSAL_FAILURE_CONF : " << cstaEvent->eventHeader.eventType << std::endl;
						}
						case CSTA_MONITOR_STOP_CONF:
						{
							std::cout << "  ACSPOSITIVE_ACK/CSTACONFIRMATION/CSTA_MONITOR_STOP_CONF : " << cstaEvent->eventHeader.eventType << std::endl;
						}
						case CSTA_ESCAPE_SVC_CONF:
						{
							std::cout << "  ACSPOSITIVE_ACK/CSTACONFIRMATION/CSTA_ESCAPE_SVC_CONF : " << cstaEvent->eventHeader.eventType << std::endl;
						}
						case CSTAUNSOLICITED:
						{
							switch (cstaEvent->eventHeader.eventType)
							{
							case CSTA_SERVICE_INITIATED:
								std::cout << "  CSTAUNSOLICITED/CSTA_SERVICE_INITIATED : " << cstaEvent->eventHeader.eventType << std::endl;
								PrintgLog(cstaEvent);
								break;
							case CSTA_ORIGINATED:
								std::cout << "  CSTAUNSOLICITED/CSTA_ORIGINATED : " << cstaEvent->eventHeader.eventType << std::endl;
								PrintgLog(cstaEvent);
								break;
							case CSTA_DELIVERED:
								std::cout << "  CSTAUNSOLICITED/CSTA_DELIVERED : " << cstaEvent->eventHeader.eventType << std::endl;
								PrintgLog(cstaEvent);
								break;
							case CSTA_ESTABLISHED:
								std::cout << "  CSTAUNSOLICITED/CSTA_ESTABLISHED : " << cstaEvent->eventHeader.eventType << std::endl;
								PrintgLog(cstaEvent);
								break;
							case CSTA_TRANSFERRED:
								std::cout << "  CSTAUNSOLICITED/CSTA_TRANSFERRED : " << cstaEvent->eventHeader.eventType << std::endl;
								PrintgLog(cstaEvent);
								break;
							case CSTA_CONFERENCED:
								std::cout << "  CSTAUNSOLICITED/CSTA_CONFERENCED : " << cstaEvent->eventHeader.eventType << std::endl;
								PrintgLog(cstaEvent);
								break;
							case CSTA_HELD:
								std::cout << "  CSTAUNSOLICITED/CSTA_HELD : " << cstaEvent->eventHeader.eventType << std::endl;
								PrintgLog(cstaEvent);
								break;
							case CSTA_RETRIEVED:
								std::cout << "  CSTAUNSOLICITED/CSTA_RETRIEVED : " << cstaEvent->eventHeader.eventType << std::endl;
								PrintgLog(cstaEvent);
								break;
							case CSTA_CALL_CLEARED:
								std::cout << "  CSTAUNSOLICITED/CSTA_CALL_CLEARED : " << cstaEvent->eventHeader.eventType << std::endl;
								PrintgLog(cstaEvent);
								break;
							case CSTA_CONNECTION_CLEARED:
								std::cout << "  CSTAUNSOLICITED/CSTA_CONNECTION_CLEARED : " << cstaEvent->eventHeader.eventType << std::endl;
								PrintgLog(cstaEvent);
								break;
							case CSTA_DIVERTED:
								std::cout << "  CSTAUNSOLICITED/CSTA_DIVERTED : " << cstaEvent->eventHeader.eventType << std::endl;
								PrintgLog(cstaEvent);
								break;
							case CSTA_FAILED:
								std::cout << "  CSTAUNSOLICITED/CSTA_FAILED : " << cstaEvent->eventHeader.eventType << std::endl;
								PrintgLog(cstaEvent);
								break;
							case CSTA_NETWORK_REACHED:
								std::cout << "  CSTAUNSOLICITED/CSTA_NETWORK_REACHED : " << cstaEvent->eventHeader.eventType << std::endl;
								PrintgLog(cstaEvent);
								break;
							}
						}
						case CSTA_MAKE_CALL_CONF : {
							std::cout << "  CSTACONFIRMATION/CSTA_MAKE_CALL_CONF : " <<
							" event_class : " <<    cstaEvent->eventHeader.eventClass << " | event_type : "<<  cstaEvent->eventHeader.eventType << std::endl;
								PrintgLog(cstaEvent);
								break;
						}
						default:
						{
							std::cout << "DEBUG  : [Error]  Event Type2: " << cstaEvent->eventHeader.eventType << std::endl;
						}
					}
					break;
				}
				break;
			}
		}
	}
}
void AesManager::PrintgLog(CSTAEvent_t *cstaEvent)
{
	auto &event = cstaEvent->event.cstaUnsolicited.u.serviceInitiated;
	auto call_id = event.initiatedConnection.callID;
	auto device_id = event.initiatedConnection.deviceID;
	ATTEvent_t att_event;
	char *ucid = nullptr;
	ucid = att_event.u.serviceInitiated.ucid;
	std::cout << " ucid : " << ucid << " | call_id : " << call_id << " | device_id : " << device_id << std::endl;
	
	auto infoManger = InfoManager::GetInstance();
	infoManger->setConnId(device_id, call_id );

	
}
int AesManager::StartEvtThread()
{
	std::cout << "StartEvtThread()" << std::endl;
	int nResult = -1;

	nResult = pthread_create(&p_thread, NULL, RecvEventThread, this);
	if (nResult != 0)
		return -1;

	return nResult;
}

Boolean DisplayServerNames(char *szServiceName, unsigned long lParam)
{
	if (strcmp(szServiceName, "") != 0)
	{
		int servicesCount = 0;
		std::cout << "AES T-Link[" << ++servicesCount << "] : " << szServiceName << "\n";

		char *szServiceNameTemp = new char[sizeof(ServerID_t)];
		strcpy(szServiceNameTemp, szServiceName);

		serviceNameMap.clear();

		serviceNameMap.insert(std::make_pair(servicesCount, szServiceNameTemp));

		return true;
	}
	else
	{
		return false;
	}
}

bool AesManager::EnumerateServiceNames()
{
	RetCode_t nRetCode = acsEnumServerNames(ST_CSTA, DisplayServerNames, 0);

	if (nRetCode != ACSPOSITIVE_ACK)
	{
		std::cout << "[ERROR] Error Code	: " << nRetCode;
		return false;
	}
	return true;
}

bool AesManager::OpenACSStream()
{
	if (!ReadServerConfig())
	{
		return false;
	};

	if (!EnumerateServiceNames())
	{
		return false;
	};

	auto configParser = ConfigParser::GetInstance();

	RetCode_t nRetCode;
	InvokeIDType_t invokeIDType = LIB_GEN_ID;
	InvokeID_t invokeID = 0;
	StreamType_t streamType = ST_CSTA;
	ServerID_t serverName;
	LoginID_t loginId;
	Passwd_t passwd;
	AppName_t appName = "";
	Level_t level = ACS_LEVEL1;
	Version_t version = "TS1-2";
	int sendQSize = 0;
	int sendExtraBufs = 5;
	int recvQSize = 0;
	int recvExtraBufs = 5;
	ATTPrivateData_t privateData;

	// T-LinkName
	std::string strServerName = configParser->getServerName();
	strcpy(serverName, strServerName.c_str());
	std::cout << "Server T-Link Name : " << serverName << std::endl;

	// loginId / loginPwd
	std::string strLoginId = configParser->getLoginId();
	strcpy(loginId, strLoginId.c_str());
	std::cout << "loginId : " << loginId << std::endl;
	std::string strLoginPwd = configParser->getLoginPwd();
	strcpy(passwd, strLoginPwd.c_str());
	std::cout << "passwd : " << passwd << std::endl;

	// PrivateData
	strcpy(privateData.vendor, "VERSION");
	privateData.data[0] = PRIVATE_DATA_ENCODING;
	std::string strPrivatData = configParser->getPrivateData();

	// Open an ACS stream
	nRetCode = acsOpenStream(
		&acs_handle_,
		invokeIDType, // library will generate invokeIDs in  the range 1-32767  or  APP_GEN_ID : application will provide invokeIDs any 4-byte value is legal;
		invokeID,	  // This param is ignored when the 2nd parameter is LIB_GEN_ID
		streamType,	  // requesting CSTA stream type.
		&serverName,  // CTI Link name "AVAYA#SWITCH1#CSTA#SERVERNAME1"

		&loginId, // CTI user login ID
		&passwd,  // CTI user password
		&appName, // name of the application
		level,	  // LIB Version, will be ignored
		&version, // API Version

		sendQSize,	   // send queue size using default 0
		sendExtraBufs, // send extra buf size
		recvQSize,	   // receive queue size using default 0
		recvExtraBufs, // receive extra bufs
		NULL
		//	(PrivateData_t *)&privateData // buffer for Private Data
	);

	if (nRetCode > 0)
	{
		nOpenStreamInvokeID_ = (int)nRetCode;
		std::cout << "acsOpenStream nRetCode = " << nRetCode << ", handle=" << acs_handle_ << std::endl;
		// StartEvtThread();
	}
	else if (nRetCode < 0)
	{
		std::cout << "acsOpenStream nRetCode < 0 :  " << nRetCode << std::endl;

		switch (nRetCode)
		{
		case ACSPOSITIVE_ACK:
		{

			return false;
		}
		case ACSERR_APIVERDENIED:
		{
			std::cout << " Error: acsOpenStream method failed to open stream\n"
					  << std::endl;
			std::cout << " Error: API Version is incorrect. Trying again\n"
					  << std::endl;
			std::cout << "  Error code: " << nRetCode << std::endl;
			return false;
		}
		case ACSERR_BADPARAMETER:
		{
			std::cout << " Error: ACSERR_BADPARAMETER\n"
					  << std::endl;
			std::cout << "  Error code: " << nRetCode << std::endl;
			return false;
		}
		default:
		{
			const int SLEEP_TIME = 5;
			std::cout << " Error: acsOpenStream method failed to open strea\n"
					  << std::endl;
			std::cout << "  Error code: " << nRetCode << std::endl;

			sleep(SLEEP_TIME);
			return false;
		}
		}
	}
	else
	{
		return false;
	}
	return true;
}

void AesManager::CloseACSStream()
{
	RetCode_t nRetCode = acsCloseStream(acs_handle_, 0, NULL);

	if (nRetCode < 0)
	{
		if (nRetCode == ACSERR_BADHDL)
		{
			std::cout << "[ERRPR] Close ACS Stream  :  ACS Handle is invalid " << std::endl;
		}
		else
		{
			std::cout << "[ERRPR] Close ACS Stream  Error code : " << nRetCode << std::endl;
		}
	}
	else
	{
		std::cout << "Close ACS Stream Success " << std::endl;

		nCloseStreamInvokeID_ = nRetCode;
	}
}

// Function to split a string by delimiter to a vector
std::vector<std::string> split_string_by_delimiter(std::string str, char delimiter)
{
	// Create a stringstream object.
	std::stringstream ss(str);

	// Create a vector to store the tokens.
	std::vector<std::string> tokens;

	// Read the tokens from the stream.
	std::string token;
	while (getline(ss, token, delimiter))
	{
		tokens.push_back(token);
	}

	return tokens;
}

int AesManager::MonitoringDevice(std::string registDn)
{

	DeviceID_t szDeviceID;
	strncpy(szDeviceID, registDn.c_str(), sizeof(szDeviceID));

	RetCode_t nRetCode;
	const InvokeID_t invokeID = 0;
	CSTAMonitorFilter_t filter;
	const CSTACallFilter_t call = cfQueued;
	filter.call = call;

	const CSTAFeatureFilter_t feature = 0;
	const CSTAAgentFilter_t agent = 0;
	const CSTAMaintenanceFilter_t maintenance = 0;
	const long privateFilter = 0;

	filter.feature = feature;
	filter.agent = agent;
	filter.maintenance = maintenance;
	filter.privateFilter = privateFilter;

	nRetCode = cstaMonitorDevice(acs_handle_, invokeID, &szDeviceID, &filter, NULL);

	if (nRetCode < 0)
	{
		std::cout << "Error: Monitoring device Fail " << nRetCode << std::endl;
		std::cout << "  Error code: " << nRetCode << std::endl;
	}
	else
	{
		std::mutex monitoringDevice_mutex; // monitoringDevice_mutex;
		std::cout << "registDn : " << registDn << " | szDeviceID : " << szDeviceID << " | invokeID(nRetCode) : " << nRetCode << std::endl;

		std::lock_guard<std::mutex> monitoringDevice_lock(monitoringDevice_mutex);

		auto infoManger = InfoManager::GetInstance();
		infoManger->setReqMonitoringInvokeId(nRetCode, registDn);

		std::cout << " ###########################################################" << std::endl;
		std::cout << "" << std::endl;
	}
	return nRetCode;
}

void AesManager::StartMoniringDevice()
{

	std::cout << "Monitor a device start" << std::endl;

	auto configParser = ConfigParser::GetInstance();

	try
	{
		std::string fileName = "monitorDN.INI";

		if (!configParser->readMonitoringDn(fileName))
		{
			std::cout << "[ERROR] Read Monitoring ini file error" << std::endl;
		}
	}
	catch (std::exception &e)
	{
		std::cout << "[ERROR] monitorDN.INI file read error : " << e.what() << std::endl;
		exit(0);
	}

	std::string configDn = configParser->getMonitoringDN();
	std::cout << "Monitor device list : " << configDn << std::endl;

	char delimiter = '|';

	std::vector<std::string> registerDN = split_string_by_delimiter(configDn, delimiter);

	if (registerDN.size() <= 0)
	{
		std::cout << "[ERROR] Try again ini file Input Device Monitoring List" << std::endl;
	}
	std::for_each(registerDN.cbegin(), registerDN.cend(), [this](auto dn)
				  { MonitoringDevice(dn); });
}

bool AesManager::ReadServerConfig()
{

	std::cout << "Read Server config start" << std::endl;
	auto configParser = ConfigParser::GetInstance();
	try
	{
		std::string fileName = "config.INI";
		if (!configParser->readConfig(fileName))
		{
			std::cout << "[ERROR] Read config ini file error" << std::endl;
			return false;
		}

		return true;
	}
	catch (std::exception &e)
	{
		std::cout << "[ERROR] config.INI file read error : " << e.what() << std::endl;
		return false;
	}
}

int AesManager::MakeCall(std::string callingDn, std::string calledDn, std::string userdata)
{

	RetCode_t nRetCode = 0;
	InvokeID_t invokeID = 0;
	DeviceID_t callingDevice;
	strncpy(callingDevice, callingDn.c_str(), sizeof(callingDevice));
	DeviceID_t calledDevice;
	strncpy(calledDevice, calledDn.c_str(), sizeof(calledDevice));

	// ATTUserToUserInfo_t userTouser;
	ATTPrivateData_t privData;
	// userTouser.type = uuiIa5Ascii;
	// userTouser.data.length = (short)strlen(szUserdata);
	// (void)strcpy((char*)userTouser.data.value, szUserdata);

	// attV15MakeCall(&privData, NULL, false, &userTouser, vidDisabled);
	nRetCode = cstaMakeCall(acs_handle_, invokeID, &callingDevice, &calledDevice, (PrivateData_t *)&privData);

	if (nRetCode < 0)
	{
		std::cout << "make call fail - " << callingDn << " to " << calledDn << " nRetCode : " << nRetCode << std::endl;

		return -1;
	}
	else
	{
		std::cout << "make call success - " << callingDn << " to " << calledDn << " nRetCode : " << nRetCode << std::endl;
		return nRetCode;
	}

	return nRetCode;
}

int AesManager::ClearCall(std::string callingDn, int callId)
{
	RetCode_t nRetCode = 0;
	InvokeID_t invokeID = 0;

	DeviceID_t callingDevice;
	strncpy(callingDevice, callingDn.c_str(), sizeof(callingDevice));

	ConnectionID_t connId;
	connId.callID = callId;
	memcpy(connId.deviceID, callingDevice, sizeof(connId.deviceID));

	ATTPrivateData_t privData;

	nRetCode = cstaClearCall(acs_handle_, invokeID, &connId, (PrivateData_t *)&privData);

	return nRetCode;
}