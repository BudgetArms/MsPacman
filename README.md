# Ms Pacman

Creating a MsPacman (1982) clone, which is a sequel to Pacman (1980). <br>

The MsPacman uses [BudgetArmsEngine]("https://github.com/BudgetArms/BudgetArmsEngine") as game engine. <br>
It's expanded from the [Minigin]("https://github.com/avadae/minigin/") template. <br>

The design patterns I used, will be mentioned in the game engine repository as well. <br>
But for here's a short list of patterns that are used.

## Design patterns:

* Dirty flags
* Observers & subjects
* Event Queues (uses enum class that has to be modified in game engine, bad design)
* Sound Queues (runs on a different thread)
* Component system (each game object can have 1 of same type component)

## Todo's:

* Add Hitbox component with debug draw toggles to easily issues
* Add MsPacman texture and sound resources
* Add level formats, to create/load levels
* Implement simple spritesheet workflow, that doesn't require extra children to make spritesheets work properly
* Implement MoveOnGrid Logic

# Build Options

## Emscripten (web) version

### On Windows

The required tools

    Emscripten
    CMake
    Ninja
    Python

Create a directory `build_web` in the root folder of your project. <br>
Open the terminal in the `build_web` folder, and run:

    emcmake cmake ..
    emmake ninja

To be able to see the webpage you can start a python webserver in the build_web folder

    python -m http.server

Then browse to http://localhost:8000 and you're good to go.
