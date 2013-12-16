
#include "../Objects/Interfaces.hpp"

#include <BlurryRoots/Yanecos/IData.hpp>

#include <vector>

#ifndef _TILEDATA_
#define _TILEDATA_

struct TileData
    : public BlurryRoots::Yanecos::IData
{
    std::vector<std::shared_ptr<IInvetoryObject>> inventory;
};

#endif
