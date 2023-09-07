#ifndef _TSAPI_TEST_INFOMANAGER_H_
#define _TSAPI_TEST_INFOMANAGER_H_

class InfoManager;

class InfoManager : public Singleton<InfoManager>
{
    public :

    InfoManager();

    ~InfoManager();

    private :
    
    public : 
    
    std::unordered_map<int, std::string> reqMonitoringInvokeIDMap;
   
    void setReqMonitoringInvokeId(int invokeId, std::string registDn);
    std::unordered_map<int, std::string> getReqMonitoringInvokeId();
    std::string getDeviceDnByInvokeId(int invokeId);

    void setConnId(std::string deviceDN, int connId);
    std::unordered_map<std::string, int> connIdMap;
    int getConnIdByCalledDN(std::string deviceDN);
};


#endif
