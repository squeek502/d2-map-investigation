d2-map-investigation
====================

A tool for investigating Diablo II map generation in the hopes of finding exploitable correlations between how things are generated (i.e. if a certain tile is found in X, then that means that Y will generate in a certain way). The ultimate goal is to find a way of knowing ahead-of-time which direction from the waypoint The Summoner will be in the Arcane Sanctuary.

Status:
- This is currently a proof-of-concept just showing that map seeds can be programmatically iterated, and that their resulting levels/areas/tiles/presets/etc can be dumped/analyzed.
- The relevant code is in [BH/MapInvestigation.cpp](BH/MapInvestigation.cpp). See [slashdiablo-maphack](https://github.com/slippittydo/slashdiablo-maphack) for reference on how the map data can be further dug into.

Example output:
```
Seed: 0         Summoner: Top Right
Seed: 1         Summoner: Top Left
Seed: 2         Summoner: Bottom Right
Seed: 3         Summoner: Bottom Left
Seed: 4         Summoner: Bottom Right
 ```
(the rest of the data is dumped to .csv files)

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
- A console window will appear and start dumping the data
  + Once it's done, you can uninject
- `.csv` files will be created next to `BH.dll`
  + `arcane.csv` contains seed + the position of the arcane sanctuary
  + `data.csv` contains a dump of all other potentially-relevant data for each seed

*Note: I wasn't able to get BH.Injector to successfully inject when run through the Visual Studio debugger; it might be possible to get that to work*

### Finding Correlations / Progress

First attempt was to dump all presets (objects, monsters, tiles that always spawn in a given room) and look for any that fulfill all 3 of these conditions:

- Exist in all same-position arcane seeds
- Do not exist in any seeds with different arcane positions
- Are in Act 1 or 2

This ended up with no matching presets outside of things like The Summoner himself, Horazon's Journal, etc.

Running `lua ../find_correlations.lua` from with the `Debug/` or `Release/` directory (whichever you are using) will parse `data.csv`/`arcane.csv` and run the check described above.

### Arcane Sanctuary TODO

- [x] Figure out how to determine which direction from the waypoint the summoner is from a generated `Act`
- [ ] Dump as much relevant information as possible for X amount of seeds (for levels in Act 1 and Act 2 before Arcane Sanctuary)
- [ ] Find correlations between Summoner direction and any of the dumped data and rank them
- [ ] Figure out if any of the found correlations are actually usable in a speedrun

## Acknowledgements

The foundational code is a modified version of [slashdiablo-maphack](https://github.com/underbent/slashdiablo-maphack) (specifically [splippitydo's fork](https://github.com/slippittydo/slashdiablo-maphack)).
