
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

CREATE TABLE IF NOT EXISTS asteroid_hit
(
  entity INTEGER REFERENCES entity (entity),
  bullet INTEGER REFERENCES entity (entity),
  damage INT
)

CREATE TABLE IF NOT EXISTS var
(
  key PRIMARY KEY, value
);

-- System to start a new game
BEGIN TRANSACTION;
  INSERT OR REPLACE INTO var VALUES ( "score", 0 );
  INSERT OR REPLACE INTO var VALUES ( "lives", 3 );
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
    (SELECT value FROM var WHERE key = "last_entity")
  );
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

-- Stored views which make circle to circle collisions easier to deal with in SQL
CREATE VIEW collision_location AS SELECT
  location.entity AS entity, x, y, radius
  FROM location INNER JOIN collision USING(entity);

CREATE VIEW player_location AS SELECT
  location.entity AS entity, x, y, radius
  FROM location INNER JOIN player_ship USING (entity) INNER JOIN collision USING(entity);

CREATE VIEW player_collision AS SELECT
  a.entity AS collider, p.location AS player
  FROM player_location AS p JOIN collision_location AS a
  WHERE
    p.entity != a.entity AND
    (p.x - a.x) * (p.x - a.x) + (p.y - a.y) * (p.y - a.y) < (p.radius + a.radius) * (p.radius + a.radius);

-- System to destroy player ships when they collide with something. Also loses 1 life
BEGIN TRANSACTION;
  UPDATE var SET value = value - 1
    WHERE key = "lives" AND (SELECT count(collider) > 0 FROM player_collision);
  DELETE FROM entity WHERE entity IN (
    SELECT player FROM player_collision GROUP BY player
  );
COMMIT TRANSACTION;

CREATE VIEW bullet_location AS SELECT
  location.entity AS entity, x, y, damage
  FROM location INNER JOIN bullet USING(entity);

CREATE VIEW bullet_collision AS SELECT
  a.entity AS collider, p.location AS projectile, damage
  FROM bullet_location AS p JOIN collision_location AS a
  WHERE
    (p.x - a.x) * (p.x - a.x) + (p.y - a.y) * (p.y - a.y) < (a.radius * a.radius);

-- System to deal with bullets hitting asteroids
-- The asteroid needs to get smaller as a result of being hit, so we save the hits.
-- Otherwise, damage might be done to the asteroid, but the collision would be lost
-- when deleting bullets.
BEGIN TRANSACTION;
  INSERT INTO asteroid_hit (entity, bullet, damage)
    SELECT collider, projectile, damage FROM bullet_collision
    WHERE collider IN (SELECT entity FROM asteroid);
  DELETE FROM bullet WHERE entity IN (SELECT bullet FROM asteroid_hit)
  UPDATE collision SET radius = radius - damage
    FROM asteroid_hit
    WHERE collision.entity = asteroid_hit.entity;
  DELETE FROM asteroid_hit;
COMMIT TRANSACTION;

-- System to convert asteroids with collision.radius < 0 into explosions
-- This also gives 100 score for each asteroid destroyed
BEGIN TRANSACTION;
  UPDATE var SET value = value + 100 * (
    SELECT count(entity) FROM collision INNER JOIN asteroid USING (entity)
    WHERE radius <= 0
  ) WHERE key = "score";
  INSERT INTO explosion
    SELECT asteroid.entity, sim_time() FROM collision INNER JOIN asteroid USING (entity)
    WHERE radius <= 0;
  DELETE FROM asteroid WHERE entity IN (SELECT entity FROM explosion);
  DELETE FROM collision WHERE entity IN (SELECT entity FROM explosion);
COMMIT TRANSACTION;

-- System to convert the oldest explosion back into new a collision if it's old enough
BEGIN TRANSACTION;
  INSERT OR REPLACE INTO var VALUES ( "last_entity", (
    SELECT entity FROM explosion WHERE age < (sim_time() - 10) ORDER BY age ASC LIMIT 1
  ));
  UPDATE location SET
    x = abs(random()) % 200) - 100, y = (abs(random()) % 200) - 100, theta = radians(abs(random()) % 360)
    WHERE
      entity = (SELECT value FROM var WHERE key = "last_entity");
  UPDATE mobile SET
    accel = 0, vel = (random() % 10) * 0.1 + 0.5, max_vel = 100, rotation = 0
    WHERE
      entity = (SELECT value FROM var WHERE key = "last_entity");
  INSERT OR ROLLBACK INTO collision VALUES (
    (SELECT value FROM var WHERE key = "last_entity"),
    (random() % 5) * 0.5 + 3
  );
  INSERT OR ROLLBACK INTO asteroid (entity) VALUES (
    (SELECT value FROM var WHERE key = "last_entity")
  );
  DELETE FROM explosion WHERE entity = (SELECT value FROM var WHERE key = "last_entity");
COMMIT TRANSACTION;

-- Render Systems are just select statements used to let the engine know what needs to be rendered.

-- Bullet rendering system
-- Note, the details of the bullet aren't important, just the position
SELECT x, y, prev_x, prev_y FROM location INNER JOIN bullet
WHERE location.entity = bullet.entity;

-- Asteroid rendering system
SELECT x, y, theta, radius FROM location INNER JOIN collision INNER JOIN asteroid
WHERE location.entity = collision.entity AND location.entity = asteroid.entity;

-- Explosion rendering system
SELECT x, y, radius FROM location INNER JOIN explosion
WHERE location.entity = explosion.entity AND explosion.age < (sim_time() - 2);

-- Score and lives rendering
SELECT key, value FROM var WHERE key = "score" OR key = "lives";

-- Render system to tell the engine if there is an active game
SELECT active = value > 0 FROM var WHERE key = "lives";

-- other things not yet in this description fo the game:
--   player respawn mechanics
--   system to force asteroids to not spawn too close to the player ship
--   power-ups to increase shield, give invulnerability, etc.
--   enemy ships
--   score blips which give score when collided by the player ship
--   A system to maintain a number of asteroids in game at any given time
--   difficulty levels, or a series of increasingly difficult asteroid sets
--     e.g. first level is five asteroids, but each level after gets an extra 5
--     and each level has a maximum number of asteroids that can be in game at once

-- The goal with update systems is to be able to condense them down into a single
-- massive set of prepared statements that can be run in sequence to update the game
-- state.

-- Update systems are followed by prepared statements for the render systems which
-- feed or even directly create command buffers to send to Vulkan. It might also
-- be useful to use this to trigger sounds, such as when a new bullet is spawned, etc.

-- Other prepared statements might be used purely for engine logic, such as
-- determining whether a game menu should display.
