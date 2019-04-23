#include "j1App.h"
#include "CardManager.h"
#include "Deck.h"
#include "EncounterNode.h"
#include "EncounterTree.h"
#include "StrategyBuilding.h"
#include "CardManager.h"
#include "EntityManager.h"

#include "GameManager.h"



GameManager::GameManager()
{

}


GameManager::~GameManager()
{
}

bool GameManager::Awake(pugi::xml_node &)
{

	return true;
}

bool GameManager::Start()
{
	encounter_tree = new EncounterTree();
	encounter_tree->CreateTree();

	CreatePlayerDeck();

	return true;
}

bool GameManager::CleanUp()
{
	delete combat_deck;
	collection.clear();
	encounter_tree->CleanTree();

	return true;
}

EncounterTree* GameManager::GetEncounterTree()
{
	return encounter_tree;
}

Deck * GameManager::GetPlayerDeck()
{
	return combat_deck;
}

bool GameManager::IsInPlayerDeck(Card * card)
{
	bool ret = false;

	Card* deck_card = combat_deck->GetCard(card->type);

	if (deck_card) {
		ret = true;
	}
	else {
		ret = false;
	}

	return ret;
}

void GameManager::CreatePlayerDeck()
{
	collection.push_back(App->card_manager->CreateCard(EntityType::G_I));
	collection.push_back(App->card_manager->CreateCard(EntityType::SNIPER));
	collection.push_back(App->card_manager->CreateCard(EntityType::NAVY_SEAL));
	collection.push_back(App->card_manager->CreateCard(EntityType::GRIZZLY));
	collection.push_back(App->card_manager->CreateCard(EntityType::ROBOT));
	collection.push_back(App->card_manager->CreateCard(EntityType::PRISM));
	collection.push_back(App->card_manager->CreateCard(EntityType::NIGHTHAWK));
	collection.push_back(App->card_manager->CreateCard(EntityType::HARRIER));
	collection.push_back(App->card_manager->CreateCard(EntityType::BLACK_EAGLE));

	combat_deck = new Deck();
	combat_deck->AddCard(GetCardFromCollection(EntityType::G_I));
	combat_deck->AddCard(GetCardFromCollection(EntityType::SNIPER));
	combat_deck->AddCard(GetCardFromCollection(EntityType::GRIZZLY));
	combat_deck->AddCard(GetCardFromCollection(EntityType::HARRIER));
}

bool GameManager::Restart()
{
	for (std::list<Card*>::iterator card = collection.begin(); card != collection.end(); ++card)
	{
		App->card_manager->DeleteCard((*card));
	}		
	collection.clear();
	delete combat_deck;
	encounter_tree->CleanTree();
	Start();

	return true;
}



Card * GameManager::GetCardFromCollection(EntityType card_type)
{
	Card* card = nullptr;
	for each (Card* c in collection)
	{
		if (c->type == card_type)
		{
			card = c;
		}
	}

	if (card != nullptr)
	{
		return card;
	}
	else {
		LOG("The card u tried to get is not existent in collection");
		return nullptr;
	}
}

void GameManager::AddCardToCollection(EntityType card_type)
{
	bool found = false;

	for each (Card* c in collection)
	{
		if (c->type == card_type)
		{
			found = true;
			c->Upgrade();
		}
	}

	if(!found) collection.push_back(App->card_manager->CreateCard(card_type));
	LOG("KAJAWOD");
}
