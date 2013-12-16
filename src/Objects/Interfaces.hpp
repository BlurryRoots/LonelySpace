

#ifndef _OBJECTLISTING_
#define _OBJECTLISTING_

struct IInvetoryObject
{
    virtual
        ~IInvetoryObject() {}
};

struct IMapObject
{
    virtual
        ~IMapObject() {}

    virtual
        bool
        IsBlocking() = 0;
};

struct IContainer
{
    virtual
        ~IContainer() {}
};

struct IExaminable
{
    virtual
        ~IExaminable() {}

    virtual
        void
        Examine() = 0;
};

struct IUsable
{
    virtual
        ~IUsable() {}

    virtual
        void
        Use() = 0;
};

#endif