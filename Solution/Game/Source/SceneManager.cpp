#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"

#include "SceneManager.h"

#include "SceneTitle.h"
#include "SceneGameplay.h"

#include "Defs.h"
#include "Log.h"

#define FADEOUT_TRANSITION_SPEED	2.0f
#define FADEIN_TRANSITION_SPEED		2.0f

SceneManager::SceneManager(App* a, Input* inp, Render* ren, Textures* textures) : Module()
{
	name.Create("scenemanager");

	this->input = inp;
	this->render = ren;
	this->tex = textures;
	audio = new Audio();
	this->app = a;
}

// Destructor
SceneManager::~SceneManager()
{}

// Called before render is available
bool SceneManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene Manager");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneManager::Start()
{
	LOG("Scene Manager Start");
	bool ret = true;

	audio->Load();

	current = new SceneGameplay(app);
	current->Load(tex, audio, render);

	next = nullptr;

	return ret;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	LOG("Updating Current Scene");
	bool ret = true;

	if (input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) current->showColliders = !current->showColliders;

	if (!onTransition)
	{
		ret = current->Update(input, audio, dt);
	}
	else
	{
		if (!fadeOutCompleted)
		{
			transitionAlpha += (FADEOUT_TRANSITION_SPEED * dt);

			// NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
			// For that reason we compare against 1.01f, to avoid last frame loading stop
			if (transitionAlpha > 1.01f)
			{
				transitionAlpha = 1.0f;

				current->UnLoad(tex, audio, render);	// Unload current screen
				next->Load(tex, audio, render);	// Load next screen

				RELEASE(current);	// Free current pointer
				current = next;		// Assign next pointer
				next = nullptr;

				// Activate fade out effect to next loaded screen
				fadeOutCompleted = true;
			}
		}
		else  // Transition fade out logic
		{
			transitionAlpha -= (FADEIN_TRANSITION_SPEED * dt);

			if (transitionAlpha < -0.01f)
			{
				transitionAlpha = 0.0f;
				fadeOutCompleted = false;
				onTransition = false;
			}
		}
	}

	// Draw current scene
	current->Draw(render);

	// Draw full screen rectangle in front of everything
	if (onTransition)
	{
		render->DrawRectangle(current->cam1, { -current->cam1->GetBounds().x, -current->cam1->GetBounds().y, 1280, 720 }, 0, 0, 0, (unsigned char)(255.0f * transitionAlpha));
	}

	if (current->transitionRequired)
	{
		onTransition = true;
		fadeOutCompleted = false;
		transitionAlpha = 0.0f;

		switch (current->nextScene)
		{
		case SceneType::TITLE: next = new SceneTitle(); break;
		case SceneType::GAMEPLAY: next = new SceneGameplay(app); break;
		default: break;
		}
		current->transitionRequired = false;
	}

	// Quit the game
	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;

	return ret;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	LOG("Freeing Scene Manager");
	bool ret = true;

	return ret;
}

bool SceneManager::LoadGame(pugi::xml_node& load)
{
	LOG("Loading Scene Manager");
	bool ret = true;

	current->LoadState(load.child(current->name.GetString()));

	return ret;
}

bool SceneManager::SaveGame(pugi::xml_node& save) const
{
	LOG("Saving Scene Manager");
	bool ret = true;

	current->SaveState(save.append_child(current->name.GetString()));

	return ret;
}