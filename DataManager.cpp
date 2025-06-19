#include "Manager.h"
#include "DataManager.h"
#include "GameData.h"
#include "Parser.h"

template<typename TData>
DataContainer<TData>::~DataContainer()
{
    for (auto data : dataContainer)
    {
        delete data;
    }
}

template<>
void DataContainer<PlayerData>::addData(PlayerData* data)
{
    int containerIndex = dataContainer.size();
    dataIndexById[data->getIndex()] = containerIndex;
    this->dataContainer.push_back(data);
}

template<>
PlayerData* DataContainer<PlayerData>::getData(int id)
{
    if (dataIndexById.find(id) == dataIndexById.end())
    {
        return nullptr;
    }

    return dataContainer[dataIndexById[id]];
}

template<>
void DataContainer<MonsterData>::addData(MonsterData* data)
{
    int containerIndex = dataContainer.size();
    dataIndexById[data->getIndex()] = containerIndex;
    this->dataContainer.push_back(data);
}

template<>
MonsterData* DataContainer<MonsterData>::getData(int id)
{
    if (dataIndexById.find(id) == dataIndexById.end())
    {
        return nullptr;
    }

    return dataContainer[dataIndexById[id]];
}

template<>
void DataContainer<ItemData>::addData(ItemData* data)
{
    int containerIndex = dataContainer.size();
    dataIndexById[data->getIndex()] = containerIndex;
    this->dataContainer.push_back(data);
}

template<>
ItemData* DataContainer<ItemData>::getData(int id)
{
    if (dataIndexById.find(id) == dataIndexById.end())
    {
        return nullptr;
    }

    return dataContainer[dataIndexById[id]];
}

DataInitializer::DataInitializer()
{
    parser = new CSVParser();

    initializer.emplace_back(PlayerData::InitializeData);
    initializer.emplace_back(MonsterData::InitializeData);
    initializer.emplace_back(ItemData::InitializeData);
}

void DataInitializer::Execute(DataManager& dataManager)
{
    for (const auto& init : initializer)
    {
        init(dataManager, *parser);
    }
}

template<>
DataManager* Manager<DataManager>::instance = nullptr;

template<>
DataManager* Manager<DataManager>::Instance()
{
    if (instance == nullptr)
    {
        instance = new DataManager();
    }

    return instance;
}

DataManager::DataManager() : Manager<DataManager>(), initializer(new DataInitializer())
{
    
}

DataManager::~DataManager()
{
    delete initializer;
}

void DataManager::InitGame()
{
    initializer->Execute(*this);
}

void DataManager::EnterGame()
{
    
}

bool DataManager::UpdateGame()
{
    return true;
}

void DataManager::ExitGame()
{
    
}

#define Test

void DataManager::PrintData()
{
#ifdef Test
    playerData.printData();
    monsterData.printData();
    itemData.printData();
#endif
}
