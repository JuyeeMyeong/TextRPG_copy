#include "Actor.h"

#include "Item.h"
#include "DataManager.h"
#include "GameData.h"

void Player::attack(Actor& target)
{
    target.damaged(*this);
}

void Player::damaged(const Actor& attacker)
{
    int damage = attacker.GetDamage() - this->GetDefense();
    this->DecreaseHP(damage);
}

void Player::useItem(Item& item)
{
    item.useItem(*this);
}

void Player::levelUp()
{
    increaseLevel();
    setData(Manager<DataManager>::Instance()->playerData.getData(level));
}

bool Player::IsNicknameEmpty()
{
    return name.empty();
}

void Player::setData(PlayerData* data)
{
    this->data = data;

    level = data->getLevel();
    
    baseHP = data->getHP();
    baseDamage = data->getDamage();
    baseDefense = data->getDefense();

    currentHP = baseHP;

    int prevIncDamage = incDamage;
    int prevIncDefense = incDefense;

    incDamage = prevIncDamage;
    incDefense = prevIncDefense;
}

void Monster::attack(Actor& target)
{
    target.damaged(*this);
}

void Monster::damaged(const Actor& attacker)
{
    int damage = attacker.GetDamage() - this->GetDefense();
    this->DecreaseHP(damage);
}

void Monster::setData(MonsterData* data)
{
    this->data = data;

    level = data->getLevel();
    
    //baseHP = data->getHP();
    baseHP = rand() % (level * 11) + (level * 20);
    //baseDamage = data->getDamage();
    baseDamage = rand() % (level * 6) + (level * 5);

    baseDefense = data->getDefense();

    name = data->getName();

    currentHP = baseHP;

    incDamage = 0;
    incDefense = 0;
}

const std::string Monster::getName() const
{
    return data->getName();
}
