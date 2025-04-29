#pragma once
#include <RuneEngine/Engine.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/InputSoundFile.hpp>
#include <SFML/System.hpp>

#include "Font.hpp"
#include "components/GameInfo.hpp"
#include "exceptions/exceptions.hpp"

inline sf::Mutex mutex;
struct ChatSocket;
struct Client;
struct Server;

class Transferable;

template<class Base, class T>
struct is_fabric_type
{
    constexpr static const bool value = std::is_base_of_v<Base, T> && !std::is_same_v<Base, T>;
};

template<class Base, class T>
constexpr const bool is_fabric_type_v = is_fabric_type<Base, T>::value;

class TransferableObject;
class TransferableObjectFabric;
class TransferableAction;
class TransferableActionFabric;
class BasicRouterResponse;
template<class RespT = BasicRouterResponse>
class BasicRouter;
using Router = BasicRouter<>;

class UdpRouter;

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
