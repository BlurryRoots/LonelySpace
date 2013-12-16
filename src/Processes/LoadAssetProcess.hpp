
#include "BaseProcess.hpp"

#include "../Events/AssetsReadyEvent.hpp"

#include <BlurryRoots/Yanecos/AssetManager.hpp>
#include <BlurryRoots/Yanecos/EventManager.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>

#ifndef _LOADASSETPROCESS_
#define _LOADASSETPROCESS_

template<class AssetT>
class LoadAssetProcess
    : public BaseProcess
{
    protected:

        BlurryRoots::Yanecos::AssetManager& _assetManager;

        std::string _path;

        std::string _key;

    public:

        LoadAssetProcess(
            BlurryRoots::Yanecos::EventManager& someEventManager,
            BlurryRoots::Yanecos::AssetManager& someAssetManager,
            std::string somePath,
            std::string someKey )
            : BaseProcess( someEventManager ),
              _assetManager( someAssetManager ),
              _path( somePath ),
              _key( someKey )
        {
        }

        void
        OnStart()
        {
            std::cout << "About to load " << _path << " as " << _key << "\n";
        }

        void
        Update( sf::Time someDeltaTime )
        {
            auto sfmlAsset = std::make_shared<AssetT>();

            if( ! sfmlAsset->loadFromFile( _path.c_str() ) )
            {
                _status = BlurryRoots::Yanecos::IProcess::Error;
                return;
            }

            _assetManager.Add<AssetT>( _key, sfmlAsset );

            std::cout << "Loaded " << _path << " as " << _key << "\n";
            
            _status = BlurryRoots::Yanecos::IProcess::Status::Done;

            _eventManager.Queue<AssetsReadyEvent>(
                std::make_shared<AssetsReadyEvent>( _key )
            );
        }
};

#endif
