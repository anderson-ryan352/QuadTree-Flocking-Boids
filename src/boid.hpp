#ifndef BOID_HPP
#define BOID_HPP

#include "SFML/Graphics.hpp"
#include <string>
#include <unordered_set>
#include "SFML/System.hpp"

class QuadTree; // forward declaration

class Boid
{
    sf::Vector2f position;
    sf::Vector2f velocity;

    sf::CircleShape triangle;

    unsigned short int id;
    unsigned short int radius;
    float separationScalar;
    float cohesionScalar;
    float alignmentScalar;

    float maxSpeedRange;

public:
    Boid();

    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f pos);

    sf::Vector2f getVelocity() const;

    int getId() const;
    void setId(int &i);

    int getRadius() const;

    bool operator==(const Boid &b) const;

    void cohesion(const std::unordered_set<Boid> &neighbors);
    void separation(const std::unordered_set<Boid> &neighbors);
    void alignment(const std::unordered_set<Boid> &neighbors);

    void stayInBounds();

    void update(QuadTree *qt);
    void draw(sf::RenderWindow &window);
};

namespace std
{
    template <>
    struct hash<Boid>
    {
        size_t operator()(const Boid &b) const
        {
            return (hash<int>()(b.getId()));
        }
    };
}
#endif