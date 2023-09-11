#ifndef _TSAPI_TEST_AESMANAGER_H_
#define _TSAPI_TEST_AESMANAGER_H_

using CSTA_RESPONSE_CALLBACK = std::function<void(const CSTAEvent_t* event)>;	


class AesManager;

class AesManager : public Singleton<AesManager>
{

public:
  AesManager();

  ~AesManager();

private:
  // Store handle of the ACS stream
  ACSHandle_t acs_handle_;
  Version_t privateDataVersion;
  
public:
  bool OpenACSStream();
  unsigned int nOpenStreamInvokeID_; // To store InvokeID for open stream request
  void CloseACSStream();
  unsigned int nCloseStreamInvokeID_; // To store InvokeID for close stream request
 
  static void *RecvEventThread(void *arg);
  //static void RecvThread();

  int StartEvtThread(); // Event Thread Create
  
  int MakeCall(std::string callingDn, std::string calledDn, std::string userdata );
  int ClearCall(std::string callingDn, int callId);
  int TransferCall(std::string callingDn, int oldCallID, int newCallId);

  

private:
  virtual void RecvThread();
  
  bool ReadServerConfig();
  bool EnumerateServiceNames();

  int MonitoringDevice(std::string registDn);
  void StartMoniringDevice();
  void PrintgLog(CSTAEvent_t* cstaEvent);

  

public : 
  bool CallReponseCallback(const CSTAEvent_t* event) noexcept;
private : 

  std::unordered_map<int, CSTA_RESPONSE_CALLBACK> response_callback_queue_;
	std::mutex response_callback_queue_mutex_;
};

#endif
