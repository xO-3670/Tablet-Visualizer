
# Tablet Visualizer for osu

Tablet Visualizer is a lightweight application designed to function as an overlay for OBS . It simulates a graphic tablet's surface and shows real-time input based on your mouse or tablet stylus movements. You can easily customize the entire program visuals by modifying `Settings.json` file.







[![GPLv3 License](https://img.shields.io/badge/License-GPL%20v3-yellow.svg)](https://choosealicense.com/licenses/gpl-3.0/)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/55a8b6fbec384a7c974e34ee04daf00d)](https://app.codacy.com/gh/xO-3670/Tablet-Visualizer/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![Release](https://img.shields.io/github/v/release/xO-3670/Tablet-Visualizer)](https://github.com/xO-3670/Tablet-Visualizer/releases/tag/1.0)


## Features

- Background of window is green

- Converts mouse/stylus movements into a visual representation on a customizable tablet image.

- You can change the default tablet and cursor image to any image in the `Settings.json` file

- Modify cursor size, skin, and behavior (trail density, lifetime) in `Settings.json`
## Screenshots

![App Screenshot](https://cdn.discordapp.com/attachments/1293699765548351568/1293699779280371804/image.png?ex=6708534f&is=670701cf&hm=0c72e1dae38f6af2aa2bb4d3fc076343c4e6bccb027a5fa131afb4ae10e0e76d&)

![App Screenshot](https://cdn.discordapp.com/attachments/1293699765548351568/1293702268012728435/Screenshot_2024-10-10_00-31-44.png?ex=670855a0&is=67070420&hm=7a3efecf6b1d46d62aab162e0de71c14316dc7bd277eab285fac90f1b149da95&)

# How to edit Settings.json

| Parameter | Type          | Description or examples                                                    |
| :------------------------ | :-------------- | :------------------------------------------------------- |
| `WindowDimensions`        | `uint32_t`      | Size of application's window                             |
| `TabletArea`              | `sf::FloatRect` | first two are offset. 0, 0, 152, 95 is default full area |
| `TabletImageTransparency` | `uint8_t`       | Transparency of tablet's image  0-255                    |
| `CursorSize`              | `float_t`       | Size of cursor                                           |
| `CursorTrailSize`         | `float_t`       | Size of trail's circles                                  |
| `CursorImage`             | `string`        | Path to cursor's image                                   |
| `CursorTrailImage`        | `string`        | Path to cursor trail's image                             |
| `CursorTrailDensity`      | `uint32_t`      | Number of circles in trail                               |
| `TrailCirclesLifetime`    | `float_t`       | How long trail's circles stay on screen                  |
| `CustomTablet`            | `bool`          | Condition if you're using custom tablet image            |
| `CustomTabletImage`       | `string`        | Path to custom tablet image                              |
| `CustomTabletSize`        |`sf::Vector2f`   | Real size of tablet in mm                                |
| `CustomTabletImageOffset` |`sf::Vector2f`   | Offset in custom tablet image to active area             |
| `CustomTabletActiveArea`  |`sf::Vector2f`   | Active area of custom tablet                             |

### If you want to customize it you should set:

- `CustomTablet` to `true`

- `WindowDimensions` to your custom tablet image size

- `CustomTabletImage` to the path of your custom image (you can also place it in the same folder and instead of full path just use name and extension of your custom image)

- `CustomTabletSize` to full size of your custom tablet in mm (you can find this in your tablet's website)

- `CustomTabletActiveArea` to size of tablet active area in mm (you can also find this in your tablet's website)

- `CustomTabletImageOffset` to position in your image where active area start (you can open image in paint and put cursor in left corner of tablet's active area and in left bottom corner of paint window you should be able to see coordinates)

- `TabletArea` to your custom tablet area. First two value are x, y offset and the last ones are width and height of tablet area


## How to build

- First step: Download source
```bash
  git clone --recursive https://github.com/xO-3670/Tablet-Visualizer.git
```

- Second step: Build third party library SFML with static libraries (compile with release configuration)
```bash
  cd "Tablet-Visualizer/third_party/SFML" 
  cmake . -DBUILD_SHARED_LIBS=OFF
```

- Last step: go back to Tablet-Visualizer and build it
```bash
  cd ../../
  mkdir build
  cd build
  cmake ..
```
