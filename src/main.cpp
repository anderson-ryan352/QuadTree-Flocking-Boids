#include <iostream>
#include <stdlib.h>
#include "boid.hpp"
#include "quadTree.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"

const unsigned int WIN_WIDTH = 1000;
const unsigned int WIN_HEIGHT = 1000;

void drawWindow(sf::RenderWindow &window, Boid (&boids)[1000])
{
        QuadTree qt(10);

        for (auto &i : boids)
        {
                i.update(&qt);
                qt.insert(i);
                i.draw(window);
        }
        qt.update(); // Quadtree requires update if not calling draw function
        // qt.draw(window);
}

int main()
{
        sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Boids Simulation", sf::Style::Default);
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(60);
        window.setActive(true);
        window.setKeyRepeatEnabled(false);

        Boid boids[1000];
        bool isRunning = true;
        bool isFlocking = false;

        sf::Event event;
        sf::Clock clock;

        // sf::sleep(sf::seconds(5));
        while (isRunning)
        {
                if (window.pollEvent(event))
                {
                        if (event.type == sf::Event::Closed)
                        {
                                isRunning = false;
                        }
                }

                if (clock.getElapsedTime() > sf::milliseconds(16))
                {
                        window.clear();
                        drawWindow(window, boids);

                        window.display();
                        clock.restart();
                }
        }
        return 0;
}