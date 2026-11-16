#ifndef WHACKAMOLESFML_GAME_H
#define WHACKAMOLESFML_GAME_H

#include <SFML/Graphics.hpp>
#include <random>
#include <string>
#include <vector>

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
  void moveAnimal(sf::Sprite& animal, sf::Vector2f& velocity, float dt);
  void restart();

  // Random movement and respawning
  sf::Vector2f randomVelocity(float minSpeed = 100.0f, float maxSpeed = 250.0f);
  void respawnAnimal(sf::Sprite& animal, sf::Vector2f& velocity);

  sf::RenderWindow& window;

  // Background
  sf::Texture background_texture{ "Data/Images/background.png" };
  sf::Sprite background{ background_texture };

  // Animal textures
  sf::Texture bird_texture{ "Data/Images/bird.png" };
  sf::Texture fox_texture{ "Data/Images/fox_Right.png" };

  // Animal sprites
  sf::Sprite bird{ bird_texture };
  sf::Sprite fox{ fox_texture };
  sf::Sprite fox2{ fox_texture };

  // "Don't click" character (red bird for now, swap in your own texture later)
  sf::Sprite bad{ bird_texture };

  // Font and text
  sf::Font font{ "Data/Fonts/OpenSans-Bold.ttf" };
  sf::Text title_text{ font, "Whack a Mole", 50 };
  sf::Text score_text{ font, "Score: 0", 30 };
  sf::Text timer_text{ font, "Time: 30", 30 };
  sf::Text game_over_text{ font, "Game Over! Click to play again", 40 };
  sf::Text play_text_menu{ font, "Play", 50 };
  sf::Text quit_text_menu{ font, "Quit", 50 };
  sf::Text welcome_text{ font,
                         "Welcome to Whack a Mole please select a option!",
                         30 };

  // Game state
  bool in_menu       = true;
  bool play_selected = true;
  bool game_over     = false;
  bool reverse       = false;

  int score      = 0;
  int spawnCount = 0;

  // Timer
  const float round_time = 30.0f;
  float time_left        = 30.0f;

  // Movement
  float speed = 200;
  sf::Vector2f fox_velocity{ -250, 120 };
  sf::Vector2f fox2_velocity{ 180, -180 };
  sf::Vector2f bad_velocity{ 150, 100 };
};

#endif // WHACKAMOLESFML_GAME_H