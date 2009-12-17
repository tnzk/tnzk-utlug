function values(t)
  -- Iterator
  local i = 0
  return function () i = i + 1; return t[i] end
end

stat = 0

function decision(enemies,id)
  -- Describe how to decide your action.
  -- Information of enemies who close to you will
  -- be passd as the table named enemies
  --
  -- Enemies is a table holding hashes that have
  -- information of each enemy. The hash in enemies
  -- have a structure:
  -- {   id: An unique integer to identify an enemy.
  --   type: An unique integer to identify an AI.
  --         If enemies have same type, they must
  --         have same AI.
  --    x,y: Integers describing an enemy's position
  --  theta: Integers describing an enemy's direction
  -- }
  --
  -- After processing, return decided your action.
  -- Your returning value must be formed like following:
  -- { move: An integer meaning you forward or backward or stop.
  --           1: Forward
  --           0: Stop
  --          -1: Backwar
  --  direction: An integer meaning you turn right or left or keep.
  --           1: Turn left
  --           0: Keep
  --          -1: Turn right
  --  shoot: An integer meaning if you shoot or not.
  --           0: shoot
  --           1: do not shoot
  -- }
  my = enemies[id]
  m = 0
  d = 0
  lock = 0

  if lock == 0 and stat == 0 then
    if ((my['theta'] > 1.54) and (my['theta'] < 1.59)) then
      stat = 1
    else
      d = 1
    end
  end

  if lock == 0 and stat == 1 then
    m = 1
    if my['y'] > 200 then
      stat = 2
    end
  end

  if lock == 0 and stat == 2 then
    d = -1
    if ((my['theta'] > 6.2) or (my['theta'] < 0.1)) then
      stat = 3
    end
  end

  if lock == 0 and stat == 3 then
    m = 1
    if my['x'] > 290 then
      stat = 4
    end
  end

  if lock == 0 and stat == 4 then
    d = -1
  end

  return { move = m, direction = d, shoot = 0}
end

