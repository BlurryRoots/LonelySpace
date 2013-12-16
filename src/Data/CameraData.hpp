
#include <BlurryRoots/Yanecos/IData.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <vector>

#ifndef _CAMERA_DATA_
#define _CAMERA_DATA_

struct CameraData
    : public BlurryRoots::Yanecos::IData
{
    sf::View view;
};

#endif
