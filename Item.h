#pragma once
#include <string>

class Actor;
class ItemUseable;
class ItemData;

enum class ItemType
{
    Potion,
    UpgradeDamage,
    None
};

class Item
{
protected:
    ItemData* data;
    int count;
    ItemType type = ItemType::None;

    void decreaseItem();
    
public:
    ~Item();

    std::string getName() const;

    int getValue() const;

    void addItem();

    ItemType getType() const { return type; }
    
    virtual void useItem(ItemUseable&) = 0;
};

class UpgradeDamageItem : public Item
{
public:
    UpgradeDamageItem() { type = ItemType::UpgradeDamage; }

    virtual void useItem(ItemUseable&) override;
};

class PotionItem : public Item
{
public:
    PotionItem() { type = ItemType::Potion; }

    virtual void useItem(ItemUseable&) override;
};

class ItemUseable
{
private:
    Actor* owner = nullptr;
    
protected:
    ItemUseable(Actor* actor) : owner(actor) { }
    
public:
    Actor& getActor()
    {
        return *owner;
    }
    
    virtual void useItem(Item&) = 0;
};
