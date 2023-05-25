#include "boid.hpp"
#include "quadtree.hpp"
#include <stdlib.h>
#include <iostream>
#include <random>

#define PI 3.14159265

const unsigned int WIN_WIDTH = 1000;
const unsigned int WIN_HEIGHT = 1000;

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<int> distWindow(0, WIN_WIDTH);
std::uniform_int_distribution<int> velocityRange(-3, 3);

unsigned short nextID = 0;

sf::Vector2f limit(sf::Vector2f vec, float maxRange)
{
    if (vec.x > maxRange)
    {
        vec.x = maxRange;
    }
    else if (vec.x < (maxRange * -1))
    {
        vec.x = (maxRange * -1);
    }
    if (vec.y > maxRange)
    {
        vec.y = maxRange;
    }
    else if (vec.y < (maxRange * -1))
    {
        vec.y = maxRange * -1;
    }

    return vec;
}

Boid::Boid()
{
    triangle = sf::CircleShape(6, 3);
    triangle.setFillColor(sf::Color::Green);

    position.x = distWindow(rng);
    position.y = distWindow(rng);
    triangle.setPosition(position.x, position.y);

    radius = 50;
    separationScalar = 2000;
    cohesionScalar = 500;
    alignmentScalar = 20;

    maxSpeedRange = 10;

    id = nextID++;

    velocity.x = velocityRange(rng);
    velocity.y = velocityRange(rng);
}

bool Boid::operator==(const Boid &b) const
{
    return (this->id == b.id);
}

sf::Vector2f Boid::getPosition() const
{
    return this->position;
}
void Boid::setPosition(sf::Vector2f pos)
{
    position.x = pos.x;
    position.y = pos.y;
    triangle.setPosition(position);
}

int Boid::getId() const
{
    return this->id;
}

int Boid::getRadius() const
{
    return this->radius;
}

sf::Vector2f Boid::getVelocity() const
{
    return this->velocity;
}

void Boid::draw(sf::RenderWindow &window)
{
    window.draw(triangle);
}

void Boid::stayInBounds()
{
    if (position.x < 0) // Out of Bounds left side
    {
        position.x = WIN_WIDTH - 1;
    }
    else if (position.x > WIN_WIDTH) // Out of Bounds right side
    {
        position.x = 1;
    }
    if (position.y < 0) // Out of Bounds top
    {
        position.y = WIN_HEIGHT - 1;
    }
    else if (position.y > WIN_HEIGHT) // Out of Bounds bottom
    {
        position.y = 1;
    }
}

/**
 *
 * Cohesion Behavior
 *
 */
void Boid::cohesion(const std::unordered_set<Boid> &neighbors)
{
    sf::Vector2f steering(0, 0);
    int totalNeighbors = 0;

    for (auto &neighbor : neighbors)
    {
        steering += neighbor.getPosition();
        totalNeighbors++;
    }

    if (totalNeighbors > 0)
    {
        steering /= (float)totalNeighbors;
        steering -= position;
        steering /= cohesionScalar;
    }

    velocity += steering;
}

/**
 *
 * Separation Behavior
 *
 */
void Boid::separation(const std::unordered_set<Boid> &neighbors)
{
    sf::Vector2f steering(0, 0);
    int totalNeighbors = 0;

    for (auto &neighbor : neighbors)
    {
        steering += neighbor.getPosition();
        totalNeighbors++;
    }

    if (totalNeighbors > 0)
    {
        steering /= (float)totalNeighbors;
        steering -= position;
        steering /= separationScalar;
    }

    velocity -= steering;
}

/**
 *
 * Alignment Behavior
 *
 */
void Boid::alignment(const std::unordered_set<Boid> &neighbors)
{
    sf::Vector2f steering(0, 0);
    int totalNeighbors = 0;

    for (auto &neighbor : neighbors)
    {
        steering += neighbor.getVelocity();
        totalNeighbors++;
    }

    if (totalNeighbors > 0)
    {
        steering.x /= (float)totalNeighbors;
        steering.y /= (float)totalNeighbors;
        steering -= velocity;
        steering /= alignmentScalar;
    }

    velocity += steering;
}

/**
 *
 * Update / Actions per frame
 *
 *
 */
void Boid::update(QuadTree *qt)
{
    std::unordered_set<Boid> neighbors = qt->query(*this);

    alignment(neighbors);
    cohesion(neighbors);
    separation(neighbors);

    velocity = limit(velocity, maxSpeedRange);
    position += velocity;
    this->stayInBounds();

    float angle = atan2(velocity.y, velocity.x);
    angle = angle * (180 / PI) - 90;
    angle = (velocity.x < 0.0f || velocity.y < 0.0f) ? angle - 180 : angle + 180;

    triangle.setRotation(angle);
    triangle.setPosition((int)position.x, (int)position.y);
}