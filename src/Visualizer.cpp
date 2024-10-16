#include "../Visualizer.hpp"


Visualizer::Visualizer()
{
}

void Visualizer::Init()
{
    if (_LoadSettingsFile())
        ShowWindow(GetConsoleWindow(), SW_HIDE);
    else
        ShowWindow(GetConsoleWindow(), SW_SHOW);

    _DesktopDimensions = _GetDesktopDimensions();
}

bool Visualizer::_LoadSettingsFile()
{
    bool LoadingStatus = true;

    nlohmann::json SettingsFile = {};
    Settings       settings = {};

    std::ifstream  SettingsFileStream("Settings.json");
	SettingsFile = nlohmann::json::parse(SettingsFileStream);


    settings.CustomTablet = SettingsFile["CustomTablet"];

    settings.CursorSize           = SettingsFile["CursorSize"];
    settings.CursorTrailSize      = SettingsFile["CursorTrailSize"];
    settings.CursorTexture        = SettingsFile["CursorImage"];
    settings.CursorTrailTexture   = SettingsFile["CursorTrailImage"];
    settings.CursorTrailDensity   = SettingsFile["CursorTrailDensity"];
    settings.TrailCirclesLifetime = SettingsFile["TrailCirclesLifetime"];

    settings.TabletImageTransparency = SettingsFile["TabletImageTransparency"];

    settings.WindowDimensions = sf::Vector2i(
            SettingsFile["WindowDimensions"].at(0),
            SettingsFile["WindowDimensions"].at(1)
    );

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
    else
    {
        if (!_TabletTexture.loadFromFile("ctl-472-s.png"))
        {
            std::cerr << "Could not load default tablet image!!!\n";
            LoadingStatus = false;
        }
    }

    settings.TabletArea = sf::FloatRect(
        SettingsFile["TabletArea"].at(0),
        SettingsFile["TabletArea"].at(1),
        SettingsFile["TabletArea"].at(2),
        SettingsFile["TabletArea"].at(3)
    );

    if (settings.CursorTexture != "")
        _CursorTexture.loadFromFile(settings.CursorTexture);
    if (settings.CursorTrailTexture != "")
        _CursorTrailTexture.loadFromFile(settings.CursorTrailTexture);

    _CheckForErrors(settings, LoadingStatus);
    _ApplySettings (settings);

    return LoadingStatus;
}

bool Visualizer::_CheckForErrors(Settings &settings, bool& loadingStatus)
{

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

void Visualizer::_ApplySettings(Settings settings)
{
    _Window.create(sf::VideoMode(settings.WindowDimensions.x, settings.WindowDimensions.y), "Tablet visualizer", sf::Style::Close);
    _Window.setFramerateLimit(90);

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

    _Cursor = Cursor(
        settings.CursorSize,
        settings.CursorTrailDensity,
        settings.CursorTrailSize,
        settings.TrailCirclesLifetime,
        _CursorTexture,
        _CursorTrailTexture
    );


    _ScaledSizeOfTablet = {
        float_t(_Window.getSize().x)/_TabletDimensions.x,    
        float_t(_Window.getSize().y)/_TabletDimensions.y
    };

    _TabletPlayfield.left     =  _TabletPlayfield.left * _ScaledSizeOfTablet.x;
    _TabletPlayfield.top      =  _TabletPlayfield.top  * _ScaledSizeOfTablet.y;

    _TabletPlayfield.width    = _TabletPlayfield.width  * _ScaledSizeOfTablet.x;
    _TabletPlayfield.height   = _TabletPlayfield.height * _ScaledSizeOfTablet.y;
}

void Visualizer::_HandleEvents()
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

void Visualizer::Update()
{   
    while (_Window.isOpen())
    {
        _HandleEvents();

        _Cursor.Update(sf::Vector2f(// Get mouse position     //Take x from mouse position and convert it to sfml window coords and make it move only in tablet area   // Add offset so it shows that cursor moves in tablet active area + offset from users's area
            (_Window.mapPixelToCoords(sf::Vector2i(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y)).x * (float(_TabletPlayfield.width)  / float(_DesktopDimensions.x))) + _TabletImageOffset.x + _TabletPlayfield.left,
            (_Window.mapPixelToCoords(sf::Vector2i(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y)).y * (float(_TabletPlayfield.height) / float(_DesktopDimensions.y))) + _TabletImageOffset.y + _TabletPlayfield.top
        ));

        _Render();
    }
}

void Visualizer::_Render()
{
    _Window.clear(sf::Color::Green);
    _Window.draw(_TabletSprite);

    _Window.draw(_Cursor);

    _Window.display();
}

sf::Vector2i Visualizer::_GetDesktopDimensions()
{

    RECT desktop;

	const HWND hDesktop = GetDesktopWindow();

	GetWindowRect(hDesktop, &desktop);

    return sf::Vector2i(desktop.right, desktop.bottom);
}
