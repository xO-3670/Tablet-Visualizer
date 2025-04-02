#include "../Visualizer.hpp"
#include "Visualizer.hpp"


TVis::Visualizer::Visualizer() :
  _IsTabletCustom(false),
  _DesktopDimensions({0,0})
{
}

void TVis::Visualizer::Init()
{
#if WindowsOS == 1
    HWND consoleWindowW10 = GetConsoleWindow();
    Sleep(2); // There needs to be delay because console widnow might not initialize in time and console will still appear in windows 11
    HWND consoleWindowW11 = GetWindow(consoleWindowW10, GW_OWNER);

    if (_LoadSettingsFile())
    {
        _HideConsole(consoleWindowW10, consoleWindowW11);
    }
    else
        ShowWindow(GetConsoleWindow(), SW_SHOW);

    if (_EnableErrors == false) // Manually disable errors console in Settings.json
    {
        _HideConsole(consoleWindowW10, consoleWindowW11);
    }

    _DesktopDimensions = _GetDesktopDimensions();

#endif
#if WindowsOS == 0

    if (!_LoadSettingsFile())
        std::cout << "Error while loading settings file!!!\n";

    _DesktopDimensions = _GetDesktopDimensionsLinux();

#endif
}

bool TVis::Visualizer::_LoadSettingsFile()
{
    bool LoadingStatus = true;

    nlohmann::json SettingsFile = {};
    Settings       settings = {};

    std::ifstream  SettingsFileStream("Settings.json");
	SettingsFile = nlohmann::json::parse(SettingsFileStream);

    // bool values
    settings.CustomTablet                 = SettingsFile["CustomTablet"];
    settings.EnableErrors                 = SettingsFile["EnableErrors"];
    settings.TrailCirclesFadingEffect     = SettingsFile["TrailCirclesFadingEffect"];
    settings.TrailCirclesSpacingOutEffect = SettingsFile["TrailCirclesSpacingOutEffect"];

    // float values                   // multiply times 10 so 1.00 will be converted to program's size standard
    settings.CursorSize               = static_cast<float_t>(SettingsFile["CursorSize"])      * 10;
    settings.CursorTrailSize          = static_cast<float_t>(SettingsFile["CursorTrailSize"]) * 10;
    settings.TrailCirclesLifetime     = SettingsFile["TrailCirclesLifetime"];

    // uint32_t
    settings.CursorTrailDensity           = SettingsFile["CursorTrailDensity"];
    settings.FramerateLimit               = SettingsFile["FramerateLimit"];

    // uint8_t values
    settings.TabletImageTransparency      = SettingsFile["TabletImageTransparency"];

    // string path
    settings.CursorTexture                = SettingsFile["CursorImage"];
    settings.CursorTrailTexture           = SettingsFile["CursorTrailImage"];

    settings.WindowDimensions = sf::Vector2i(
        SettingsFile["WindowDimensions"].at(0),
        SettingsFile["WindowDimensions"].at(1)
    );

    settings.TabletArea = sf::FloatRect(
        SettingsFile["TabletArea"].at(0),
        SettingsFile["TabletArea"].at(1),
        SettingsFile["TabletArea"].at(2),
        SettingsFile["TabletArea"].at(3)
    );

    // if user wants to use their own tablet image and data this is loaded
    if (settings.CustomTablet)
    {
        settings.CustomTabletSize = sf::Vector2f(
            SettingsFile["CustomTabletSize"].at(0),
            SettingsFile["CustomTabletSize"].at(1)
        );

        settings.CustomTabletActiveArea = sf::Vector2f(
            SettingsFile["CustomTabletActiveArea"].at(0),
            SettingsFile["CustomTabletActiveArea"].at(1)
        );

        settings.CustomTabletImageOffset = sf::Vector2f(
            SettingsFile["CustomTabletImageOffset"].at(0),
            SettingsFile["CustomTabletImageOffset"].at(1)
        );

        settings.CustomTabletImage = SettingsFile["CustomTabletImage"];
    }
    else // if user don't use custom tablet, default image is loaded
    {
        if (!_TabletTexture.loadFromFile("ctl-472-s.png"))
        {
            std::cerr << "Could not load default tablet image!!!\n";
            LoadingStatus = false;
        }
    }

    if (settings.CursorTexture != "")
        _CursorTexture.loadFromFile(settings.CursorTexture);
    if (settings.CursorTrailTexture != "")
        _CursorTrailTexture.loadFromFile(settings.CursorTrailTexture);

    _CheckForErrors(settings, LoadingStatus);
    _ApplySettings (settings);

    return LoadingStatus;
}

