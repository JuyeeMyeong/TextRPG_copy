#pragma once

#include <unordered_map>

#include "Manager.h"

enum class ContentsType;
class Contents;
class Player;
class InputModule;
class Command;

class GameManager : public Manager<GameManager>
{
private:
    std::unordered_map<ContentsType, Contents*> contentsMap;
    ContentsType currentContents;

    Player* player;

    InputModule* inputModule;
    
public:
    GameManager();
    virtual ~GameManager() override;

    Player* getPlayer() const { return player; }

    virtual void InitGame() override;
    virtual void EnterGame() override;
    virtual bool UpdateGame() override;
    virtual void ExitGame() override;

    bool IsNickNameEmpty();
    void ChangeContents(Command& command);
};
