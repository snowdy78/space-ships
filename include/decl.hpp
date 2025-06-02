#pragma once
#include <RuneEngine/Engine.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/InputSoundFile.hpp>
#include <SFML/System.hpp>
#include <concepts>

#include "Font.hpp"
#include "components/GameInfo.hpp"
#include "exceptions/exceptions.hpp"

#ifndef SPACE_SHIP_DEBUG
	#define SPACE_SHIP_DEBUG
#endif

inline sf::Mutex mutex;
struct ChatSocket;
struct Client;
struct Server;
class AbstractAction;

class Transferable;

template<typename... Types>
struct type_list {};

template<class Base, class T>
struct is_fabric_type
{
	constexpr static bool value = std::is_base_of_v<Base, T> && !std::is_same_v<Base, T>;
};

template<class Base, class T>
constexpr bool is_fabric_type_v = is_fabric_type<Base, T>::value;

template<class List, template<class T, class... Args> class Pred, class... Args>
struct is_any_of
{
	constexpr static bool value = []<typename... Types>(type_list<Types...>) {
		return (Pred<Types, Args...>::value || ...);
	}(List());
};
template<class List, template<class... Args> class Pred, class... Args>
struct is_all_of
{
	constexpr static bool value = []<typename... Types>(type_list<Types...>) {
		return (Pred<Types, Args...>::value && ...);
	}(List());
};

template<class List, template<class T, class... Args> class Pred, class... Args>
constexpr bool is_any_of_v = is_any_of<List, Pred, Args...>::value;

template<class List, template<class T, class... Args> class Pred, class... Args>
constexpr bool is_all_of_v = is_all_of<List, Pred, Args...>::value;


template<typename T, typename List>
struct is_any_same_of
{
	constexpr static bool value = is_any_of_v<List, std::is_same, T>;
};
template<class T, class List>
constexpr bool is_any_same_of_v = is_any_same_of<T, List>::value;


template<class T>
concept BindingSeparatorConcept = std::is_enum_v<T>;
enum class DefaultBindingSeparators
{
	Controls
};
class TransferDataConverter;
struct ConvertFunction;
template<class FromType, class ToType, class ConvertFunc = ConvertFunction>
class ConvertedTransfer;

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
class TransferObjectBase;
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
class GameManager;

class GameObject;
class GameObjectFactory;
class GameObjectTranslator;

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

#include "decl.inl"
