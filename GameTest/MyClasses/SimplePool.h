#ifndef __SIMPLEPOOL_H__
#define __SIMPLEPOOL_H__

template <class MyType> class SimplePool
{
protected:
    std::vector<MyType> m_poolContents;

public:
    SimplePool()
    {
    }

    virtual ~SimplePool()
    {
        while (m_poolContents.empty() == false)
        {
            delete m_poolContents.back();
            m_poolContents.pop_back();
        }
    }

    void AddNewToPool(MyType object)
    {
        m_poolContents.push_back(object);
    }

    MyType GetFromPool()
    {
        if (m_poolContents.size() != 0)
        {
            MyType pObject = m_poolContents.back();
            m_poolContents.pop_back();
            return pObject;
        }
        else
        {
            return nullptr;
        }


    }

    void ReturnToPool(MyType object)
    {
        m_poolContents.push_back(object);
    }
};
#endif //__ObjectPool_H__