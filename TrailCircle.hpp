#ifndef TRAILCIRCLE_HPP
#define TRAILCIRCLE_HPP

#include <SFML/Graphics.hpp>

#include <math.h>

namespace TVis
{
class TrailCircle : public sf::Drawable
{
public:
    TrailCircle(sf::Vector2f cursorPosition, float& sizeOfTrailCircle, sf::Texture* trailTexture = nullptr);
    ~TrailCircle() = default;

    const sf::Int32     GetLifetime();
    const sf::Vector2f& GetPosition();

    void SetColorAlpha(uint8_t newAlpha);
    void SetScale(sf::Vector2f newScale);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;

private:
    sf::CircleShape _TrailBody;
    sf::Clock       _Lifetime;

};
}

#endif