
#include "../Objects/Interfaces.hpp"

#include <BlurryRoots/Yanecos/IData.hpp>

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>

#ifndef _TILEDATA_
#define _TILEDATA_

struct TileData
    : public BlurryRoots::Yanecos::IData
{
    std::string type;

    std::vector<std::shared_ptr<IMapObject>> content;

    bool accessible;
};

#endif
