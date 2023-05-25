#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <unordered_set>
#include "boid.hpp"
#include "SFML/Graphics.hpp"

class QuadTree
{
    int capacity;
    std::unordered_set<Boid> flock;

    sf::Vector2f topLeft;
    sf::Vector2f bottomRight;

    QuadTree *northwest;
    QuadTree *northeast;
    QuadTree *southwest;
    QuadTree *southeast;

public:
    QuadTree(int cap);
    QuadTree(sf::Vector2f topL, sf::Vector2f bottomR, int cap);

    void insert(Boid &b);
    std::unordered_set<Boid> query(Boid &b) const;

    bool inBounds(sf::Vector2f coords);
    bool intersects(Boid &b) const;

    void update();
    void draw(sf::RenderWindow &window);
};
#endif