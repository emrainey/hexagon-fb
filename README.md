# Hexagon

## History

This is copy half of a port of a project I was playing around with in 2003. I wanted to learn more about GLUT and OpenGL in general.

## High Level Notion

At it's core, the program is simply a file system browser with a 3D view.

Visually, the program displays all the files and directories as a set of hexagonal tiles spiralling around a central "down" tile. As each directory is accessed, a new layer of tiles is added above the current layer with the new central tile allowing you to go back down to the layer down while being centered on the lower item in the previous layer. This means the layers of tiles tend to "grow" on a organic manner". At the bottom, the root of the file system would have no central tile. As you ascend the file system into deep leaves, the stack of previous tile layers can get visually clunky so they would be dimmed and/or would vanish into darkness after a few layers.

Logically, the program seeks to use common standard libraries (from the common APIs of the time). While I'm updating it to use OpenGL's shaders and (possibly Vulkan in the future) it should seek to use the standard C++libraries and other open source solutions _where it makes sense_.

## Lower Level Design and Components

### Historical Problems

Previously the system was coded in something preceding C++03. The API was not as well thought out as it could be and while it has an abstraction layer for it's only implementation above Win32, it was still not abstract enough.

### Hierarchy

* `World` - contains everything
  * has the `std::list<View *>`
  * has the pointer to the root `View`
  * has the pointer to the focus `View`
  * has the pointer to the "top" or latest `View`
  * has the `std::list<Light *>`
* `View` - `"no description"` ? I think this is the root of a file system
  * Has a link set to other neighboring `View`s (6) - I think the idea was that you could traverse them as some sort of 6 leafed tree.
  * has the `std::list<Layer *>` to the list of Layers - (TODO: should switch to a `std::stack`)
  * Has a link to the bottom `Layer`
  * Has a link to the `Model`
  * Has a `Vector` position
* `Layer` - contains the list of nodes to display ?
  * has a `std::list<Node*>`
  * has a link to the "selected" `Node*`
  * has a link to the `Model`
  * has a `Vector` position
* `Node`
  * has a 3D `Model`
  * has an `Entity` in the File System (TODO make Entity)
* `Model`
  * holds the `GL` information to render the 3D data
  * has `static`s which can build/render/etc predefined `Shape`s
* `Shape`
  * is actually empty at the moment. (TODO remove?)

* `PersistentStorage` - contains a key-value pair system for storing attributes like model names, etc.
* `Key` - a string name and value (TODO revise - modelled on Windows Registry)
* `Menu` - a menu to see in the `World`
* `MenuOption` - an item in the `Menu`
* `ShellExecutor` - executes shell commands
* `Choreographer` - animates Nodes in various ways
* `ActionRetriever` - retrieves various `Menu`s for a given filepath (TODO `Entity`)

Things to delete, reuse from other projects (see hobbies)

* `Vector` - reuse `geometry::R3::vector`?
* `Color` - reuse `raytrace::Color`?
* `Debug` - move to basal
* `FileSystem` - use `std::filesystem`

### New Heirarchy

* Everything in `namespace hexagon`
* Uses `yaml-cpp`
* Uses `std::filesystem`
* Uses `OpenGL::GL` v4 w/ Shaders
* `World`
  * has at least one element in `std::vector<Root>`
* `Column`
  * has at least one element in `std::vector<Layer>`
* `Layer`
  * has at least one element in `std::vector<Node>`
* `Node` - represents an `Entity` in the file system
  * has one `Model` per ____? (or Shape?)
* `Entity` -
* `Choreographer` - knows how to animate a `Node` via it's predefined `Dances`
* `ActionRetriever` - takes a `Node`'s Filesystem `Entity` and determines what it can do with it, returning `Menu`.

#### OpenGL Specific Items

Simple Types:

```cpp
namespace hexagon {
constexpr size_t dimensions{3u};
union triplet {
    struct {
        float r;
        float g;
        float b;
    } rgb;
    struct {
        float x;
        float y;
        float z;
    } xyz;
};
using point = float[dimensions];
using vector = float[dimensions];
using color = float[dimensions];
```

* `Object`
  * has a 3D position
  * has a 3D orientation
  * has a base scalar

* `Light` is an `Object`
  * has a 3D color

*
