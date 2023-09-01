#ifndef _SINGLETON_TEMPALTE_H_
#define _SINGLETON_TEMPALTE_H_

template<typename T>
class Singleton {

    public:
        Singleton() = default; // default 생성자 

        static std::shared_ptr<T> GetInstance() {
            if (!instance_) {
                std::cout << "create Instance" ;
                 instance_.reset(new T()); // Free resource and set to a new Simple instance

                // instance_ = new T();
            }
            return instance_;

        }
        Singleton(const Singleton&) = delete;
        Singleton& operator= (const Singleton) = delete;

        
    protected:

    static std::shared_ptr<T> instance_;
};


#endif
