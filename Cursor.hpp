#ifndef CURSOR_HPP
#define CURSOR_HPP

#include <SFML/Graphics.hpp>

#include <vector>
#include <algorithm>

#include "TrailCircle.hpp"

namespace TVis 
{
class Cursor : public sf::Drawable
{
public:
    Cursor();
    Cursor(
        float_t      cursorSize,
        uint32_t     trailDensity,
        float_t      sizeOfTrailCircles,
        float_t      trailCirclesLifetime,
        bool         trailCirclesFadingEffect,
        sf::Texture& cursorTexture,
        sf::Texture& cursorTrailTexture
    );
    ~Cursor() = default;

    void Update(sf::Vector2f cursorPosition);

    const sf::Vector2f& GetPosition();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;

private:
    sf::CircleShape _CursorBody;

    std::vector<TrailCircle> _TrailCircles;
    sf::Texture*             _CursorTrailTexturePtr;

    uint16_t _TrailDensity;
    float_t  _TrailCirclesSize;
    float_t  _TrailCirclesLifetime;

    bool     _TrailCirclesFadingEffect;

};
}

#endif