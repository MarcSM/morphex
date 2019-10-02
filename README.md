# Morphex
**A Spectral Morphing Synthesizer**

[![GitHub release](https://img.shields.io/github/release/MarcSM/morphex.svg)](https://github.com/MarcSM/morphex/releases) [![Github All Releases](https://img.shields.io/github/downloads/MarcSM/morphex/total.svg)]() [![License: AGPL v3](https://img.shields.io/badge/License-AGPL%20v3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)

![](https://github.com/MarcSM/SpectralMorphingTool/blob/master/Assets/Images/morphex_v02.png?raw=true)

<img align="right" width="100" height="100" src="https://github.com/MarcSM/SpectralMorphingTool/blob/master/Assets/Images/Logos/morphex_logo.png?raw=true">

Morphex is a an open source audio plugin that morphs the harmonics of two sounds while exploring the latent timbral space that emerges from the spectral analysis of musical sounds, and to craft new hybrid instruments. 

## Demo Video

If you want to see the plugin live you can take a look at this video:

www.youtube.com/watch?v=4NT5JkBR96U

In this video you can hear some presets of the plugin. External plugin effects such as distortion and reverb are used at the end of the video to see what kind of sounds can we get when using the plugin within a DAW in a music production use case.

## Research Repository

www.github.com/MarcSM/morphex-research

## System requirements

Supported OS:

- macOS 10.13+

Morphex has been tested on macOS with the following hosts:

- Ableton Live
- Logic Pro X
- Reaper
- Hosting AU

Right now we have 3 different builds of the plugin:

- A standalone app for macOS (you can open it directly)
- An Audio Unit plugin “AU”
- And a “VST3” plugin

## How to compile Morphex

### macOS

1. Clone this repository

2. Download Juce (http://www.juce.com/) - This project is currently made using JUCE v5.4.3

3. Open "Morphex.jucer" using the Projucer app

4. On the menu bar go to "Projucer -> Global Paths" and set "Path to JUCE" and "JUCE Modules".

5. Click on "File -> Save Project and open in IDE" to load the project in XCode. 

6. Once the project is loaded hit de build button.

7. Build done!

## License

All the software is distributed with the Affero GPL license (http://www.gnu.org/licenses/agpl-3.0.en.html). The sounds located at "data/sounds" come from freesound.org, in particular from http://www.freesound.org/people/xserra/packs/13038/.
