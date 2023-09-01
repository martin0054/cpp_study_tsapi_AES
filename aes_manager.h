#ifndef _TSAPI_TEST_AES_MANAGER_H_
#define _TSAPI_TEST_AES_MANAGER_H_

class Aes_manager;

class Aes_manager  : public Singleton<Aes_manager>
{
  
public:
    Aes_manager(){ std::cout << "Aes_manager constructed" << std::endl; }
        
    ~Aes_manager(){ std::cout << "Aes_manager destructed" << std::endl;}
   

private:
    bool ReadServerConfig();
    void EnumerateServiceNames();

	// Store handle of the ACS stream
	std::unique_ptr<ACSHandle_t> acs_handle_;

// RetCode_t acsOpenStream(
// ACSHandle_t *acsHandle, /* RETURN */

/* INPUT 

unsigned long InvokeID_t invokeID


InvokeIDType_t invokeIDType
enum {
APP_GEN_ID,  :  application will provide invokeIDs; 
LIB_GEN_ID  : library will generate invokeIDs in
} InvokeIDType_t;

보통 0 

treamType_t streamType
typedef enum StreamType_t {
ST_CSTA = 1,
ST_OAM = 2,
} StreamType_t;

char ServerID_t[49] *serverID
char LoginID_t[49] *loginID
char Passwd_t[49]  *passwd
char AppName_t[21] *applicationName

Level_t acsLevelReq

enum Level_t {
ACS_LEVEL1 = 1,
ACS_LEVEL2 = 2,
ACS_LEVEL3 = 3,
ACS_LEVEL4 = 4
} Level_t;

char Version_t[21] *apiVer
unsigned short sendQSize : 0, 
 unsigned short sendExtraBufs :0 
 unsigned short recvQSize, :0
 unsigned short recvExtraBufs : 0/
PrivateData_t *privateData); 
*/

public : 
    bool OpenACSStream();

    // Handle object used to wait for acsOpenStreamConf Event(Global)
	sem_t hOpenStreamConfEvent_;	
	// Handle object used to wait for acsCloseStreamConf Event
	sem_t hCloseStreamConfEvent_;	
};

#endif
