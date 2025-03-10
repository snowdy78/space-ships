#pragma once
#include <RuneEngine/Engine.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/InputSoundFile.hpp>
#include <SFML/System.hpp>

#include "Font.hpp"
#include "components/GameInfo.hpp"

inline sf::Mutex mutex;
struct ChatSocket;
struct Client;
struct Server;

class TransferObject;
class TransferInstance;

class RigitBody2d;
class Bullet;
class Ship;
class BulletMother;
class Gun;
class Collider;
class Collidable;
class PolygonCollider;
class EllipseCollider;
class AbstractShip;
class SpaceField;
class DamageDealer;
class Hittable;
