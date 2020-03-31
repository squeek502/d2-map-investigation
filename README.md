d2-map-investigation
====================

A tool for investigating Diablo II map generation in the hopes of finding exploitable coorelations between how things are generated (i.e. if a certain tile is found in X, then that means that Y will generate in a certain way). The ultimate goal is to find a way of knowing ahead-of-time which direction from the waypoint The Summoner will be in the Arcane Sanctuary.

Status:
- This is currently a proof-of-concept just showing that map seeds can be programmatically iterated, and that their resulting levels/areas/tiles/presets/etc can be dumped/analyzed.

Example output:
```
First tile location for the second area of each act:

Seed: 0
 Act 1: Cold Plains: 1080,992
 Act 2: Dry Hills: 1024,992
 Act 3: Great Marsh: 1056,800
 Act 4: Plains of Despair: 1152,1064
 Act 5: Frigid Highlands: 736,1024

Seed: 1
 Act 1: Cold Plains: 1144,1088
 Act 2: Dry Hills: 976,1040
 Act 3: Great Marsh: 992,864
 Act 4: Plains of Despair: 1064,1104
 Act 5: Frigid Highlands: 736,1024
 ```

### Usage

Requirements:
- Visual Studio 2017 (or newer, presumably)
- Diablo II v1.13c or v1.13d

*Note: If you are using Diablo II v1.13d, then you need to define `D2_VERSION_113D`. This can be done by adding `#define D2_VERSION_113D` to the top of `D2Ptrs.h`*

- Open `BH.sln` in Visual Studio and build it
- Open Diablo II and load into a game (doesn't work unless you're actually in a game)
- Run `BH.Injector.exe` from the `Debug/` or `Release/` directory (depending on your build mode)
  + If you run it from a command prompt, you can use `BH.Injector.exe -p -o 2` to toggle injection and exit immediately
  + Otherwise, you use the interactive menu to choose how to inject BH.dll
- A console window will appear and dump the data

*Note: I wasn't able to get BH.Injector to succesfully inject when run through the Visual Studio debugger; it might be possible to get that to work*

### Arcane Sanctuary TODO

- [ ] Figure out how to determine which direction from the waypoint the summoner is from a generated `Act`
- [ ] Dump as much relevant information as possible for X amount of seeds (for levels in Act 1 and Act 2 before Arcane Sanctuary)
- [ ] Find coorelations between Summoner direction and any of the dumped data and rank them
- [ ] Figure out if any of the found coorelations are actually usable in a speedrun

## Acknowledgements

The foundational code is a modified version of [slashdiablo-maphack](https://github.com/underbent/slashdiablo-maphack) (specifically [splippitydo's fork](https://github.com/slippittydo/slashdiablo-maphack)).
