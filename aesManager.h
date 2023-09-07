#ifndef _TSAPI_TEST_AESMANAGER_H_
#define _TSAPI_TEST_AESMANAGER_H_

class AesManager;

class AesManager : public Singleton<AesManager>
{

public:
  AesManager();

  ~AesManager();

private:
  // Store handle of the ACS stream
  ACSHandle_t acs_handle_;

public:
  bool OpenACSStream();
  unsigned int nOpenStreamInvokeID_; // To store InvokeID for open stream request
  void CloseACSStream();
  unsigned int nCloseStreamInvokeID_; // To store InvokeID for close stream request
  static void *RecvEventThread(void *arg);
  int StartEvtThread(); // Event Thread Create
  int MakeCall(std::string callingDn, std::string calledDn, std::string userdata );
  int ClearCall(std::string callingDn, int callId);

private:
  virtual void RecvThread();
  bool ReadServerConfig();
  bool EnumerateServiceNames();
  int MonitoringDevice(std::string registDn);
  void StartMoniringDevice();
  void PrintgLog(CSTAEvent_t* cstaEvent);

};

#endif
