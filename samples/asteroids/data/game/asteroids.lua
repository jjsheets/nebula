
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

local collision = newComponent("collision",
  numeric("radius")
)

local asteroid = newComponent("asteroid",
  int("size")
)

local explosion = newComponent("explosion",
  int("size")
)

local bullet = newComponent("bullet",
  numeric("age"),
  int("damage")
)

local player_ship = newComponent("player_ship",
  numeric("reload"),
  int("shield"),
  int("health"),
  int("lives"),
  int("score")
)

local accelerate = update{
  target = mobile,
  given = mobile:as("old"),
  require = "old.accel != 0.0",
  set = "vel = min(old.max_vel, max(0.0, old.vel + old.accel))"
}

local move = update{
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

local player_location = newView("player_location",
  select{
    given = entitiesWithAll(location, player_ship),
    limit = 1,
    result = {"x", "y"}
  }
)

local movement = systemGroup(
  accelerate, move
)
