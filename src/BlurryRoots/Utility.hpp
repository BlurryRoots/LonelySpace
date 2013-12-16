
#ifndef _UTIL_STUFF_
#define _UTIL_STUFF_

namespace BlurryRoots
{
    
namespace Util
{

template<class ImplementerT, class InterfaceT>
bool
implements( void )
{
    return 
        std::is_convertible<ImplementerT*, InterfaceT*>::value;
}

}

}

#endif
