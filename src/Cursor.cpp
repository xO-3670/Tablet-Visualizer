#include "../Cursor.hpp"


TVis::Cursor::Cursor() :
  _CursorTrailTexturePtr(nullptr),
  _TrailCirclesLifetime(0),
  _TrailCirclesSize(0),
  _TrailDensity(0),
  _TrailCirclesFadingEffect(true)
{

}

TVis::Cursor::Cursor(
    float_t      cursorSize,
    uint32_t     trailDensity,
    float_t      sizeOfTrailCircles,
    float_t      trailCirclesLifetime,
    bool         TrailCirclesFadingEffect,
    bool         TrailCirclesSpacingOutFadingEffect,
    sf::Texture& cursorTexture,
    sf::Texture& cursorTrailTexture
) : 
  _TrailDensity(trailDensity),
  _TrailCirclesSize(sizeOfTrailCircles),
  _CursorTrailTexturePtr(&cursorTrailTexture),
  _TrailCirclesLifetime(trailCirclesLifetime),
  _TrailCirclesFadingEffect(TrailCirclesFadingEffect),
  _TrailCirclesSpacingOutEffect(TrailCirclesSpacingOutFadingEffect)
{
    _CursorBody.setRadius   (cursorSize);
    _CursorBody.setOrigin   (_CursorBody.getRadius(), _CursorBody.getRadius());
    _CursorBody.setFillColor(sf::Color(235,235,235, 255));
    _CursorBody.setTexture  (&cursorTexture);

    _TrailCircles.resize(trailDensity, TrailCircle(_CursorBody.getPosition(), _TrailCirclesSize,  _CursorTrailTexturePtr));
}

void TVis::Cursor::Update(sf::Vector2f cursorPosition)
{
    _CursorBody.setPosition(cursorPosition);

    for (int i = 0; i < _TrailCircles.size(); ++i)
    {
        if (_TrailCirclesFadingEffect) // fading effect switch
            _TrailCircles.at(i).SetColorAlpha(255 - static_cast<uint8_t>((_TrailCircles[i].GetLifetime() / _TrailCirclesLifetime) * 255));
        if (_TrailCirclesSpacingOutEffect) // spacing out effect switch
        {
            float calculatedScale = (1000.0 - static_cast<float>((_TrailCircles[i].GetLifetime() / _TrailCirclesLifetime) * 1000.0));
            _TrailCircles.at(i).SetScale({calculatedScale/1000, calculatedScale/1000});
        }

        if (_TrailCircles[i].GetLifetime() >= _TrailCirclesLifetime)
            _TrailCircles.erase(_TrailCircles.begin());
    }
    if (_TrailCircles.size() < _TrailDensity)
        _TrailCircles.push_back(TrailCircle(cursorPosition, _TrailCirclesSize, _CursorTrailTexturePtr));
}

const sf::Vector2f& TVis::Cursor::GetPosition()
{
    return _CursorBody.getPosition();
}

void TVis::Cursor::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (int i = 0; i < _TrailCircles.size(); ++i)
        target.draw(_TrailCircles[i]);
    
    target.draw(_CursorBody);
}