#include <iostream>

#include "Contents.h"
#include "Actor.h"
#include "DataManager.h"
#include "Manager.h"
#include "InputModule.h"
#include "GameManager.h"

void CombatContents::InitContents()
{
    monster = new Monster();

    *sequencer << [this](Command& command) { return this->HandlePlayerCommand(command); };
    *sequencer << [this](Command& command) { return this->HandleMonsterAttack(command); }; 
    *sequencer += [this](Command& command) { return this->IsDead(command); };
}

void CombatContents::EnterContents()
{
    int monsterID = 10001 + rand() % 10; // monster은 일단 10마리 있음, 이 중 랜덤 생성
    MonsterData* data = Manager<DataManager>::Instance()->monsterData.getData(monsterID);
    monster->setData(data);

    player = Manager<GameManager>::Instance()->getPlayer();
    
    std::cout << "Combat" << std::endl;

    std::cout << monster->getName() << std::endl;
}

void CombatContents::ExitContents()
{
    
}

bool CombatContents::HandlePlayerCommand(Command& command)
{
    char commandContext = command.getCommand();
    bool isBattleOngoing  = true;

    while(isBattleOngoing)
    {
        if(commandContext == 'a')
        {
            player->attack(*monster);
            std::cout << "You attacked " << monster->getName() << "! [Monster HP: " << monster->GetHP() << "]\n";
            return true;
        } else if (commandContext == 'x')
        {
            std::cout << "You ran away from the battle." << std::endl;
            return false;

        } else
        {
            std::cout << "You can only command 'a' for attack." << std::endl;
        }

        isBattleOngoing  = IsDead(command);

        if(isBattleOngoing)
        {
            std::cout << "Next Command: " << std::endl;
            std::cout << "a. attack" << std::endl;
            std::cout << "p. use potion" << std::endl;
            std::cout << "x. run away (Finish the battle)" << std::endl;
            std::cout << "Enter your choice:" << std::endl;
            std::cin >> commandContext;
        }
    }

    return false;
}

bool CombatContents::HandleMonsterAttack(Command& command)
{
    if (monster->GetHP() <= 0) return true; // 몬스터가 죽으면 반격 X

    monster->attack(*player);
    std::cout << monster->getName() << " attacks! [Player HP: " << player->GetHP() << "]\n";
    return true;
}

bool CombatContents::IsDead(Command& command)
{
    if (player->GetHP() <= 0) {
        std::cout << "You died. Game Over.\n";
        return false;
    }

    if (monster->GetHP() <= 0) {
        std::cout << "You defeated the monster!\n";
        return false;
    }

    return true;
}
