#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "EntityManager.h"
#include "TransitionManager.h"
#include "CardManager.h"
#include "Deck.h"
#include "GUI.h"
#include "EncounterNode.h"
#include "StrategyBuilding.h"
#include "GameManager.h"
#include "StrategyMapScene.h"
#include "Brofiler/Brofiler.h"

StrategyMapScene::StrategyMapScene() : Scene()
{

}

// Destructor
StrategyMapScene::~StrategyMapScene()
{}

// Called before the first frame
bool StrategyMapScene::Start()
{
	BROFILER_CATEGORY("SMStart", Profiler::Color::Red);

	App->map->Load("Nodes Map.tmx");
	App->ResumeGame();

	App->game_manager->GetEncounterTree()->CreateAllNodes();
	App->game_manager->GetEncounterTree()->UpdateTreeState();



	//UI
	uint w, h;
	App->win->GetWindowSize(w, h);

	main_panel = App->gui->CreateImage({ 0,0 }, { 0, 0, (int)w, (int)h }, nullptr, false);
	banner = App->gui->CreateImage({ 4,5 }, { 1,769,1017,83 }, main_panel);
	SDL_Rect small_button_rect[3];
	small_button_rect[0] = { 0,481,59,51 };
	small_button_rect[1] = { 60,481,58,51 };
	small_button_rect[2] = { 119,481,58,51 };

	SDL_Rect large_button_rect[3];
	large_button_rect[0] = { 0,533,220,51 };
	large_button_rect[1] = { 0,585,220,51 };
	large_button_rect[2] = { 0,637,220,51 };

	settings_button = App->gui->CreateButton({ 50,700 }, small_button_rect, main_panel);
	menu_button = App->gui->CreateButton({ 700,700 }, large_button_rect, main_panel);
	text_menu = App->gui->CreateLabel({ 50,10 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "MENU", { 0,0,0,0 }, 0, menu_button);

	gold = App->gui->CreateLabel({ 90, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "GOLD", { 0,0,0,0 }, 0, main_panel);
	energy = App->gui->CreateLabel({ 450, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "ENERGY", { 0,0,0,0 }, 0, main_panel);
	health = App->gui->CreateLabel({ 860, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "HEALTH", { 0,0,0,0 }, 0, main_panel);

	iPoint world_position = App->map->MapToWorld((int)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetPosition().x, (int)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetPosition().y);

	App->render->camera.x = -world_position.x + w * 0.5;
	App->render->camera.y = -world_position.y + h;

	// Troops menu
	troops_background = App->gui->CreateImage({ 20,95 }, { 789,1222,990,600 }, main_panel);
	
	backbutton_t_b = App->gui->CreateButton({ 940,100 }, small_button_rect, troops_background);
	back_cross_text = App->gui->CreateLabel({ 15,10 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 28, "X", { 0,0,0,0 }, 0, backbutton_t_b);
	App->gui->DisableElement(backbutton_t_b);
	troops_button = App->gui->CreateButton({ 380,100 }, large_button_rect, troops_background);
	troops_text = App->gui->CreateLabel({ 50,10 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "TROOPS", { 0,0,0,0 }, 0, troops_button);
	App->gui->DisableElement(troops_button);
	buildings_button = App->gui->CreateButton({ 600,100 }, large_button_rect, troops_background);
	buildings_text = App->gui->CreateLabel({ 35,10 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "BUILDINGS", { 0,0,0,0 }, 0, buildings_button);
	App->gui->DisableElement(buildings_button);


	
	deck_buttons[0] = App->gui->CreateButton({ 320,69 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[0]->type, "deck"), troops_background);
	deck_buttons[1] = App->gui->CreateButton({ 485,69 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[1]->type, "deck"), troops_background);
	deck_buttons[2] = App->gui->CreateButton({ 666,69 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[2]->type, "deck"), troops_background);
	deck_buttons[3] = App->gui->CreateButton({ 800,69 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[3]->type, "deck"), troops_background);
	

	
	collection_buttons[0] = App->gui->CreateButton({ 31,320 }, App->gui->LoadUIButton(1, "upgrade"), troops_background);
	collection_buttons[1] = App->gui->CreateButton({ 170,320 }, App->gui->LoadUIButton(2, "upgrade"), troops_background);
	collection_buttons[2] = App->gui->CreateButton({ 130,500 }, App->gui->LoadUIButton(3, "upgrade"), troops_background);
	collection_buttons[3] = App->gui->CreateButton({ 390,320 }, App->gui->LoadUIButton(4, "upgrade"), troops_background);
	collection_buttons[4] = App->gui->CreateButton({ 550,320 }, App->gui->LoadUIButton(5, "upgrade"), troops_background);
	collection_buttons[5] = App->gui->CreateButton({ 460,500 }, App->gui->LoadUIButton(6, "upgrade"), troops_background);
	collection_buttons[6] = App->gui->CreateButton({ 720,320 }, App->gui->LoadUIButton(7, "upgrade"), troops_background);
	collection_buttons[7] = App->gui->CreateButton({ 860,320 }, App->gui->LoadUIButton(8, "upgrade"), troops_background);
	collection_buttons[8] = App->gui->CreateButton({ 800,500 }, App->gui->LoadUIButton(9, "upgrade"), troops_background);
	App->gui->DisableElement(troops_background);
	
	


	return true;
}

// Called each loop iteration
bool StrategyMapScene::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 10;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 10;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 10;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 10;
	return true;
}

// Called each loop iteration
bool StrategyMapScene::Update(float dt)
{
	BROFILER_CATEGORY("SMUpdate", Profiler::Color::Green);
	return true;
}

// Called each loop iteration
bool StrategyMapScene::PostUpdate()
{
	bool ret = true;

	App->map->Draw();

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	App->game_manager->GetEncounterTree()->DrawTreeLines();

	return ret;
}

// Called before quitting
bool StrategyMapScene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool StrategyMapScene::GUIEvent(UIElement * element, GUI_Event gui_event)
{
	

	if (gui_event == GUI_Event::LEFT_CLICK_DOWN) {
		if (element == settings_button) {
			//(options = App->gui->CreateImage();
		}
		else if (element == menu_button) {
			App->game_manager->collection;
			

			App->gui->EnableElement(troops_button);
			App->gui->EnableElement(buildings_button);
			App->gui->EnableElement(troops_background);
			App->gui->EnableElement(backbutton_t_b);

			App->gui->DisableElement(menu_button);
			App->gui->DisableElement(settings_button);



		}
		else if (element == backbutton_t_b) {
			App->gui->DisableElement(troops_background);
			App->gui->DisableElement(backbutton_t_b);
			App->gui->DisableElement(troops_button);
			App->gui->DisableElement(buildings_button);

			App->gui->EnableElement(menu_button);
			App->gui->EnableElement(settings_button);

		}
		else if (element == deck_buttons[0])
		{
			App->gui->DisableElement(deck_buttons[0]);
			App->game_manager->GetPlayerDeck()->RemoveCard(0);
		}
		else if (element == deck_buttons[1])
		{
			App->gui->DisableElement(deck_buttons[1]);
			App->game_manager->GetPlayerDeck()->RemoveCard(1);
		}
		else if (element == deck_buttons[2])
		{
			App->gui->DisableElement(deck_buttons[2]);
			App->game_manager->GetPlayerDeck()->RemoveCard(2);
		}
		else if (element == deck_buttons[3])
		{
			App->gui->DisableElement(deck_buttons[3]);
			App->game_manager->GetPlayerDeck()->RemoveCard(3);
		}
		else if (element == collection_buttons[0])
		{
			AddCardToDeck(element,0);
		}
		else if (element == collection_buttons[1])
		{
			AddCardToDeck(element,1);
		}
		else if (element == collection_buttons[2])
		{
			AddCardToDeck(element,2);
		}
		else if (element == collection_buttons[3])
		{
			AddCardToDeck(element,3);
		}
		else if (element == collection_buttons[4])
		{
			AddCardToDeck(element,4);
		}
		else if (element == collection_buttons[5])
		{
			AddCardToDeck(element,5);
		}
		else if (element == collection_buttons[6])
		{
			AddCardToDeck(element,6);
		}
		else if (element == collection_buttons[7])
		{
			AddCardToDeck(element,7);
		}
		else if (element == collection_buttons[8])
		{
			AddCardToDeck(element,8);
		}
		else if (element == collection_buttons[9])
		{
			AddCardToDeck(element,9);
		}
	}

	return true;
}

void  StrategyMapScene::AddCardToDeck(UIElement * element, uint num) {
	for (int i = 0; i < 4; i++) {
		Card* card = App->game_manager->GetPlayerDeck()->GetCard((EntityType)(num+1));
		if (deck_buttons[i]->enabled == false && !card) {
			App->gui->EnableElement(deck_buttons[i]);
			App->game_manager->GetPlayerDeck()->AddCard(card);
			break;
		}
	}
}
