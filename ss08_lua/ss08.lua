function values(t)
  -- Iterator
  local i = 0
  return function () i = i + 1; return t[i] end
end

function look_at(my, dir)
  return (my['theta'] > dir - 0.05) and (my['theta'] < dir + 0.05)
end

function decision(enemies,id)
  my = enemies[id]
  m = 1
  d = 0
  l = 5000

  target = nil
  for e in values(enemies) do
    if e['type'] ~= my['type'] then
      target = e
    end
  end

  rad = math.atan2(my['y'] - target['y'], my['x'] - target['x']) + math.pi

  if look_at(my,rad) then
    d = 0
  else
    if my['theta'] < rad then
      d = 1
    else
      d = -1
    end
  end

  return { move = m, direction = d, shoot = 0}
end

