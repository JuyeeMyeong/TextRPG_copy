#include <iostream>

#include "Contents.h"
#include "Actor.h"
#include "DataManager.h"
#include "Manager.h"
#include "InputModule.h"
#include "GameManager.h"

void CombatContents::InitContents()
{
    std::cout << "DEBUGGING: combatcontents::initcontents() " << this << std::endl;

    monster = new Monster();

    *sequencer << [this](Command& command) 
        { 
            std::cout << "DEBUGGING: Executing HandlePlayerCommand" << std::endl;
            return this->HandlePlayerCommand(command); 
        };
}

void CombatContents::EnterContents()
{
    bIsBoss = false; 

    int monsterID = 10001 + rand() % 10;
    MonsterData* data = Manager<DataManager>::Instance()->monsterData.getData(monsterID);
    monster->setData(data);

    player = Manager<GameManager>::Instance()->getPlayer();
    
    std::cout << "Combat" << std::endl;

    std::cout << monster->getName() << std::endl;

    if (player->GetLevel() >= 10)
    {
        bIsBoss = true;

        std::string bossName = "Boss " + monster->getName();
        monster->SetName(bossName);

        int bossHP = static_cast<int>(monster->GetHP() * (1.5 + rand() % 51 / 100.0f)); // 1.5 ~ 2.0
        int bossDmg = static_cast<int>(monster->GetDamage() * (1.5 + rand() % 51 / 100.0f));
    }
}

void CombatContents::ExitContents()
{
    if (bIsBoss)
    {
        std::cout << " You cleared the game ! " << std::endl;
        std::cout << "Total Monsters Defeated: " << player->getMonsterKillCount() << std::endl;
        std::cout << "Thank you for playing!" << std::endl;

        exit(0);
        return;
    }

    std::cout << "DEBUGGING: ExitContents() of combat contents" << std::endl;
}

bool CombatContents::HandlePlayerCommand(Command& command)
{
    static bool bMonsterIntroduced = false;
    if (!bMonsterIntroduced)
    {
        std::cout << monster->getName() << " appeared! HP: " << monster->GetHP() << ", Damage: " << monster->GetDamage() << std::endl;
        bMonsterIntroduced = true;
    }

    player->attack(*monster);
    std::cout << "You attacked " << monster->getName() << "! [Monster HP: " << monster->GetHP() << "]\n";

    if (IsDead())
    {
        bMonsterIntroduced = false;
        return false;
    }

    MonsterAttack();

    if (IsDead())
    {
        bMonsterIntroduced = false;
        return false;
    }

    // 포션 사용 처리
    // if (rand() % 100 < 30) { ... }

    return true;
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
        return true;
    }

    if (monster->GetHP() <= 0) {
        std::cout << "You defeated " << monster->getName() << "! Congratulations\n";

        // 경험치 얻기 + 레벨링
        // 골드 얻기 10 ~ 20
        int goldEarned = rand() % 11 + 10;

        if(rand() % 100 < 30)
        {
            /*
            Item* rewardItem;
            if (rand() % 2 == 0)
            {
                rewardItem = new PotionItem();
            } else
            {
                rewardItem = new UpgradeDamageItem();
            }*/

            // 아이템 인벤토리에 추가
        }

        player->addKillCount();
        player->IncreaseHP(1100);

        return true;
    }

    return false;
}
