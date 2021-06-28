# NOTICE

This repository is going to be quite out of date for a while. I've made a separate repository with all the online multiplayer code stripped out while I foucs on getting a prototype up and running.
I've also split all the engine code into its own repository, that can now be found [HERE](https://github.com/Katipo007/avokii)

The rest of this readme is currently a bit out of date.


# diorama roguelite
A 3d, 3/4 view, action rogue-lite game I'm making in a dedicated C++ engine I'm writing.
The game is being designed for up to 4 players with a server-client architecture.

There is a bit of separation between game and engine code and I plan on splitting it fully at some point but not for some time.

# License
Haven't decided on a license for this project yet but will probably license the game source separately to the engine. Currently not for public release/use but I'm happy to share access with people I personally know.
The engine source will probably end up releasing under the MIT license.

# State of the Project
A bit sad at the moment, currently the 'game' consists of a chatbox and a character displayed on screen, that is it.
## Immediate goals:
- Get character movement working again and committed.
- A basic NPC enemy which chases the closest player.
- 'Zone' generation and moving between zones which will act as the 'dioramas'/'areas' that you play the game through.

# What I'm using
## Tools
- Visual Studio 2019 with /std:c++latest (for C++20 features)
- [Free texture packer](https://github.com/odrick/free-tex-packer) for generating texture atlases
- [CastleDB](http://castledb.org) for managing static game data
- [RenderDoc](https://renderdoc.org/) has been a great tool while writing my graphics abstraction and debugging graphics problems in general.
- [Pivotal Tracker](https://www.pivotaltracker.com) for task and issue planning/management.
## Libraries
| Library | Licence | Comments |
|-|-|-|
| [SDL2](https://www.libsdl.org/) | [zlib](https://www.libsdl.org/license.php) | Creating graphical windows and accessing keyboard/mouse input. |
| [GLEW](https://github.com/nigels-com/glew) | [[see here]](https://github.com/nigels-com/glew/blob/master/LICENSE.txt) | OpenGL |
| [spdlog v1.x](https://github.com/gabime/spdlog.git) | [MIT](https://github.com/gabime/spdlog/blob/v1.x/LICENSE) | Logging |
| [glm](https://github.com/g-truc/glm.git) | [Happy Bunny / MIT](https://github.com/g-truc/glm/blob/master/copying.txt) | Math types |
| [yojimbo](https://github.com/networkprotocol/yojimbo) | [BSD-3-Clause License](https://github.com/networkprotocol/yojimbo/blob/master/LICENCE) | Client-server networking |
| [Dear ImGui](https://github.com/ocornut/imgui.git) | [MIT](https://github.com/ocornut/imgui/blob/master/LICENSE.txt) | In-game tools and placeholder game UI |
| [stbimage](http://nothings.org/stb/) | [Public Domain / MIT](https://github.com/nothings/stb/blob/master/LICENSE) | Image processing utilities |
| [entt](https://github.com/skypjack/entt) | [MIT](https://github.com/skypjack/entt/blob/master/LICENSE) | Entity component system (ECS) |
| [sigslot](https://github.com/palacaze/sigslot) | [MIT](https://github.com/palacaze/sigslot/blob/master/LICENSE) | Subscribable signals/events |
| [Nlohmann Json](https://github.com/nlohmann/json) | [MIT](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT) | JSON parser |
| [pugixml](https://pugixml.org/) | [MIT](https://github.com/zeux/pugixml/blob/master/LICENSE.md) | XML parser |
| [magic_enum](https://github.com/Neargye/magic_enum) | [MIT](https://github.com/Neargye/magic_enum/blob/master/LICENSE) | A great utility library providing static reflection for enums |
| [frozen](https://github.com/serge-sans-paille/frozen) | [Apache-2.0 License](https://github.com/serge-sans-paille/frozen/blob/master/LICENSE) | Header-only library for 0 initialization cost containers |
| [yaml-cpp](https://github.com/jbeder/yaml-cpp) | [MIT](https://github.com/jbeder/yaml-cpp/blob/master/LICENSE) | Yaml file parsing |
| [Bitsery](https://github.com/fraillt/bitsery) | [MIT](https://github.com/fraillt/bitsery/blob/master/LICENSE) | Header only binary serialization library |
## Art
- "Scroll-o-Sprites" and some other CC-BY-3.0 works by _Sebastiaan van Hijfte_ as placeholders

# Project Architecture
This section still needs fleshing out and nothing is set in stone yet. I'm more than happy to answer any questions.

I've recently been refactoring the engine into a "Core" class which facilitates the "Game" classes, the resource manager and various engine "plugins".
I have different types of plugins (system, input, graphics, etc) which is designed to abstract away and encapsulate specific systems like rendering. There can only be one of each type active at a given time and are designed to be swappable based on the current platform the game application is running on.

## Where do I even start??
`Client/ClientMain.cpp` and `DedicatedServerMain.cpp` act as entry points, initialise the core, plugins and `ClientGame` `ServerGame` respectfully.
From here it should be fairly straight forward to follow any particular class to see how everything works but as a quick run-down:
- Core manages the main loop, storing plugins and the resource manager.
- Plugins are written to meet an API (see headers in `Common/Core/API/*`)
- Resource manager handles loading 'assets' from disk and caching them as 'resources'.
- The game class uses plugins and resources for doing the actual game.

# Running the project:
- Clone the repro and setup the git modules (recursive clone and building the relevant libraries for 64bit)
- Build the project to `bin/Client` and copy `SDL2.dll` here
- Build the texture atlases using _Free Texture Packer_ (.ftpp file is `Assets/2DArt`)
-- Should output to `bin/Client/Art/*`
- Run `DedicatedServer.exe` and `Client.exe` from `bin/Client/`
- (hopefully) Results!