bool TVis::Visualizer::_CheckForErrors(Settings &settings, bool& loadingStatus)
{

    if (settings.FramerateLimit <= 0)
    {
        settings.FramerateLimit = 30;
        std::cerr << "Framerate limit is 0 or lower!!!\nAutomatically setting to 30\n";
        loadingStatus = false;
    }

    if (settings.CursorSize == 0)
    {
        std::cerr << "Incorrect cursor size!!!\n";
        loadingStatus = false;
    }

    if (settings.CursorTexture == "")
    {
        std::cerr << "Incorrect cursor image path!!!\n";
        loadingStatus = false;
    }

    if (settings.WindowDimensions.x == 0 || settings.WindowDimensions.y == 0)
    {
        std::cerr << "Incorrect window dimensions!!!\n";
        loadingStatus = false;
    }

    if (settings.TabletArea.width == 0 || settings.TabletArea.height == 0)
    {
        std::cerr << "Incorrect tablet dimensions!!!\n";
        loadingStatus = false;
    }

    if (settings.CustomTablet)
    {
        if (settings.CustomTabletSize.x == 0 || settings.CustomTabletSize.y == 0)
        {
            std::cerr << "Incorrect custom tablet dimensions!!!\n";
            loadingStatus = false;
        }

        if (settings.CustomTabletActiveArea.x == 0 || settings.CustomTabletActiveArea.y == 0)
        {
            std::cerr << "Incorrect custom tablet active area!!!\n";
            loadingStatus = false;
        }

        if (!_TabletTexture.loadFromFile(settings.CustomTabletImage))
        {
            std::cerr << "Could not load custom tablet image!!!\n";
            loadingStatus = false;
        }

        if (settings.CustomTabletImageOffset.x == 0 || settings.CustomTabletImageOffset.y == 0)
        {
            std::cerr << "Custom tablet image offset (x or y) is 0!!!\n";
            loadingStatus = false;
        }
    }
    return loadingStatus;
}

