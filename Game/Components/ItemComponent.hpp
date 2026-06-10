#pragma once

#include "Components/Component.hpp"


namespace Game
{
    enum class ItemType
    {
        PacDot,
        PowerPellet,
        Fruit,
    };

    class ItemComponent final : public bae::Component
    {
    public:
        explicit ItemComponent(bae::GameObject& owner, ItemType item);

        [[nodiscard]] ItemType GetItemType() const;

    private:
        ItemType m_ItemType;
    };
}
