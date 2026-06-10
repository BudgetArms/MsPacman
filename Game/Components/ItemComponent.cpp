#include "ItemComponent.hpp"

#include "Components/TextureComponent.hpp"


using namespace Game;


ItemComponent::ItemComponent(bae::GameObject& owner, const ItemType item) :
    Component(owner),
    m_ItemType{ item }
{
    std::string texturePath{};
    switch(m_ItemType)
    {
        case ItemType::PacDot:
            texturePath = "Textures/Pickups/PacDot.png";
            break;
        case ItemType::PowerPellet:
            texturePath = "Textures/Pickups/PowerPellet.png";
            break;
        case ItemType::Fruit:
            // TODO: randomize this to have other types of fruit textures
            texturePath = "Textures/Pickups/Cherries.png";
            break;
    }

    m_Owner->AddComponent<bae::TextureComponent>(owner, texturePath);
}

ItemType ItemComponent::GetItemType() const
{
    return m_ItemType;
}

