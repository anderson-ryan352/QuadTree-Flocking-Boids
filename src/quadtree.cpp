#include "quadtree.hpp"
#include <iostream>

const unsigned int WIN_WIDTH = 1000;
const unsigned int WIN_HEIGHT = 1000;

QuadTree::QuadTree(int cap)
{
    capacity = cap;

    topLeft = sf::Vector2f(0, 0);
    bottomRight = sf::Vector2f(WIN_WIDTH, WIN_HEIGHT);

    northwest = NULL;
    northeast = NULL;
    southwest = NULL;
    southeast = NULL;
}

QuadTree::QuadTree(sf::Vector2f topL, sf::Vector2f bottomR, int cap)
{
    capacity = cap;

    topLeft = topL;
    bottomRight = bottomR;

    northwest = NULL;
    northeast = NULL;
    southwest = NULL;
    southeast = NULL;
}

void QuadTree::draw(sf::RenderWindow &window)
{
    sf::RectangleShape rect(sf::Vector2f(bottomRight.x - topLeft.x, bottomRight.y - topLeft.y));

    rect.setPosition(topLeft.x, topLeft.y);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(-2);
    rect.setOutlineColor(sf::Color::Red);

    window.draw(rect);

    if (northwest != NULL)
    {
        northwest->draw(window);
        delete (northwest);
    }
    if (northeast != NULL)
    {
        northeast->draw(window);
        delete (northeast);
    }
    if (southwest != NULL)
    {
        southwest->draw(window);
        delete (southwest);
    }
    if (southeast != NULL)
    {
        southeast->draw(window);
        delete (southeast);
    }
}

void QuadTree::insert(Boid &b)
{
    sf::Vector2f bPosition = b.getPosition();

    if (!(inBounds(bPosition)))
    {
        return;
    }

    if (abs(topLeft.x - bottomRight.x) <= 5 && abs(topLeft.y - bottomRight.y) <= 5) // smallest square allowed
    {
        return;
    }

    if (flock.size() < capacity) // if flock has space
    {
        this->flock.insert(b);
    }
    else // subdivide
    {
        // west
        if ((topLeft.x + bottomRight.x) / 2 >= bPosition.x)
        {
            // northwest
            if ((topLeft.y + bottomRight.y) / 2 >= bPosition.y)
            {
                if (northwest == NULL)
                {
                    northwest = new QuadTree(
                        sf::Vector2f(topLeft.x, topLeft.y),
                        sf::Vector2f((topLeft.x + bottomRight.x) / 2,
                                     (topLeft.y + bottomRight.y) / 2),
                        this->capacity);
                }
                northwest->insert(b);
            }
            else // southwest
            {
                if (southwest == NULL)
                {
                    southwest = new QuadTree(
                        sf::Vector2f(topLeft.x, (topLeft.y + bottomRight.y) / 2),
                        sf::Vector2f((topLeft.x + bottomRight.x) / 2, bottomRight.y), this->capacity);
                }
                southwest->insert(b);
            }
        }
        else // east
        {
            // northeast
            if ((topLeft.y + bottomRight.y) / 2 >= bPosition.y)
            {
                if (northeast == NULL)
                {
                    northeast = new QuadTree(
                        sf::Vector2f((topLeft.x + bottomRight.x) / 2, topLeft.y),
                        sf::Vector2f(bottomRight.x, (topLeft.y + bottomRight.y) / 2),
                        this->capacity);
                }
                northeast->insert(b);
            }

            else // southeast
            {
                if (southeast == NULL)
                {
                    southeast = new QuadTree(
                        sf::Vector2f((topLeft.x + bottomRight.x) / 2, (topLeft.y + bottomRight.y) / 2),
                        sf::Vector2f(bottomRight.x, bottomRight.y),
                        this->capacity);
                }
                southeast->insert(b);
            }
        }
    }
}

// Checks if coordinates are within current quadtree
bool QuadTree::inBounds(sf::Vector2f coords)
{
    return (coords.x >= topLeft.x && coords.x <= bottomRight.x && coords.y >= topLeft.y && coords.y <= bottomRight.y);
}

// Checks if boid influence radius overlaps current quadtree
bool QuadTree::intersects(Boid &b) const
{
    return (b.getPosition().x + b.getRadius() >= topLeft.x && b.getPosition().x - b.getRadius() <= bottomRight.x && b.getPosition().y + b.getRadius() >= topLeft.y && b.getPosition().y - b.getRadius() <= bottomRight.y);
}

std::unordered_set<Boid> QuadTree::query(Boid &b) const
{
    std::unordered_set<Boid> found;
    // Check if boid influence radius is overlapping current quadTree
    if (!intersects(b))
    {
        return found; // boid influence radius not within current quadTree
    }
    int bX = b.getPosition().x;
    int bY = b.getPosition().y;
    int r = b.getRadius();
    sf::Vector2f neighPos;
    for (auto &i : flock)
    {
        neighPos = i.getPosition();
        // check if neighbor within boid's influence radius
        if (neighPos.x <= bX + r &&
            neighPos.x >= bX - r &&
            neighPos.y <= bY + r &&
            neighPos.y >= bY - r)
        {
            found.insert(i);
        }
    }
    if (northwest)
    {
        // search northwest quad
        std::unordered_set<Boid> flockmates(northwest->query(b));
        for (auto &i : flockmates)
        {
            found.insert(i);
        }
    }
    if (northeast)
    {
        // search northeast quad
        std::unordered_set<Boid> flockmates(northeast->query(b));
        for (auto &i : flockmates)
        {
            found.insert(i);
        }
    }
    if (southwest)
    {
        // search southwest quad
        std::unordered_set<Boid> flockmates(southwest->query(b));
        for (auto &i : flockmates)
        {
            found.insert(i);
        }
    }
    if (southeast)
    {
        // search souteast quad
        std::unordered_set<Boid> flockmates(southeast->query(b));
        for (auto &i : flockmates)
        {
            found.insert(i);
        }
    }

    return found;
}

/*
 * Used if not drawing quadTree
 */
void QuadTree::update()
{
    if (northwest != NULL)
    {
        delete (northwest);
    }
    if (northeast != NULL)
    {
        delete (northeast);
    }
    if (southwest != NULL)
    {
        delete (southwest);
    }
    if (southeast != NULL)
    {
        delete (southeast);
    }
}