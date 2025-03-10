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

class Transferable;
class TransferableFabric;

class TransferableAction;
class TransferableActionFabric;

class GameObject;
class GameObjectFabric;
class GameObjectFabricTranslator;

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

class AbstractAction;

class ActionManager;
class ShipShootAction;
