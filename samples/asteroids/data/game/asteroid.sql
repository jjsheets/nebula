

CREATE TABLE IF NOT EXISTS entity
(
  entity INT PRIMARY KEY AUTOINCREMENT
);

CREATE TABLE IF NOT EXISTS location
(
  entity INT PRIMARY KEY
    REFERENCES entity (entity) ON DELETE CASCADE,
  x NUMERIC,
  y NUMERIC,
  prev_x NUMERIC,
  prev_y NUMERIC,
  theta NUMERIC
);

CREATE TABLE IF NOT EXISTS mobile
(
  entity INT PRIMARY KEY
    REFERENCES entity (entity) ON DELETE CASCADE,
  accel NUMERIC,
  vel NUMERIC,
  max_vel NUMERIC,
  rotation NUMERIC
);

// System to adjust locations based on current location and mobile data
UPDATE OR FAIL location SET
  prev_x = old.x,
  prev_y = old.y,
  theta = old.theta + mobile.rotation,
  x = old.x - sin(old.theta) * mobile.vel,
  y = old.y + cos(old.theta) * mobile.vel
FROM location AS old INNER JOIN mobile USING (entity)
WHERE vel > 0.0;

// System to deal with acceleration
UPDATE OR FAIL mobile SET
  vel = min(old.max_vel, max(0.0, old.vel + old.accel))
FROM mobile AS old
WHERE accel != 0.0;

CREATE TABLE IF NOT EXISTS collision
(
  entity INT PRIMARY KEY
    REFERENCES entity (entity) ON DELETE CASCADE,
  radius NUMERIC
);

CREATE TABLE IF NOT EXISTS asteroid
(
  entity INT PRIMARY KEY
    REFERENCES entity (entity) ON DELETE CASCADE,
  size INT
);

CREATE TABLE IF NOT EXISTS explosion
(
  entity INT PRIMARY KEY
    REFERENCES entity (entity) ON DELETE CASCADE,
  size INT
);

CREATE TABLE IF NOT EXISTS bullet
(
  entity INT PRIMARY KEY
    REFERENCES entity (entity) ON DELETE CASCADE,
  age NUMERIC,
  damage INT
);

CREATE TABLE IF NOT EXISTS player_ship
(
  entity INT PRIMARY KEY
    REFERENCES entity (entity) ON DELETE CASCADE,
  reload NUMERIC,
  shield INT,
  health INT,
  lives INT,
  score INT
);
