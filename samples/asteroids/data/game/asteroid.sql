

CREATE TABLE IF NOT EXISTS entity
(
  entity INTEGER PRIMARY KEY AUTOINCREMENT,
  id TEXT UNIQUE
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

CREATE TABLE IF NOT EXISTS collision
(
  entity INT PRIMARY KEY
    REFERENCES entity (entity) ON DELETE CASCADE,
  radius NUMERIC
);

CREATE TABLE IF NOT EXISTS asteroid
(
  entity INT PRIMARY KEY
    REFERENCES entity (entity) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS explosion
(
  entity INT PRIMARY KEY
    REFERENCES entity (entity) ON DELETE CASCADE,
  age INT
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
  health INT
);

CREATE TABLE IF NOT EXISTS var
(
  key PRIMARY KEY, value
);

-- System to start a new game
BEGIN TRANSACTION;
  INSERT OR REPLACE INTO var VALUES ( "score", 0 );
  INSERT OR REPLACE INTO var VALUES ( "lives", 3 );
COMMIT TRANSACTION;

-- System to add the player entity if it does not exist.
BEGIN TRANSACTION;
  INSERT OR ROLLBACK INTO entity (id) VALUES ("player");
  INSERT OR REPLACE INTO var VALUES ( "last_entity", last_insert_rowid() );
  INSERT OR ROLLBACK INTO location VALUES (
    (SELECT value FROM var WHERE key = "last_entity"),
    0, 0, 0, 0, 0
  );
  INSERT OR ROLLBACK INTO mobile VALUES (
    (SELECT value FROM var WHERE key = "last_entity"),
    0, 0, 5, 0
  );
  INSERT OR ROLLBACK INTO collision VALUES (
    (SELECT value FROM var WHERE key = "last_entity"),
    1
  );
  INSERT OR ROLLBACK INTO player_ship VALUES (
    (SELECT value FROM var WHERE key = "last_entity"),
    1.0, 100, 100, 3, 0
  );
COMMIT TRANSACTION;

-- System to adjust locations based on current location and mobile data
UPDATE OR FAIL location SET
  prev_x = old.x,
  prev_y = old.y,
  theta = old.theta + mobile.rotation,
  x = old.x - sin(old.theta) * mobile.vel,
  y = old.y + cos(old.theta) * mobile.vel
FROM location AS old INNER JOIN mobile USING (entity)
WHERE vel > 0.0 AND old.entity = mobile.entity;

-- System to deal with acceleration
UPDATE OR FAIL mobile SET
  vel = min(old.max_vel, max(0.0, old.vel + old.accel))
FROM mobile AS old
WHERE accel != 0.0;

-- System to wrap around the play field
BEGIN TRANSACTION;
  UPDATE location SET x = x + 200, prev_x = prev_x + 200 WHERE x < -100;
  UPDATE location SET x = x - 200, prev_x = prev_x - 200 WHERE x > 100;
  UPDATE location SET y = y + 200, prev_y = prev_y + 200 WHERE y < -100;
  UPDATE location SET y = y - 200, prev_y = prev_y - 200 WHERE y > 100;
COMMIT TRANSACTION;

-- System to add an asteroid to the game
BEGIN TRANSACTION;
  INSERT OR ROLLBACK INTO entity (id) VALUES (NULL);
  INSERT OR REPLACE INTO var VALUES ( "last_entity", last_insert_rowid() );
  INSERT OR ROLLBACK INTO location (entity, x, y, theta) VALUES (
    (SELECT value FROM var WHERE key = "last_entity"),
    (abs(random()) % 200) - 100, (abs(random()) % 200) - 100, radians(random() % 360)
  );
  INSERT OR ROLLBACK INTO mobile VALUES (
    (SELECT value FROM var WHERE key = "last_entity"),
    0, (random() % 10) * 0.1 + 0.5, 100, 0
  );
  INSERT OR ROLLBACK INTO collision VALUES (
    (SELECT value FROM var WHERE key = "last_entity"),
    (random() % 5) * 0.5 + 3
  );
  INSERT OR ROLLBACK INTO asteroid (entity) VALUES (
    (SELECT value FROM var WHERE key = "last_entity"),
  );
COMMIT TRANSACTION;

-- System to initialize previous location for newly spawned location components
BEGIN TRANSACTION;
  UPDATE location SET prev_x = x, prev_y = y WHERE prev_x ISNULL;
COMMIT TRANSACTION;

-- System to add a bullet to the game
BEGIN TRANSACTION;
  INSERT OR ROLLBACK INTO entity (id) VALUES (NULL);
  INSERT OR REPLACE INTO var VALUES ( "last_entity", last_insert_rowid() );
  INSERT OR ROLLBACK INTO location (entity, x, y, theta) VALUES (
    (SELECT value FROM var WHERE key = "last_entity")
  );
  UPDATE location SET
    (x, y, prev_x, prev_y, theta) = (
      SELECT x - sin(theta), y + cos(theta), x, y, theta FROM location
      WHERE entity = (SELECT entity FROM entity WHERE id = "player")
    )
  WHERE entity = ((SELECT value FROM var WHERE key = "last_entity"));
  INSERT OR ROLLBACK INTO mobile VALUES (
    (SELECT value FROM var WHERE key = "last_entity"),
    0, 5, 100, 0
  );
  INSERT OR ROLLBACK INTO bullet (entity) VALUES (
    (SELECT value FROM var WHERE key = "last_entity"),
    sim_time(), 0.5
  );
COMMIT TRANSACTION;

-- System to destroy bullets when their age value is 10 seconds lower than the current simulation time
DELETE FROM entity WHERE entity IN (
  SELECT entity FROM bullet WHERE age < (sim_time() - 10)
);

-- TODO: create systems to check for collisions between player ship and asteroids
-- When this happens, the player entity should be deleted and the lives variable reduced by 1
-- start by making a select statement which returns player entities that intersect any asteroid, with a limit of 1
-- the select statement should return the entity number.
-- a delete statement can then be easily made from that.

-- TODO: create system to react to collisions between bullets and asteroids
-- when this happens:
--   the asteroid's collision radius is decreased by the damage of the bullet
--   the bullet is deleted

-- System to convert asteroids with collision.radius < 0 into explosions
-- TODO: Test the following
BEGIN TRANSACTION;
  INSERT INTO explosion
    SELECT asteroid.entity, sim_time() FROM collision INNER JOIN asteroid
    WHERE collision.entity = asteroid.entity AND radius <= 0;
  DELETE FROM asteroid WHERE entity IN (SELECT entity FROM explosion);
  DELETE FROM collision WHERE entity IN (SELECT entity FROM explosion);
COMMIT TRANSACTION;

-- System to destroy old explosions
DELETE FROM entity WHERE entity IN (
  SELECT entity FROM explosion WHERE age < (sim_time() - 10)
);
