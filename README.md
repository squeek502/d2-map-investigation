d2-map-investigation
====================

A tool for investigating Diablo II map generation in the hopes of finding exploitable correlations between how things are generated (i.e. if a certain tile is found in X, then that means that Y will generate in a certain way). The ultimate goal is to find a way of knowing ahead-of-time which direction from the waypoint The Summoner will be in the Arcane Sanctuary.

Status:
- This started as a proof-of-concept just showing that map seeds can be programmatically iterated, and that their resulting levels/areas/tiles/presets/etc can be dumped/analyzed.
- The relevant code is in [BH/MapInvestigation.cpp](BH/MapInvestigation.cpp). See [slashdiablo-maphack](https://github.com/slippittydo/slashdiablo-maphack) for reference on how the map data can be further dug into.

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
- `.csv` files will be created next to `BH.dll` (see Output below)

*Note: I wasn't able to get BH.Injector to successfully inject when run through the Visual Studio debugger; it might be possible to get that to work*

### Output

All data is dumped to .csv files. Each of them have the seed as the first column.

#### `arcane.csv`

| Column | Description |
| --- | --- |
| summoner_dir | The correct path from the waypoint to the summoner (e.g. TopRight) |
| portals_dir | The path from the waypoint that has portals along it (e.g. TopLeft) |

#### `areas.positional.relationships.csv`

| Column | Description |
| --- | --- |
| from_level | The name of the level used as the 'from' level |
| to_level | The name of the level used as the 'to' level |
| direction | The direction from the 'from' level to the 'to' level (e.g. BottomLeft) |

*Note: This is only useful for levels that connect without a 'warp' (i.e. Blood Moor to Cold Plains, but __not__ Blood Moor to Den of Evil), and that happen to be in order in the internal level IDs. This direction does not take into account where actual exits are, its just comparing the starting coordinates of ID-sequential levels. Outputting the actual relationships between __all__ actually connected areas using their actual exits would be a useful improvement to this data.*

#### `areas.rooms.csv`

| Column | Description |
| --- | --- |
| level_name | Name of the level that this room is part of |
| room_flags | dwRoomFlags field of the room (what this affects is unknown) |
| room_number | Corresponds to the `Def` column of LevelPrest.txt (`0` means 'None') |
| sub_number | Corresponds to the `Def` column of LevelPrest.txt (`0` means 'None'). Always the same as room_number? |
| preset_type | dwPresetType field of the room (what this affects is unknown) |
| is_preset | Whether or not preset_txt_number is valid (1 if valid, 0 if invalid) |
| preset_txt_number | dwTxtFileNo of pPreset of the room (what this refers to is unknown) |
| level_relative_room_coords_x | X coordinate of the room relative to the starting room of its level |
| level_relative_room_coords_y | Y coordinate of the room relative to the starting room of its level |
| absolute_room_coords_x | X coordinate of the room in absolute coordinates |
| absolute_room_coords_y | Y coordinate of the room in absolute coordinates |

*Note: 'Room' does refer to a colloquial room, it's just the term used for the large 'building blocks' of levels in D2*

#### `areas.rooms.presets.csv`

| Column | Description |
| --- | --- |
| level_name | Name of the level that this preset is contained in |
| id | A distinguishing string for this preset (Type + ID + Name) |
| level_relative_room_coords_x | X coordinate of the containing room relative to the starting room of its level |
| level_relative_room_coords_y | Y coordinate of the containing room relative to the starting room of its level |
| absolute_room_coords_x | X coordinate of the containing room in absolute coordinates |
| absolute_room_coords_y | Y coordinate of the containing room in absolute coordinates |
| room_relative_preset_coords_x | X coordinate of the preset relative to its room in 'tile units' (tile units = room units * 5) |
| room_relative_preset_coords_y | Y coordinate of the preset relative to its room in 'tile units' (tile units = room units * 5) |
| absolute_preset_coords_x | X coordinate of the preset in absolute 'tile units' (tile units = room units * 5) |
| absolute_preset_coords_y | Y coordinate of the preset in absolute 'tile units' (tile units = room units * 5) |

*Note: 'tile units' and 'room units' are probably the wrong name for these, but its to distinguish between the scale of coordinates used for rooms and the scale of coordinates used within a room*

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
