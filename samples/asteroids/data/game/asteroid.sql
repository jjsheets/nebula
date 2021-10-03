

CREATE TABLE IF NOT EXISTS entity
(
  entity INT PRIMARY KEY AUTOINCREMENT
);

CREATE TABLE IF NOT EXISTS locatable
(
  entity INT PRIMARY KEY
    REFERENCES entity (entity) ON DELETE CASCADE,
  x NUMERIC,
  y NUMERIC,
  prev_x NUMERIC,
  prev_y NUMERIC,
  theta NUMERIC
);

CREATE TABLE IF NOT EXISTS movable
(
  entity INT PRIMARY KEY
    REFERENCES entity (entity) ON DELETE CASCADE,
  accel NUMERIC,
  vel NUMERIC,
  rotation NUMERIC
);

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

CREATE TABLE IF NOT EXISTS player
(
  entity INT PRIMARY KEY
    REFERENCES entity (entity) ON DELETE CASCADE,
  reload NUMERIC,
  shield INT,
  health INT,
  lives INT,
  score INT
);
