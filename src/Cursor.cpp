
#include "../Cursor.hpp"


Cursor::Cursor(float_t cursorSize, uint32_t trailDensity, float_t sizeOfTrailCircles, float_t trailCirclesLifetime ,sf::Texture& cursorTexture, sf::Texture& cursorTrailTexture)
: _TrailDensity(trailDensity),
  _TrailCirclesSize(sizeOfTrailCircles),
  _CursorTrailTexturePtr(&cursorTrailTexture)
{
    _CursorBody.setRadius   (cursorSize);
    _CursorBody.setOrigin   (_CursorBody.getRadius(), _CursorBody.getRadius());
    _CursorBody.setFillColor(sf::Color(235,235,235, 255));
    _CursorBody.setTexture  (&cursorTexture);

    _TrailCircles.resize(trailDensity, TrailCircle(_CursorBody.getPosition(), _TrailCirclesSize,  _CursorTrailTexturePtr));

void Cursor::Update(sf::Vector2f cursorPosition)
{
    _CursorBody.setPosition(cursorPosition);

    for (int i = 0; i < _TrailCircles.size(); ++i)
    {
        if (_TrailCircles[i].GetLifetime() >= _TrailCirclesLifetime)
        {
            _TrailCircles.erase(_TrailCircles.begin());
        }
    }
    if (_TrailCircles.size() < _TrailDensity)
        _TrailCircles.push_back(TrailCircle(cursorPosition, _TrailCirclesSize, _CursorTrailTexturePtr));
}

const sf::Vector2f& Cursor::GetPosition()
{
    return _CursorBody.getPosition();
}

void Cursor::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (int i = 0; i < _TrailCircles.size(); ++i)
    {
        target.draw(_TrailCircles[i]);
    }
    target.draw(_CursorBody);
}