void TVis::Visualizer::_ApplySettings(Settings settings) // this function exist so Settings struct is destroyed after being used for storing data
{
    _Window.create(sf::VideoMode(settings.WindowDimensions.x, settings.WindowDimensions.y), "Tablet visualizer", sf::Style::Close);
    _Window.setFramerateLimit(settings.FramerateLimit);

    _EnableErrors    = settings.EnableErrors;
    _IsTabletCustom  = settings.CustomTablet;
    _TabletPlayfield = settings.TabletArea;
    
    if (!_IsTabletCustom) // values for default settings
    {
        _TabletImageOffset = sf::Vector2f(96,   72); // offset from default image in pixels
        _TabletDimensions  = sf::Vector2f(210, 146); // 210 and 146 is wacom 472's size in mm
        _TabletActiveArea  = sf::Vector2f(152,  95); // 152 and  95 is wacom 472's active/work area also in mm
    }
    else
    {
        _TabletImageOffset = settings.CustomTabletImageOffset;
        _TabletDimensions  = settings.CustomTabletSize;
        _TabletActiveArea  = settings.CustomTabletActiveArea;
    }

    _TabletSprite.setTexture(_TabletTexture);
    _TabletSprite.setColor(
        sf::Color(
            _TabletSprite.getColor().r, _TabletSprite.getColor().g, _TabletSprite.getColor().b,
            settings.TabletImageTransparency
        )
    );

    Cursor cursor(
        settings.CursorSize,
        settings.CursorTrailDensity,
        settings.CursorTrailSize,
        settings.TrailCirclesLifetime,
        settings.TrailCirclesFadingEffect,
        settings.TrailCirclesSpacingOutEffect,
        _CursorTexture, 
        _CursorTrailTexture
    );
    _Cursor = cursor;


    _ScaledSizeOfTablet = {
        float_t(_Window.getSize().x)/_TabletDimensions.x,    
        float_t(_Window.getSize().y)/_TabletDimensions.y
    };

    _TabletPlayfield.left     =  _TabletPlayfield.left * _ScaledSizeOfTablet.x;
    _TabletPlayfield.top      =  _TabletPlayfield.top  * _ScaledSizeOfTablet.y;

    _TabletPlayfield.width    = _TabletPlayfield.width  * _ScaledSizeOfTablet.x;
    _TabletPlayfield.height   = _TabletPlayfield.height * _ScaledSizeOfTablet.y;
}

void TVis::Visualizer::_HandleEvents()
{
    if (_Window.pollEvent(_WindowEvent))
        switch (_WindowEvent.type)
        {
            case sf::Event::Closed:
            {
                _Window.close();
                break;
            }
            default:
                break;
        }
}

void TVis::Visualizer::Update()
{   
    while (_Window.isOpen())
    {
        _HandleEvents();

        _Cursor.Update(sf::Vector2f(// Get mouse position // Take x from mouse position and convert it to sfml window coords // make it move only in tablet area // Add offset so it shows that cursor moves in tablet active area + offset from users's area
            (_Window.mapPixelToCoords(sf::Vector2i(sf::Mouse::getPosition().x, 0)).x * (_TabletPlayfield.width  / static_cast<float>(_DesktopDimensions.x))) + _TabletImageOffset.x + _TabletPlayfield.left,
            (_Window.mapPixelToCoords(sf::Vector2i(0, sf::Mouse::getPosition().y)).y * (_TabletPlayfield.height / static_cast<float>(_DesktopDimensions.y))) + _TabletImageOffset.y + _TabletPlayfield.top
        ));

        _Render();
    }
}

void TVis::Visualizer::_Render()
{
    _Window.clear(sf::Color::Green);
    
    _Window.draw(_TabletSprite);
    _Window.draw(_Cursor);

    _Window.display();
}

#if WindowsOS == 0
sf::Vector2i TVis::Visualizer::_GetDesktopDimensionsLinux()
{
    Display* display = XOpenDisplay(NULL);
    Screen*  screen  = DefaultScreenOfDisplay(display);

    return sf::Vector2i(screen->width, screen->height);
}
#endif

#if WindowsOS == 1
sf::Vector2i TVis::Visualizer::_GetDesktopDimensions()
{
    RECT desktop;

	const HWND hDesktop = GetDesktopWindow();

	GetWindowRect(hDesktop, &desktop);

    return sf::Vector2i(desktop.right, desktop.bottom);
}

void TVis::Visualizer::_HideConsole(HWND& W10ConsoleHandle, HWND& W11ConsoleHandle)
{
    if (W11ConsoleHandle == NULL)
        ShowWindow(GetConsoleWindow(), SW_HIDE);
    else
        ShowWindow(W11ConsoleHandle, SW_HIDE);
}

void TVis::Visualizer::_ShowConsole(HWND &W10ConsoleHandle, HWND &W11ConsoleHandle)
{
    if (W11ConsoleHandle == NULL)
        ShowWindow(GetConsoleWindow(), SW_SHOW);
    else
        ShowWindow(W11ConsoleHandle, SW_SHOW);
}
#endif
