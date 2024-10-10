
# Tablet Visualizer for osu

Tablet Visualizer is a lightweight application designed to function as an overlay for OBS . It simulates a graphic tablet's surface and shows real-time input based on your mouse or tablet stylus movements. You can easily customize the entire program visuals by modifying  Settings.json file.







[![GPLv3 License](https://img.shields.io/badge/License-GPL%20v3-yellow.svg)](https://choosealicense.com/licenses/gpl-3.0/)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/55a8b6fbec384a7c974e34ee04daf00d)](https://app.codacy.com/gh/xO-3670/Tablet-Visualizer/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
![Release](https://img.shields.io/github/v/release/xO-3670/Tablet-Visualizer)


## Features

- Background of window is green

- Converts mouse/stylus movements into a visual representation on a customizable tablet image surface.

- You can change the default tablet image to any image in the Settings.json file

- Modify cursor size, skin, and behavior (trail density, lifetime) in Settings.json
## Screenshots

![App Screenshot](https://cdn.discordapp.com/attachments/1293699765548351568/1293699779280371804/image.png?ex=6708534f&is=670701cf&hm=0c72e1dae38f6af2aa2bb4d3fc076343c4e6bccb027a5fa131afb4ae10e0e76d&)

![App Screenshot](https://cdn.discordapp.com/attachments/1293699765548351568/1293702268012728435/Screenshot_2024-10-10_00-31-44.png?ex=670855a0&is=67070420&hm=7a3efecf6b1d46d62aab162e0de71c14316dc7bd277eab285fac90f1b149da95&)


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
