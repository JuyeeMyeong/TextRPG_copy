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
}

void CombatContents::EnterContents()
{
    int monsterID = 10001 + rand() % 10;
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
            if (IsDead())
            {
                // 경험치 상승 처리 후 전투종료
                return false;
            }

            MonsterAttack();
            if(IsDead())
            {
                return false;
            }

            return true;
        } else if (commandContext == 'p')
        {
            bool used = false;
            
            for (Item* item : player->getInventory())
            {
                // Potion 사용 처리 
            }
        } else if (commandContext == 'x')
        {
            std::cout << "You ran away from the battle." << std::endl;
            return false;

        } else
        {
            std::cout << "You can only command 'a' for attack." << std::endl;
        }

        isBattleOngoing  = !IsDead();

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

bool CombatContents::MonsterAttack()
{
    monster->attack(*player);
    std::cout << monster->getName() << " attacks! [Player HP: " << player->GetHP() << "]\n";
    return true;
}

bool CombatContents::IsDead()
{
    if (player->GetHP() <= 0) {
        std::cout << "You died. Game Over.\n";
        return false;
    }

    if (monster->GetHP() <= 0) {
        std::cout << "You defeated the monster! Congratulations\n";
        return false;
    }

    return true;
}
