
#include "BaseSystem.hpp"

#include <BlurryRoots/Yanecos/AssetManager.hpp>
#include <BlurryRoots/Yanecos/EventManager.hpp>
#include <BlurryRoots/Yanecos/EntityManager.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#ifndef _RENDERSYSTEM_
#define _RENDERSYSTEM_

class RenderSystem
    : public BaseSystem
{
    protected:

        BlurryRoots::Yanecos::EventManager& _eventManager;

        BlurryRoots::Yanecos::AssetManager& _assetManager;

        BlurryRoots::Yanecos::EntityManager& _entityManager;

        sf::RenderTarget& _renderTarget;

    public:

        RenderSystem(
            BlurryRoots::Yanecos::EventManager& someEventManager,
            BlurryRoots::Yanecos::AssetManager& someAssetManager,
            BlurryRoots::Yanecos::EntityManager& someEntityManager,
            sf::RenderTarget& someRenderTarget )
            : _eventManager( someEventManager ),
              _assetManager( someAssetManager ),
              _entityManager( someEntityManager ),
              _renderTarget( someRenderTarget )
        {
        }
};

#endif
