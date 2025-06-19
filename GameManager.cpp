#include "Manager.h"
#include "GameManager.h"
#include "DataManager.h"
#include "InputModule.h"
#include "Contents.h"
#include "Actor.h"

template<>
GameManager* Manager<GameManager>::instance = nullptr;

template<>
GameManager* Manager<GameManager>::Instance()
{
    if (instance == nullptr)
    {
        instance = new GameManager();
    }

    return instance;
}

GameManager::GameManager()
{
    contentsMap[ContentsType::Menu] = new MenuContents();
    contentsMap[ContentsType::Combat] = new CombatContents();
    contentsMap[ContentsType::Shop] = new ShopContents();

    player = new Player();
    inputModule = new InputModule();
}

GameManager::~GameManager()
{
    for (auto contents : contentsMap)
    {
        delete contents.second;
    }

    delete player;
    delete inputModule;
}

void GameManager::InitGame()
{
    std::cout << "DEBUGGING: GameManager::InitGame()" << std::endl;
    contentsMap[ContentsType::Menu]->InitContents();
    contentsMap[ContentsType::Combat]->InitContents();
    contentsMap[ContentsType::Shop]->InitContents();

    currentContents = ContentsType::Menu;
}

void GameManager::EnterGame()
{
    while (IsNickNameEmpty())
    {
        std::string name;
        
        std::cout << "Please enter your nickname" << std::endl;
        std::getline(std::cin, name); // std::cin >> name;

        if (name.find(' ') != std::string::npos)
        {
            std::cout << "Please enter your nickname without space" << std::endl;
            continue;
        }

        player->SetName(name);
    }

    auto playerData = DataManager::Instance()->playerData.getData(1);
    player->setData(playerData);

    contentsMap[currentContents]->EnterContents();
}

bool GameManager::UpdateGame()
{
    if (currentContents == ContentsType::Combat)
    {
        Command dummyCommand;
        bool keepGoing = contentsMap[currentContents]->UpdateContents(dummyCommand);

		if (!keepGoing)
		{
			contentsMap[currentContents]->ExitContents();
            Command returnMenuCommand;
            returnMenuCommand.setCommand('0');
            ChangeContents(returnMenuCommand);
		}
        return true;
    }

    Command currentCommand;
    if  (inputModule->Execute())
    {
        currentCommand = inputModule->getCurrentCommand();
    }
    else
    {
        return true;
    }

    if (currentCommand.getCommand() == 'q')
    {
        ExitGame();
        
        return false;
    }

    bool keepGoing = contentsMap[currentContents]->UpdateContents(currentCommand);

    ChangeContents(currentCommand);

    return true;
}

void GameManager::ExitGame()
{
    exit(0);   
}

bool GameManager::IsNickNameEmpty()
{
    return player->IsNicknameEmpty();
}

void GameManager::ChangeContents(Command& command)
{
    std::string commandContext = "";
    commandContext += command.getCommand();

    std::cout << "DEBUGGING: Changing contents with command: " << command.getCommand() << std::endl;

    contentsMap[currentContents]->ExitContents();
    
    int commandNum = std::stoi(commandContext);
    currentContents = static_cast<ContentsType>(commandNum);

    std::cout << "DEBUGGING: Switched to contents index: " << commandNum << std::endl;

    contentsMap[currentContents]->EnterContents();
}