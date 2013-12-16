
#include "Data/PositionData.hpp"
#include "Data/TileData.hpp"

#include <BlurryRoots/Yanecos/EntityManager.hpp>
#include <BlurryRoots/Yanecos/EventManager.hpp>

#include <memory>

#ifndef _MAPCREATOR_
#define _MAPCREATOR_

class MapCreator
{
    private:

        BlurryRoots::Yanecos::EntityManager& _entityManager;

        BlurryRoots::Yanecos::EventManager& _eventManager;

    public:

        MapCreator(
            BlurryRoots::Yanecos::EntityManager& someEntityManager,
            BlurryRoots::Yanecos::EventManager& someEventManager )
        : _entityManager( someEntityManager ),
          _eventManager( someEventManager )
        {}

        void CreateRoom(
            std::size_t someWidth, std::size_t someHeight,
            std::size_t someStartX, std::size_t someStartY
            )
        {
            // create map
            for( int iHeight = 0; iHeight < someHeight; ++iHeight )
            {
                for( int iWidth = 0; iWidth < someWidth; ++iWidth )
                {
                    std::size_t id = 
                        _entityManager.CreateEntity( "MAP" );
                    {
                        auto data = 
                            _entityManager.AddDataToEntity<PositionData>( id );

                        data->x = someStartX + iWidth;
                        data->y = someStartY + iHeight;
                    }
                    {
                        auto data =
                            _entityManager.AddDataToEntity<TileData>( id );

                        if( iWidth == 0 && iHeight == 0 )
                        {
                            data->type = "TopLeftCorner";
                            data->accessible = false;
                        }
                        else if( iWidth > 0 && iWidth < (someWidth - 1) && iHeight == 0 )
                        {
                            data->type = "TopWall";
                            data->accessible = false;
                        }
                        else if( iWidth == (someWidth - 1) && iHeight == 0 )
                        {
                            data->type = "TopRightCorner";
                            data->accessible = false;
                        }
                        else if( iWidth == 0 && iHeight > 0 && iHeight < (someHeight - 1) )
                        {
                            data->type = "LeftWall";
                            data->accessible = false;
                        }
                        else if( iWidth == 0 && iHeight == (someHeight - 1) )
                        {
                            data->type = "BottomLeftCorner";
                            data->accessible = false;
                        }
                        else if( iWidth > 0 && iWidth < (someWidth - 1) && iHeight == (someHeight - 1) )
                        {
                            data->type = "BottomWall";
                            data->accessible = false;
                        }
                        else if( iWidth == (someWidth - 1) && iHeight == (someHeight - 1) )
                        {
                            data->type = "BottomRightCorner";
                            data->accessible = false;
                        }
                        else if( iWidth == (someWidth - 1) && iHeight > 0 && iHeight < (someHeight - 1) )
                        {
                            data->type = "RightWall";
                            data->accessible = false;
                        }
                        else
                        {
                            data->type = "Floor";
                            data->accessible = true;
                        }
                    }
                }
            }
        }
};

#endif
