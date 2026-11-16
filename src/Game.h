
#ifndef WHACKAMOLESFML_GAME_H
#define WHACKAMOLESFML_GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <string>

class Game
{
	public:
	Game(sf::RenderWindow& window);
	~Game();
    bool init();
	void update(float dt);
	void render();
	void mouseClicked(sf::Event event);
	void keyPressed(sf::Event::KeyPressed keyPressEvent);
	private:
	bool collisionCheck(sf::Vector2i click, sf::Sprite sprite);
	void spawn();
    int spawnCount = 0;
    void moveAnimal(sf::Sprite& animal, sf::Vector2f& velocity, float dt); 


	sf::RenderWindow& window;

	//Define variables here e.g. Texture and Sprite variables
    sf::Texture background_texture{ "Data/Images/background.png" };
  sf::Sprite background{background_texture}; 


// Animal textures
  sf::Texture bird_texture{ "Data/Images/bird.png" };
  sf::Texture fox_texture{ "Data/Images/fox_Right.png" };
  
    // Animal sprites
    sf::Sprite bird{ bird_texture };
    sf::Sprite fox{ fox_texture };
    sf ::Sprite fox2{ fox_texture };

	sf::Font font{ "Data/Fonts/OpenSans-Bold.ttf" };
    sf::Text title_text{ font, "Whack a Mole", 50 };
    sf::Text play_text{ font, "Play", 50 };
    sf ::Text quit_text{ font, "Quit", 50 };
    sf ::Text wlelcome_text{ font, "Welcome to Whack a Mole please select a option!", 30};
    sf::Text score_text{ font, "score 0," };

  bool in_menu = true;

sf::Text play_text_menu{ font, "Play", 50 };
sf::Text quit_text_menu{ font, "Quit", 50 };
sf::Text welcome_text{ font,
                       "Welcome to Whack a Mole please select a option!",
                    30 };
  

	bool reverse = false;
  bool play_selected = true;
	float speed = 200;

  // Animal movement speeds
  float bird_speed = 200;
  float fox_speed  = 250;
  float fox2_speed = 180;

  // Animal movement directions
  sf::Vector2f bird_velocity{ 200, 150 };
  sf::Vector2f fox_velocity{ -250, 120 };
  sf::Vector2f fox2_velocity{ 180, -180 };

  // Random movement and respawning
  sf::Vector2f randomVelocity(float minSpeed = 100.0f, float maxSpeed = 250.0f);
  void respawnAnimal(sf::Sprite& animal, sf::Vector2f& velocity);

	int score = 0;
 
	

};

#endif // WHACKAMOLESFML_GAME_H
