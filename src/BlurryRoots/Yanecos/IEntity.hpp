
#include "IData.hpp"

#include <memory>
#include <typeinfo>

#ifndef _IENTITY_
#define _IENTITY_

namespace BlurryRoots
{
    
namespace Yanecos
{

class IEntity
{
    protected:

        virtual
            std::shared_ptr<IData> GetDataWithTypeHash( std::size_t ) = 0;

    public:

        virtual
            ~IEntity() {}

        template<class T>
        std::shared_ptr<T>
        GetData( void )
        {
            return this->GetDataWithTypeHash( typeid( T ).hash_code() );
        }

        virtual 
            void 
            AddData( std::shared_ptr<IData> someData ) = 0;

        virtual
            std::size_t
            GetId() = 0;


};

}

}

#endif
