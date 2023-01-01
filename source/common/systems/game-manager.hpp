#pragma once

#include "../ecs/world.hpp"
#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

	// The Game Manager system is responsible for controling the game logic.
	// keep track of the score, time etc... 
	class GameMananger {

		Application* app; // The application in which the state runs
		enum states { MENU, PLAY, GAMEOVER };
		states currentState;
		State *State;
	public:
		states menu_state = MENU;
		states play_state = PLAY;
		states gameover_state = GAMEOVER;
		//game score
		static int score;
		static double timePlayed;
		static bool gameOver;
		static bool win;

		//this static object can be used by different logic components 
		static GameMananger gm;

		// When a state enters, it should call this function and give it the pointer to the application
		void enter(Application* app, states state) {
			this->app = app;
			currentState = state;
		}
		//used to add/subtract score ex:coin collection or hit obstacle
		void upScore(int value)
		{
			score += value;
		}
        	void downScore(int value)
		{
			score -= value;
		}
		void startGame()
		{
			if(currentState != our::GameMananger::PLAY)
			app->changeState("main");
		}
		// This should be called every frame to update all entities containing a MovementComponent. 
		void update(double deltatime) {
		
			// if(gameOver)
			// {
			// 	app->changeState("menu");
			// 	return;
			// }
			if (app->getKeyboard().justPressed(GLFW_KEY_ESCAPE))
			{
				switch (currentState)
				{
				case our::GameMananger::MENU:
					app->changeState("main");
					break;
				case our::GameMananger::PLAY:
					app->changeState("menu");
					break;
				default:
					break;
				}
                
			}
			if (score == 0)
			{
				win = true;
				std::cout << "win" << std::endl;
			}
			else
			{
				win = false;
			}

		}
	};
	int GameMananger::score = 0;
	double GameMananger::timePlayed = 0.0f;
	bool GameMananger::gameOver = false;
	bool GameMananger::win = false;

	GameMananger GameMananger::gm;

}
