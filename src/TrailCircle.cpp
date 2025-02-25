#include "../TrailCircle.hpp"

TVis::TrailCircle::TrailCircle(sf::Vector2f cursorPosition, float& sizeOfTrailCircle, sf::Texture* trailTexture)
{
    _Lifetime.restart();

    if (trailTexture != nullptr)
        _TrailBody.setTexture(trailTexture);

    _TrailBody.setRadius(sizeOfTrailCircle);
    _TrailBody.setOrigin(sizeOfTrailCircle, sizeOfTrailCircle);
    _TrailBody.setPosition(cursorPosition);
}

const sf::Int32 TVis::TrailCircle::GetLifetime()
{
    return _Lifetime.getElapsedTime().asMilliseconds();
}

const sf::Vector2f& TVis::TrailCircle::GetPosition()
{
    return _TrailBody.getPosition();
}

void TVis::TrailCircle::SetColorAlpha(uint8_t newAlpha)
{
    _TrailBody.setFillColor(sf::Color(255,255,255,newAlpha));
}

void TVis::TrailCircle::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(_TrailBody);
}
