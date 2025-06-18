#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "Item.h"

class PlayerData;
class MonsterData;

class Actor
{
protected:
    std::string name;
    
    int level = 1;
    
    int baseHP = 0;
    int baseDamage = 0;
    int baseDefense = 0;

    int currentHP = 0;
    
    int incDamage = 0;
    int incDefense = 0;

    bool isDead()
    {
        return currentHP <= 0;
    }

public:
    void SetName(const std::string& name)
    {
        this->name = name;
    }
    
    int GetHP() const
    {
        return currentHP;
    }
    
    int GetDamage() const
    {
        return baseDamage * level + incDamage;
    }

    int GetDefense() const
    {
        return baseDefense * level + incDefense;
    }

    void IncreaseHP(int hp)
    {
        this->currentHP = std::min(this->currentHP + hp, baseHP);
    }

    void DecreaseHP(int hp)
    {
        this->currentHP = std::max(0, this->currentHP - hp);
    }

    void IncreaseDamage(int damage)
    {
        this->incDamage += damage;
    }

    void DecreaseDamage(int damage)
    {
        this->incDamage = std::max(0, this->incDamage - damage);       
    }

    void IncreaseDefense(int defense)
    {
        this->incDamage += defense;
    }

    void DecreaseDefense(int defense)
    {
        this->incDefense = std::max(0, this->incDefense - defense);       
    }

    void increaseLevel()
    {
        ++this->level;
    }

    virtual void attack(Actor& target) = 0;
    virtual void damaged(const Actor& attacker) = 0;
};

class Player : public Actor, public ItemUseable
{
private:
    PlayerData* data;
    
    std::vector<Item*> inventory;

    
public:
    Player() : ItemUseable(this) { }

    void setData(PlayerData* data);
    void levelUp();
    bool IsNicknameEmpty();

    const std::vector<Item*>& getInventory() const 
    {
        return inventory;
    }
    
    virtual void attack(Actor& target) override;
    virtual void damaged(const Actor& attacker) override;

    virtual void useItem(Item&) override;
};

class Monster : public Actor
{
private:
    MonsterData* data;
    
public:
    virtual void attack(Actor& target) override;
    virtual void damaged(const Actor& attacker) override;

    void setData(MonsterData* data);

    const std::string getName() const;
};
