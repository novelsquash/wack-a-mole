#include "Game.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

Game::Game(sf::RenderWindow& game_window) : window(game_window)
{
  srand(static_cast<unsigned int>(time(NULL)));
}

Game::~Game() {}

bool Game::init()
{
  // Textures are already loaded by the members in Game.h,
  // so here we just set up the sprites and text.

  // Bird setup
  bird.setOrigin({ bird.getLocalBounds().size.x / 2.0f,
                   bird.getLocalBounds().size.y / 2.0f });
  bird.setPosition({ 100, 100 });
  bird.setScale({ 0.5f, 0.5f });

  // Fox setup
  fox.setOrigin(
    { fox.getLocalBounds().size.x / 2.0f, fox.getLocalBounds().size.y / 2.0f });
  fox.setPosition({ 300, 200 });
  fox.setScale({ 0.5f, 0.5f });

  // Second fox setup
  fox2.setOrigin({ fox2.getLocalBounds().size.x / 2.0f,
                   fox2.getLocalBounds().size.y / 2.0f });
  fox2.setPosition({ 500, 300 });
  fox2.setScale({ 0.5f, 0.5f });

  // Bad character setup (red bird, don't click it!)
  bad.setOrigin(
    { bad.getLocalBounds().size.x / 2.0f, bad.getLocalBounds().size.y / 2.0f });
  bad.setPosition({ 800, 400 });
  bad.setScale({ 0.5f, 0.5f });
  bad.setColor(sf::Color::Red);

  // Title setup
  title_text.setPosition(
    { window.getSize().x / 2.0f - title_text.getGlobalBounds().size.x / 2.0f,
      50 });
  title_text.setFillColor(sf::Color(255, 255, 255, 200));

  // Welcome text
  welcome_text.setPosition(
    { window.getSize().x / 2.0f - welcome_text.getGlobalBounds().size.x / 2.0f,
      150 });

  // Play button
  play_text_menu.setPosition(
    { window.getSize().x / 2.0f -
        play_text_menu.getGlobalBounds().size.x / 2.0f,
      window.getSize().y / 2.0f -
        play_text_menu.getGlobalBounds().size.y / 2.0f });

  // Quit button
  quit_text_menu.setPosition(
    { window.getSize().x / 3.0f -
        quit_text_menu.getGlobalBounds().size.x / 2.0f,
      window.getSize().y / 2.0f -
        quit_text_menu.getGlobalBounds().size.y / 2.0f });

  // Score and timer positions
  score_text.setPosition({ window.getSize().x - 220.0f, 20 });
  timer_text.setPosition({ 20, 20 });

  // Game over text
  game_over_text.setFillColor(sf::Color::Red);
  game_over_text.setPosition({ window.getSize().x / 2.0f -
                                 game_over_text.getGlobalBounds().size.x / 2.0f,
                               window.getSize().y / 2.0f });

  return true;
}

void Game::render()
{
  if (in_menu)
  {
    window.draw(welcome_text);
    window.draw(play_text_menu);
    window.draw(quit_text_menu);
  }
  else
  {
    window.draw(background);
    window.draw(bird);
    window.draw(fox);
    window.draw(fox2);
    window.draw(bad);

    window.draw(title_text);
    window.draw(score_text);
    window.draw(timer_text);

    if (game_over)
    {
      window.draw(game_over_text);
    }
  }
}

void Game::restart()
{
  game_over = false;
  time_left = round_time;
  score     = 0;
  score_text.setString("Score: 0");
  timer_text.setString("Time: " + std::to_string((int)round_time));

  respawnAnimal(fox, fox_velocity);
  respawnAnimal(fox2, fox2_velocity);
  respawnAnimal(bad, bad_velocity);
}

void Game::mouseClicked(sf::Event event)
{
  // Get the current position of the mouse
  sf::Vector2i click = sf::Mouse::getPosition(window);

  // Check if the player is currently on the menu
  if (in_menu)
  {
    // Check if PLAY was clicked
    if (play_text_menu.getGlobalBounds().contains(
          sf::Vector2f((float)click.x, (float)click.y)))
    {
      in_menu = false;
    }

    // Check if QUIT was clicked
    if (quit_text_menu.getGlobalBounds().contains(
          sf::Vector2f((float)click.x, (float)click.y)))
    {
      window.close();
    }
  }
  else
  {
    // Click anywhere to play again after game over
    if (game_over)
    {
      restart();
      return;
    }

    // Check the bad character first: clicking it costs points and time
    if (collisionCheck(click, bad))
    {
      score     = std::max(0, score - 3);
      time_left = std::max(0.0f, time_left - 3.0f);
      std::cout << "BAD HIT! -3 points, -3 seconds" << std::endl;
      respawnAnimal(bad, bad_velocity);
    }
    // Check if the bird was clicked
    else if (collisionCheck(click, bird))
    {
      score++;
      std::cout << "BIRD HIT!" << std::endl;
      spawn();
    }
    // Check if the first fox was clicked
    else if (collisionCheck(click, fox))
    {
      score += 2;
      std::cout << "FOX HIT!" << std::endl;
      respawnAnimal(fox, fox_velocity);
    }
    // Check if the second fox was clicked
    else if (collisionCheck(click, fox2))
    {
      score += 2;
      std::cout << "SECOND FOX HIT!" << std::endl;
      respawnAnimal(fox2, fox2_velocity);
    }

    // Update the score displayed on the screen
    score_text.setString("Score: " + std::to_string(score));
  }
}

