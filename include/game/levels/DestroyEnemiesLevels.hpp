#pragma once

#include "BasicLevelDestroyEnemies.hpp"
#include "game/asteroids/GiantAsteroid.hpp"
#include "game/asteroids/SimpleAsteroid.hpp"
#include "game/asteroids/SmallAsteroid.hpp"
#include "game/guns/LaserRifle.hpp"
#include "game/guns/MiniGun.hpp"
#include "game/ships/DemonicEnemyShip.hpp"
#include "game/ships/EnemyShip.hpp"
#include "game/ships/RedEnemyShip.hpp"

template<class BasicLevel, class WeaponSummon, class... EnemiesTypes>
class NumeratedLevel : public BasicLevel
{
public:
	NumeratedLevel(SpaceField &field)
		: BasicLevel(field)
	{
		BasicLevel::template pool<EnemiesTypes...>([](EnemiesTypes &enemy) {
			BasicLevel::randomlySummonEnemy(enemy);
		}...);
		if constexpr (!std::is_same_v<WeaponSummon, void>)
			BasicLevel::template randomlySummonWeapon<WeaponSummon>();
	}
};
struct Level1;
struct Level2;
struct Level3;
struct Level4;
struct Level5;
struct Level1Factory final : BasicLevelFactory<Level1Factory, Level1>
{
};
struct Level2Factory final : BasicLevelFactory<Level2Factory, Level2>
{
};
struct Level3Factory final : BasicLevelFactory<Level3Factory, Level3>
{
};
struct Level4Factory final : BasicLevelFactory<Level4Factory, Level4>
{
};
struct Level5Factory final : BasicLevelFactory<Level5Factory, Level5>
{
};
constexpr char level1_name[] = "Level1";
struct Level1 final : NumeratedLevel<
						  BasicLevelDestroyEnemies<
							  Level1, Level2Factory, AbstractLevel::Difficulty::Star1, level1_name, SimpleAsteroid>,
						  Pistol, EnemyShip>
{
	using NumeratedLevel::NumeratedLevel;
};

constexpr char level2_name[] = "Level2";
struct Level2 final
	: NumeratedLevel<
		  BasicLevelDestroyEnemies<
			  Level2, Level3Factory, AbstractLevel::Difficulty::Star2, level2_name, SimpleAsteroid, SmallAsteroid>,
		  LaserRifle, EnemyShip>
{
	using NumeratedLevel::NumeratedLevel;
};

constexpr char Level3_name[] = "Level3";
struct Level3 final
	: NumeratedLevel<
		  BasicLevelDestroyEnemies<
			  Level3, Level4Factory, AbstractLevel::Difficulty::Star3, Level3_name, SimpleAsteroid, SmallAsteroid>,
		  void, EnemyShip, RedEnemyShip>
{
	using NumeratedLevel::NumeratedLevel;
};
constexpr char Level4_name[] = "Level4";
struct Level4 final : NumeratedLevel<
						  BasicLevelDestroyEnemies<
							  Level4, Level5Factory, AbstractLevel::Difficulty::Star4, Level4_name, SimpleAsteroid,
							  SmallAsteroid, GiantAsteroid>,
						  MiniGun, EnemyShip, RedEnemyShip>
{
	using NumeratedLevel::NumeratedLevel;
};
constexpr char Level5_name[] = "Level5";
struct Level5 final : NumeratedLevel<BasicLevelDestroyEnemies<
						  Level5, Level5Factory, AbstractLevel::Difficulty::Star5, Level5_name, SimpleAsteroid,
						  SmallAsteroid, GiantAsteroid>, void, EnemyShip, RedEnemyShip, DemonicEnemyShip>
{
	using NumeratedLevel::NumeratedLevel;
};
