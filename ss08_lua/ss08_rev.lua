function values(t)
  -- Iterator
  local i = 0
  return function () i = i + 1; return t[i] end
end

function look_at(my, dir)
  return (my['theta'] > dir - 0.05) and (my['theta'] < dir + 0.05)
end

stat = 0

function decision(enemies,id)
  my = enemies[id]
  m = 0
  d = 0

  if stat == 0 then
    if look_at(my, 1.57) then
      stat = 1
    else
      d = 1
    end
  end

  if stat == 1 then
    m = 1
    if my['y'] > 200 then
      stat = 2
    end
  end

  if stat == 2 then
    d = -1
    if ((my['theta'] > 6.2) or (my['theta'] < 0.1)) then
      stat = 3
    end
  end

  if stat == 3 then
    m = 1
    if my['x'] > 290 then
      stat = 4
    end
  end

  if stat == 4 then
    if look_at(my, 4.71) then
      stat = 5
    else
      d = -1
    end
  end

  if stat == 5 then
    m = 1
    if my['y'] < -200 then
      stat = 6
    end
  end

  if stat == 6 then
    if look_at(my, 3.14) then
      stat = 7
    else
      d = -1
    end
  end

  if stat == 7 then
    m = 1
    if my['x'] < -290 then
      stat = 0
    end
  end


  return { move = m, direction = d, shoot = 0}
end

