#ifndef _TSAPI_TEST_CONFIGPARSER_H_
#define _TSAPI_TEST_CONFIGPARSER_H_

class ConfigParser;

class  ConfigParser : public Singleton<ConfigParser>
{
   public:
         ConfigParser(){ std::cout << "ConfigParser constructed" << std::endl; }
        
         ~ConfigParser(){ std::cout << "ConfigParser destructed" << std::endl;}
   
   public:
      // static shared_ptr<ConfigParser> GetInstance() {
      //    if (!parser_) {
      //       parser_ = new ConfigParser();
      //    }
      //    return parser_;

      // }
   public:
      bool readMonitoringDn(std::string fileName);
      std::string getMonitoringDN();
      
      bool readConfig(std::string fileName);
      std::string getListenPort();
      std::string getServerName();
      std::string getServerNameSec();
      std::string getLoginId();
      std::string getLoginPwd();
      std::string getPrivateData();
      float getHbPeriod();
      float getMoniorSleep();
      bool getBlindTransfer();
      int getTsapiLicense();
      
   private:
      std::string monitoring_Dn_;
     
      std::string listen_port_;
      std::string server_name_;
      std::string server_name_sec;
      std::string login_id_;
      std::string login_pwd_;
      std::string private_data_;
      float hb_period_;
      float monitor_sleep_;
      bool blind_transfer;
      int tsapi_license_;

    //  static shared_ptr<ConfigParser> parser_;
};

#endif