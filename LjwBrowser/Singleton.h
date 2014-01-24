#pragma once

template<class T>
class CSingleton
{
public:
    CSingleton(void);
    ~CSingleton(void);

    static T* Instance(){
        if(!m_instance){
            m_instance = new T;
        }
        return m_instance;
    }

private:
    static T* m_instance;
};

template<class T>
T* CSingleton<T>::m_instance = NULL;

