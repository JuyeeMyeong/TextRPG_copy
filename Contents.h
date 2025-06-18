#pragma once

#include <iostream>
#include <vector>
#include <functional>

class Command;
class Monster;
class Player;

enum class ContentsType
{
    Menu,
    Combat,
    Shop
};

class EnumeratorSequencer
{
private:
    std::vector<std::function<bool(Command&)>> sequencer;

public:
    void operator+=(std::function<bool(Command&)> func);
    void operator<<(std::function<bool(Command&)> func);

    bool ExecuteSequence(Command& command);
};

class Contents
{
protected:
    EnumeratorSequencer* sequencer;
    
public:
    Contents();
    ~Contents();
    
    virtual void InitContents() = 0;
    virtual void EnterContents() = 0;
    bool UpdateContents(Command& command);
    virtual void ExitContents() = 0;
};

class MenuContents : public Contents
{
public:
    virtual void InitContents() override;
    virtual void EnterContents() override;
    virtual void ExitContents() override;

    bool IsNickNameNotEmpty(Command& command);
    bool CanChangeContents(Command& command);
};

class CombatContents : public Contents
{
private:
    Monster* monster;
    Player* player;
    
public:
    virtual void InitContents() override;
    virtual void EnterContents() override;
    virtual void ExitContents() override;

    bool HandlePlayerCommand(Command& command); // 공격, 전투 종료, 아이템 사용 로직
    bool HandleMonsterAttack(Command& command); // 몬스터의 반격 로직, 플레이어가 공격하면 바로 실행
    bool IsDead(Command& command); // 플레이어나 몬스터가 죽었는지 확인 -> 승패처리
};

class ShopContents : public Contents
{
public:
    virtual void InitContents() override;
    virtual void EnterContents() override;
    virtual void ExitContents() override;
};
