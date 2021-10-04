
-- This will build the equivalent of the sql file

local location = newComponent("location",
  numeric("x"),
  numeric("y"),
  numeric("prev_x"),
  numeric("prev_y"),
  numeric("theta")
)

local mobile = newComponent("mobile",
  numeric("accel"),
  numeric("vel"),
  numeric("rotation")
)

local accelerate = updateSystem{
  target = mobile,
  given = mobile:as("old"),
  require = "old.accel != 0.0",
  set = {
    "vel = min(old.max_vel, max(0.0, old.vel + old.accel))"
  }
}

local move = updateSystem{
  target = location,
  given = entitiesWithAll(location:as("old"), mobile),
  require = "old.vel > 0.0",
  set = {
    "prev_x = old.x",
    "prev_y = old.y",
    "theta = old.theta + mobile.rotation",
    "x = old.x - sin(old.theta) * mobile.vel",
    "y = old.y + cos(old.theta) * mobile.vel"
  }
}

local movement = systemGroup(
  accelerate, move
)

newComponent("collision",
  numeric("radius")
)

newComponent("asteroid",
  int("size")
)

newComponent("explosion",
  int("size")
)

newComponent("bullet",
  numeric("age"),
  int("damage")
)

newComponent("player_ship",
  numeric("reload"),
  int("shield"),
  int("health"),
  int("lives"),
  int("score")
)
