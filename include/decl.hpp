#pragma once
#include <RuneEngine/Engine.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/InputSoundFile.hpp>
#include <SFML/System.hpp>
#include <concepts>

#include "Font.hpp"
#include "components/GameInfo.hpp"
#include "exceptions/exceptions.hpp"

template<class T>
rn::Json to_json(const rn::Vec2<T> &vec)
{
	return {
		{ "x", vec.x },
		{ "y", vec.y }
	};
}


inline sf::Mutex mutex;
struct ChatSocket;
struct Client;
struct Server;
class AbstractAction;

class Transferable;

template<class Base, class T>
struct is_fabric_type
{
	constexpr static const bool value = std::is_base_of_v<Base, T> && !std::is_same_v<Base, T> && !std::is_abstract_v<T>;
};

template<class Base, class T>
constexpr const bool is_fabric_type_v = is_fabric_type<Base, T>::value;

template<typename... Types>
struct type_list
{
};

template<typename T, typename List>
concept is_any_of = []<typename... Types>(type_list<Types...>) {
	return (std::is_same_v<Types, T> || ...);
}(List());

template<class T>
concept BindingSeparatorConcept = std::is_enum_v<T>;
enum class DefaultBindingSeparators
{
	Controls
};

template<class PropsT>
concept BindingPropsConcept = requires {
	PropsT{};
};
struct DefaultBindingProps : rn::Json
{
	using rn::Json::Json;
};
template<
	BindingSeparatorConcept BindSepE = DefaultBindingSeparators,
	class KeyIdT = size_t, class AcceptedKeyTypes = type_list<>, class... PropsTypes>
class ControlsScheme;
using Controls = ControlsScheme<DefaultBindingSeparators, DefaultBindingProps, size_t, type_list<sf::Keyboard::Key, sf::Mouse::Button>>;

class TransferableObject;
template<class T>
class BaseTransferableObject;
class TransferableObjectFabric;
class TransferableAction;
class TransferableActionFabric;
class BasicRouterResponse;
template<class RespT = BasicRouterResponse>
class BasicRouter;
using Router = BasicRouter<>;

class UdpRouter;

class Effect;
class EffectManager;
class SoundManager;
template<template<class T> class Container, class Ty>
class AbstractManager;
class ActionManager;
class FileLoader;
class GameGlobals;

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

class ShootAction;
