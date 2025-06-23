#pragma once
#include <RuneEngine/Engine.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/InputSoundFile.hpp>
#include <SFML/System.hpp>
#include <concepts>

#include "components/GameInfo.hpp"
#include "exceptions/exceptions.hpp"
#include "components/soft_parameter.hpp"

#ifndef SPACE_SHIP_DEBUG
	//#define SPACE_SHIP_DEBUG
#endif

inline static sf::Color default_collider_color{ 0, 210, 160, 64 };

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

template<class T>
using loading = const T *const &;
template<class T>
using loading_ptr = const T *const *;
template<class T>
class LoadFunction;
template<class T>
concept LoaderConcept = requires(const sf::String &path, T &value) { LoadFunction<T>::load(path, value); };

template<LoaderConcept T>
class FileLoader;
class GameManager;

class GameObject;
class GameObjectFactory;
class GameObjectTranslator;

class RigitBody2d;
class AbstractBullet;
class PlayerShip;
class AbstractAsteroid;
class AbstractWeapon;
class Collider;
class Collidable;
class PolygonCollider;
class EllipseCollider;
class AbstractShip;
class SpaceField;
class SpaceFieldObject;
class SpaceItem;
class DamageDealer;
class Hittable;
class AbstractAction;

class ShootAction;


template<class T>
concept SpaceFieldObjectConcept = std::is_base_of_v<SpaceFieldObject, T>;
template<class T, class... Args>
concept ShipConcept = std::is_base_of_v<AbstractShip, T> && SpaceFieldObjectConcept<T> && !std::is_abstract_v<T>
					  && requires(Args const &...args) { T(args...); };
template<class T>
concept BulletConcept = std::is_base_of_v<AbstractBullet, T> && SpaceFieldObjectConcept<T> && !std::is_abstract_v<T>;
template<class T>
concept AsteroidConcept
	= std::is_base_of_v<AbstractAsteroid, T> && SpaceFieldObjectConcept<T> && !std::is_abstract_v<T>;

template<class T>
concept SpaceItemConcept = std::is_base_of_v<SpaceItem, T>;
template<class W>
concept WeaponConcept = std::is_base_of_v<AbstractWeapon, W> && std::is_final_v<W> && SpaceItemConcept<W>;
