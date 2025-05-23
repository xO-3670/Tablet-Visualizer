#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>

#if defined(_WIN32) || defined(WIN32)

#include <Windows.h>

#define WindowsOS 1
#endif

#ifdef __unix__

#include <unistd.h>
#include <X11/Xlib.h>

#define WindowsOS 0
#endif

#include "Cursor.hpp"

namespace TVis
{
struct Settings
{
    sf::Vector2i  WindowDimensions;
    sf::FloatRect TabletArea;

    float_t       CursorSize;
    float_t       CursorTrailSize;
    float_t       TrailCirclesLifetime;
    uint32_t      CursorTrailDensity;
    uint32_t      FramerateLimit;
    std::string   CursorTexture;
    std::string   CursorTrailTexture;

    uint8_t       TabletImageTransparency;

    bool          CustomTablet;
    bool          EnableErrors;
    bool          TrailCirclesFadingEffect;
    bool          TrailCirclesSpacingOutEffect;

    sf::Vector2f  CustomTabletActiveArea;
    sf::Vector2f  CustomTabletSize;
    sf::Vector2f  CustomTabletImageOffset;
    std::string   CustomTabletImage;
};

class Visualizer
{
public:
    Visualizer();
    ~Visualizer() = default;

    void Init();
    void Update();

private:
    bool _LoadSettingsFile();
    bool _CheckForErrors(Settings& settings, bool& loadingStatus);

    void _ApplySettings(Settings settings);
    void _HandleEvents();
    void _Render();

#if WindowsOS == 0
    sf::Vector2i _GetDesktopDimensionsLinux();
#endif
#if WindowsOS == 1
    sf::Vector2i _GetDesktopDimensions();
    // Fix for console showing up in windows 11 even when disabled by ShowWindow(GetConsoleWindow(), SW_HIDE)
    // Takes two handles of console one when on windows 10 and second when on 11
    void         _HideConsole(HWND& W10ConsoleHandle, HWND& W11ConsoleHandle);
    void         _ShowConsole(HWND& W10ConsoleHandle, HWND& W11ConsoleHandle);
#endif

private:
    sf::RenderWindow _Window;
    sf::Event        _WindowEvent;

    sf::Vector2i     _DesktopDimensions;
    sf::FloatRect    _TabletPlayfield;

    sf::Vector2f     _TabletImageOffset;
    sf::Vector2f     _ScaledSizeOfTablet;
    sf::Vector2f     _TabletDimensions;
    sf::Vector2f     _TabletActiveArea;

    sf::Texture      _TabletTexture;
    sf::Texture      _CursorTexture;
    sf::Texture      _CursorTrailTexture;

    sf::Sprite       _TabletSprite;

    Cursor           _Cursor;

    bool             _IsTabletCustom;
    bool             _EnableErrors;
};
}

#endif