#ifndef TRAILCIRCLE_HPP
#define TRAILCIRCLE_HPP

#include <SFML\Graphics.hpp>

#include <math.h>

class TrailCircle : public sf::Drawable
{
public:
    TrailCircle(sf::Vector2f cursorPosition, float& sizeOfTrailCircle, sf::Texture* trailTexture = nullptr);
    ~TrailCircle() = default;

    sf::Int32 GetLifetime();
    sf::Vector2f GetPosition();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;

private:
    sf::CircleShape _TrailBody;
    sf::Clock       _Lifetime;

};

#endif