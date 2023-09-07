#include "header.h"

template <typename T>
std::shared_ptr<T> Singleton<T>::instance_;

InfoManager::InfoManager()
{
    std::cout << "InfoManager constructed" << std::endl;
    reqMonitoringInvokeIDMap.clear();
    connIdMap.clear();
}

InfoManager::~InfoManager()
{
    std::cout << "InfoManager destructed" << std::endl;
}

void InfoManager::setReqMonitoringInvokeId(int invokeId, std::string registDn)
{

    std::cout << "setReqMonitoringResult - registDn : " << registDn << " | invokeID(nRetCode) : " << invokeId << std::endl;
    reqMonitoringInvokeIDMap.insert(make_pair(invokeId, registDn));

    for (std::pair<int, std::string> elem : reqMonitoringInvokeIDMap)
    {
        std::cout << "key : " << elem.first << " value : " << elem.second << std::endl;
    }
}

std::unordered_map<int, std::string> InfoManager::getReqMonitoringInvokeId()
{
    return reqMonitoringInvokeIDMap;
}

std::string InfoManager::getDeviceDnByInvokeId(int invokeId)
{
    auto iter = reqMonitoringInvokeIDMap.find(invokeId);
    if (iter != reqMonitoringInvokeIDMap.end())
    {
        std::cout << "invokeId : "<< iter->first << ", DeviceDN : " << iter->second << std::endl;
        return iter->second;
    }else{
        return 0;
    }
}



void InfoManager::setConnId(std::string deviceDN, int connId){
    std::cout << "setConnId - connId : " << connId << " | deviceDN : " << deviceDN << std::endl;
    connIdMap.insert(make_pair(deviceDN,connId));

    for (std::pair<std::string, int> elem : connIdMap)
    {
        std::cout << "key : " << elem.first << " value : " << elem.second << std::endl;
    }
}

int InfoManager::getConnIdByCalledDN(std::string deviceDN)
{
    auto iter = connIdMap.find(deviceDN);
    if (iter != connIdMap.end())
    {
        std::cout << "deviceDN : "<< iter->first << ", connId : " << iter->second << std::endl;
        return iter->second;
    }else{
        return 0;
    }
}
