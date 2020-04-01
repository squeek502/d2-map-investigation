-- Use room/preset coordinates to further distinguish unique presets
local USE_POSITION_DATA = false

local utils = {}

function utils.splitString(str, sep)
  local parts = {}
  local pos = 0
  local splitIterator = function() return str:find(sep, pos, true) end
  for sepStart, sepEnd in splitIterator do
    table.insert(parts, str:sub(pos, sepStart - 1))
    pos = sepEnd + 1
  end
  table.insert(parts, str:sub(pos))
  return parts
end

function utils.csvToTable(filepath)
  local headers = nil
  local rows = {}
  for line in assert(io.lines(filepath)) do
    line = line:gsub("\r$", "")
    local row = {}
    local fields = utils.splitString(line, ",")
    if not headers then
      headers = fields
    else
      assert(#fields == #headers, "mismatch number of fields between row and header row")
      for i=1,#headers do
        local fieldName = headers[i]
        row[fieldName] = tonumber(fields[i]) or fields[i]
      end
      table.insert(rows, row)
    end
  end
  return rows
end

local function getLookupId(row)
  if USE_POSITION_DATA then
    return row.id .. ':' .. row.x ..','.. row.y .. ':' .. row.x2 .. ',' .. row.y2
  else
    return row.id
  end
end

-- do the stuff

local function go(filesuffix, limit)
  if not filesuffix then filesuffix = "" end
  local arcanes = utils.csvToTable('arcane'..filesuffix..'.csv')
  local arcane_lookup = {}
  local seeds_by_arcane_pos = {TopLeft={}, BottomRight={}, BottomLeft={}, TopRight={}}
  for _, row in ipairs(arcanes) do
    if limit and row.seed >= limit then break end
    arcane_lookup[row.seed] = row.arcane_pos
    table.insert(seeds_by_arcane_pos[row.arcane_pos], row.seed)
  end

  local data_csv = utils.csvToTable('data'..filesuffix..'.csv')
  local data_by_seed = {}
  for _, row in ipairs(data_csv) do
    if limit and row.seed >= limit then break end
    if not data_by_seed[row.seed] then data_by_seed[row.seed] = {} end
    table.insert(data_by_seed[row.seed], row)
  end

  local seed_lookups = {}

  local numseeds = 0
  for seed, rows in pairs(data_by_seed) do
    if not seed_lookups[seed] then seed_lookups[seed] = {} end
    for _, data in ipairs(rows) do
      local lookupId = getLookupId(data)
      seed_lookups[seed][lookupId] = true
    end
    numseeds = numseeds + 1
  end

  local posLookup = {}
  for pos, seeds in pairs(seeds_by_arcane_pos) do
    posLookup[pos] = {}
    for _, seed in ipairs(seeds) do
      for id in pairs(seed_lookups[seed]) do
        posLookup[pos][id] = true
      end
    end
  end

  local overlapsByPos = {}
  local overlapsByPosLookup = {}
  for pos, seeds in pairs(seeds_by_arcane_pos) do
    local overlappingIds = {}
    local seen = {}
    local firstSeed = seeds[1]
    local seedData = data_by_seed[firstSeed]
    for _, row in ipairs(seedData) do
      local lookupId = getLookupId(row)
      local overlaps = true
      for _, seed in ipairs(seeds) do
        if not seed_lookups[seed][lookupId] then
          overlaps = false
          break
        end
      end
      if overlaps and not seen[lookupId] then
        table.insert(overlappingIds, lookupId)
        seen[lookupId] = true
      end
    end
    overlapsByPos[pos] = overlappingIds
    overlapsByPosLookup[pos] = {}
    for _, id in ipairs(overlappingIds) do
      overlapsByPosLookup[pos][id] = true
    end
  end

  local nonOverlappingOverlaps = {}
  local seen = {}
  for pos, ids in pairs(overlapsByPos) do

    --[[
    local f = io.open(pos..filesuffix..'.txt', 'w')
    f:write(table.concat(ids, '\n'))
    f:close()
    ]]

    for _, id in ipairs(ids) do
      local overlaps = false
      for _pos in pairs(overlapsByPos) do
        if _pos ~= pos and posLookup[_pos][id] then
          overlaps = true
          break
        end
      end
      if not overlaps and not seen[id] then
        table.insert(nonOverlappingOverlaps, id)
        seen[id] = true
      end
    end
  end

  if #nonOverlappingOverlaps > 0 then
    for _, id in ipairs(nonOverlappingOverlaps) do
      print(id)
    end
  else
    print("no correlations found")
  end
end

go()