void Game::update(float dt)
{
  if (!in_menu && !game_over)
  {
    // COUNTDOWN
    time_left -= dt;
    if (time_left <= 0.0f)
    {
      time_left = 0.0f;
      game_over = true;
    }
    timer_text.setString("Time: " + std::to_string((int)std::ceil(time_left)));

    // BIRD MOVEMENT
    if (bird.getPosition().x >= window.getSize().x - bird.getGlobalBounds().size.x)
    {
      reverse = false;
    }

    if (bird.getPosition().x <= 0)
    {
      reverse = true;
    }

    // Move bird and flip sprite
    if (reverse)
    {
      bird.setTextureRect(sf::IntRect(
        { 0, 0 },
        { (int)bird.getTexture().getSize().x,
          (int)bird.getTexture().getSize().y }));

      bird.move({ speed * dt, 0 });
    }
    else
    {
      bird.setTextureRect(sf::IntRect(
        { (int)bird.getTexture().getSize().x, 0 },
        { -(int)bird.getTexture().getSize().x,
          (int)bird.getTexture().getSize().y }));

      bird.move({ -speed * dt, 0 });
    }

    // FOX MOVEMENT
    moveAnimal(fox, fox_velocity, dt);
    moveAnimal(fox2, fox2_velocity, dt);

    // BAD CHARACTER MOVEMENT
    moveAnimal(bad, bad_velocity, dt);
  }
}

void Game::keyPressed(sf::Event::KeyPressed keyPressEvent)
{
  // Menu keys only work on the menu
  if (!in_menu)
  {
    return;
  }

  if (
    keyPressEvent.scancode == sf::Keyboard::Scan::Left ||
    keyPressEvent.scancode == sf::Keyboard::Scan::Right)
  {
    play_selected = !play_selected;

    if (play_selected)
    {
      play_text_menu.setString("Play");
      quit_text_menu.setString("Quit");
    }
    else
    {
      play_text_menu.setString("Quit");
      quit_text_menu.setString("Play");
    }
  }
  else if (keyPressEvent.scancode == sf::Keyboard::Scan::Enter)
  {
    if (play_selected)
    {
      in_menu = false;
    }
    else
    {
      window.close();
    }
  }
}

bool Game::collisionCheck(sf::Vector2i click, sf::Sprite sprite)
{
  // Check if the mouse click is inside the sprite's bounding box
  sf::FloatRect bounds = sprite.getGlobalBounds();
  return bounds.contains(
    { static_cast<float>(click.x), static_cast<float>(click.y) });
}

void Game::moveAnimal(sf::Sprite& animal, sf::Vector2f& velocity, float dt)
{
  animal.move(velocity * dt);

  sf::FloatRect bounds = animal.getGlobalBounds();

  // Bounce off right and left
  if (
    bounds.position.x <= 0 ||
    bounds.position.x + bounds.size.x >= window.getSize().x)
  {
    velocity.x = -velocity.x;
  }

  // Bounce off top and bottom
  if (
    bounds.position.y <= 0 ||
    bounds.position.y + bounds.size.y >= window.getSize().y)
  {
    velocity.y = -velocity.y;
  }
}

void Game::spawn()
{
  spawnCount++;

  if (spawnCount == 1)
  {
    bird.setPosition({ 200, 150 });
  }
  else if (spawnCount == 2)
  {
    bird.setPosition({ 400, 250 });
  }
  else if (spawnCount == 3)
  {
    bird.setPosition({ 600, 350 });
  }
  else
  {
    spawnCount = 0;
    bird.setPosition({ 300, 200 });
  }
}

sf::Vector2f Game::randomVelocity(float minSpeed, float maxSpeed)
{
  // Random speed between minSpeed and maxSpeed on each axis
  float range = maxSpeed - minSpeed;

  float vx = minSpeed + (static_cast<float>(rand()) / RAND_MAX) * range;
  float vy = minSpeed + (static_cast<float>(rand()) / RAND_MAX) * range;

  // Random direction
  if (rand() % 2 == 0)
    vx = -vx;
  if (rand() % 2 == 0)
    vy = -vy;

  return sf::Vector2f(vx, vy);
}

void Game::respawnAnimal(sf::Sprite& animal, sf::Vector2f& velocity)
{
  // Sprite origin is its centre, so keep the whole sprite on screen
  sf::FloatRect bounds = animal.getGlobalBounds();
  float half_w         = bounds.size.x / 2.0f;
  float half_h         = bounds.size.y / 2.0f;

  // std::max(1, ...) stops a divide-by-zero if a sprite is bigger than the
  // window
  int range_x =
    std::max(1, static_cast<int>(window.getSize().x - bounds.size.x));
  int range_y =
    std::max(1, static_cast<int>(window.getSize().y - bounds.size.y));

  float x = half_w + static_cast<float>(rand() % range_x);
  float y = half_h + static_cast<float>(rand() % range_y);

  animal.setPosition({ x, y });
  velocity = randomVelocity(100.0f, 250.0f);
}