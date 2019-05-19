#include "SDL\include\SDL.h"
#include "SDL_ttf/include/SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Textures.h"
#include "Fonts.h"

Fonts::Fonts() : Module()
{
	name = "fonts";
}

// Destructor
Fonts::~Fonts()
{}

// Called before render is available
bool Fonts::Awake(pugi::xml_node& conf)
{
	LOG("Init True Type Font library");
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		const char* path = conf.child("default_font").attribute("file").as_string(DEFAULT_FONT);
		int size = conf.child("default_font").attribute("size").as_int(DEFAULT_FONT_SIZE);
		default = Load(path, size);
	}

	return ret;
}

// Called before quitting
bool Fonts::CleanUp()
{
	LOG("Freeing True Type fonts and library");

	for (std::map<std::string, TTF_Font*>::iterator item = fonts.begin(); item != fonts.end(); ++item)
	{	
		App->tex->UnLoad(item->first);
		TTF_CloseFont(item->second);
		fonts.erase(item);
	}

	fonts.clear();
	TTF_Quit();
	return true;
}

// Load new texture from file path
_TTF_Font* const Fonts::Load(const char* path, int size)
{
	TTF_Font* font = nullptr;
	std::string key = path + std::to_string(size);
	std::map<std::string, TTF_Font*>::iterator item = fonts.find(key);

	if (item == fonts.end())
	{
		TTF_Font* new_font = TTF_OpenFont(path, size);

		if (new_font == NULL)
		{
			LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
		}
		else
		{
			LOG("Successfully loaded font %s size %d", path, size);
			fonts.insert({ key, new_font });
			return new_font;
		}
	}
	else
		font = item->second;


	return font;
}

// Print text using font
SDL_Texture* Fonts::Print(const char* text, SDL_Color color, TTF_Font* font, int max_width)
{
	SDL_Texture* ret = NULL;
	SDL_Surface* surface = TTF_RenderText_Blended_Wrapped((font) ? font : default, text, color, max_width);

	if (surface == NULL)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = App->tex->LoadSurface(surface,text);
		SDL_FreeSurface(surface);
	}

	return ret;
}

// calculate size of a text
bool Fonts::CalcSize(const char* text, int& width, int& height, _TTF_Font* font) const
{
	bool ret = false;

	if (TTF_SizeText((font) ? font : default, text, &width, &height) != 0)
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
		ret = true;

	return ret;
}