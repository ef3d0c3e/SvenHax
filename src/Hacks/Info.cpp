#include "Info.hpp"
#include "../Hooks/Hooks.hpp"
#include "ESP.hpp"
#include <array>
#include <filesystem>

std::map<i32, Info::EntInfo> Info::entityInfo;
std::map<std::string_view, std::pair<Info::EntityClass, Info::ClassFlags>> Info::modelMap;

Info::EntInfo::EntInfo(Entity* _ent):
	ent(*_ent)
{
	if (!ent.model)
	{
		classId = EntityClass::NONE;
		classFlags = ClassFlags::NEUTRAL;
		return;
	}
	std::string_view model(ent.model->name.data(), strnlen(ent.model->name.data(), ent.model->name.size()));
	if (model.ends_with(".mdl"))
	{
		const auto pos = model.rfind('/');
		if (pos != std::string::npos)
			model = model.substr(pos+1);
	}
	const auto it = modelMap.find(model);
	if (it == modelMap.end())
	{
		classId = EntityClass::NONE;
		classFlags = ClassFlags::NEUTRAL;
	}
	else
	{
		classId = it->second.first;
		classFlags = it->second.second;
	}
}

Info::ClassFlags Info::EntInfo::GetClassFlags() const noexcept
{
	return classFlags;
}

Info::EntityClass Info::EntInfo::GetClass() const noexcept
{
	return classId;
}

std::string Info::EntInfo::GetClassName() const noexcept
{
	static const std::array<std::string, 44> classes = {
		"Unknown",
		"Player",
		"Scientist",
		"Barney",
		"Otis",
		"Headcrab",
		"Zombie",
		"Bullsquid",
		"Houndeye",
		"Barnacle",
		"Vortigaunt",
		"Human Grunt",
		"Alient Grunt",
		"Tentacle",
		"Sentry",
		"Turret",
		"Leech",
		"G-Man",
		"Female Assassin",
		"Male Assassin",
		"Snark",
		"Chumtoad",
		"Alien Controller",
		"Ichtyosaur",
		"Gargantua",
		"Baby Gargantua",
		"Big Momma",
		"Osprey",
		"Apache",
		"Nihilanth",
		"H.E.V",
		"Xen Tree",
		"Xen Fungus",
		"Human Grunt",
		"Gonome",
		"Pit Drone",
		"Shock Trooper",
		"Voltigore",
		"Baby Voltigore",
		"Pit Worm",
		"Shock Rifle",
		"Spore Ammo",
		"Heavy Grunt",
		"Robot Grunt"
	};

	return classes[classId];
}

bool Info::EntInfo::IsFriend() const noexcept
{
	return classFlags & ClassFlags::FRIEND;
}

bool Info::EntInfo::IsEnemy() const noexcept
{
	return classFlags & ClassFlags::ENEMY;
}

bool Info::EntInfo::IsNeutral() const noexcept
{
	return classFlags & ClassFlags::NEUTRAL;
}

bool Info::EntInfo::IsWorldEntity() const noexcept
{
	return classFlags & ClassFlags::WORLD_ENTITY;
}

bool Info::EntInfo::IsDeadbody() const noexcept
{
	return classFlags & ClassFlags::DEAD_BODY;
}

void Info::PopulateModelMap()
{
	modelMap.clear();
	// TODO: Parse map's bsp

	modelMap.insert({"scientist.mdl", std::make_pair(EntityClass::SCIENTIST, ClassFlags::FRIEND | ClassFlags::DEAD_BODY)});
	modelMap.insert({"cleansuit_scientist.mdl", std::make_pair(EntityClass::SCIENTIST, ClassFlags::FRIEND | ClassFlags::DEAD_BODY)});
	modelMap.insert({"scientist_rosenberg.mdl", std::make_pair(EntityClass::SCIENTIST, ClassFlags::FRIEND | ClassFlags::DEAD_BODY)});
	modelMap.insert({"wheelchair_sci.mdl", std::make_pair(EntityClass::SCIENTIST, ClassFlags::FRIEND | ClassFlags::DEAD_BODY)});

	modelMap.insert({"barney.mdl", std::make_pair(EntityClass::BARNEY, ClassFlags::FRIEND | ClassFlags::DEAD_BODY)});
	modelMap.insert({"otis.mdl", std::make_pair(EntityClass::OTIS, ClassFlags::FRIEND | ClassFlags::DEAD_BODY)});

	modelMap.insert({"headcrab.mdl", std::make_pair(EntityClass::HEADCRAB, ClassFlags::ENEMY)});

	modelMap.insert({"zombie.mdl", std::make_pair(EntityClass::ZOMBIE, ClassFlags::ENEMY)});
	modelMap.insert({"zombie_soldier.mdl", std::make_pair(EntityClass::ZOMBIE, ClassFlags::ENEMY)});
	modelMap.insert({"zombie_barney.mdl", std::make_pair(EntityClass::ZOMBIE, ClassFlags::ENEMY)});

	modelMap.insert({"bullsquid.mdl", std::make_pair(EntityClass::BULLSQUID, ClassFlags::ENEMY)});
	modelMap.insert({"houndeye.mdl", std::make_pair(EntityClass::HOUNDEYE, ClassFlags::ENEMY)});
	modelMap.insert({"barnacle.mdl", std::make_pair(EntityClass::BARNACLE, ClassFlags::ENEMY)});
	modelMap.insert({"islave.mdl", std::make_pair(EntityClass::VORTIGAUNT, ClassFlags::ENEMY)});

	modelMap.insert({"hgrunt.mdl", std::make_pair(EntityClass::HUMAN_GRUNT, ClassFlags::ENEMY | ClassFlags::DEAD_BODY)});
	modelMap.insert({"agrunt.mdl", std::make_pair(EntityClass::ALIENT_GRUNT, ClassFlags::ENEMY)});

	modelMap.insert({"tentacle2.mdl", std::make_pair(EntityClass::TENTACLE, ClassFlags::ENEMY)});
	modelMap.insert({"tentacle3.mdl", std::make_pair(EntityClass::TENTACLE, ClassFlags::ENEMY)});

	modelMap.insert({"sentry.mdl", std::make_pair(EntityClass::SENTRY, ClassFlags::ENEMY)});
	modelMap.insert({"turret.mdl", std::make_pair(EntityClass::TURRET, ClassFlags::ENEMY)});
	modelMap.insert({"miniturret.mdl", std::make_pair(EntityClass::TURRET, ClassFlags::ENEMY)});
	modelMap.insert({"leech.mdl", std::make_pair(EntityClass::LEECH, ClassFlags::ENEMY)});

	modelMap.insert({"gman.mdl", std::make_pair(EntityClass::GMAN, ClassFlags::ENEMY)});

	modelMap.insert({"hassassin.mdl", std::make_pair(EntityClass::FEMALE_ASSASSIN, ClassFlags::ENEMY)});

	modelMap.insert({"w_squeak.mdl", std::make_pair(EntityClass::SNARK, ClassFlags::ENEMY)});
	modelMap.insert({"chubby.mdl", std::make_pair(EntityClass::CHUMTOAD, ClassFlags::FRIEND)});

	modelMap.insert({"controller.mdl", std::make_pair(EntityClass::ALIEN_CONTROLLER, ClassFlags::ENEMY)});

	modelMap.insert({"icky.mdl", std::make_pair(EntityClass::ICHTYOSAUR, ClassFlags::ENEMY)});

	modelMap.insert({"garg.mdl", std::make_pair(EntityClass::GARGANTUA, ClassFlags::ENEMY)});
	modelMap.insert({"babygarg.mdl", std::make_pair(EntityClass::BABY_GARGANTUA, ClassFlags::ENEMY)});

	modelMap.insert({"big_mom.mdl", std::make_pair(EntityClass::BIG_MOMMA, ClassFlags::ENEMY)});

	modelMap.insert({"osprey.mdl", std::make_pair(EntityClass::OSPREY, ClassFlags::ENEMY)});
	modelMap.insert({"apache.mdl", std::make_pair(EntityClass::APACHE, ClassFlags::ENEMY)});
	modelMap.insert({"blkop_apache.mdl", std::make_pair(EntityClass::APACHE, ClassFlags::ENEMY)});

	modelMap.insert({"nihilanth.mdl", std::make_pair(EntityClass::NIHILANTH, ClassFlags::ENEMY)});

	modelMap.insert({"player.mdl", std::make_pair(EntityClass::HEV, ClassFlags::WORLD_ENTITY)});
	modelMap.insert({"tree.mdl", std::make_pair(EntityClass::XEN_TREE, ClassFlags::WORLD_ENTITY)});
	modelMap.insert({"fungus.mdl", std::make_pair(EntityClass::XEN_FUNGUS, ClassFlags::WORLD_ENTITY)});
	modelMap.insert({"fungus(small).mdl", std::make_pair(EntityClass::XEN_FUNGUS, ClassFlags::WORLD_ENTITY)});
	modelMap.insert({"fungus(large).mdl", std::make_pair(EntityClass::XEN_FUNGUS, ClassFlags::WORLD_ENTITY)});

	modelMap.insert({"hgrunt_opforf.mdl", std::make_pair(EntityClass::HUMAN_GRUNT_OPFOR, ClassFlags::FRIEND | ClassFlags::DEAD_BODY)});
	modelMap.insert({"hgrunt_torchf.mdl", std::make_pair(EntityClass::HUMAN_GRUNT_OPFOR, ClassFlags::FRIEND | ClassFlags::DEAD_BODY)});
	modelMap.insert({"hgrunt_medicf.mdl", std::make_pair(EntityClass::HUMAN_GRUNT_OPFOR, ClassFlags::FRIEND | ClassFlags::DEAD_BODY)});
	modelMap.insert({"hgrunt_opfor.mdl", std::make_pair(EntityClass::HUMAN_GRUNT_OPFOR, ClassFlags::FRIEND | ClassFlags::DEAD_BODY)});
	modelMap.insert({"hgrunt_torch.mdl", std::make_pair(EntityClass::HUMAN_GRUNT_OPFOR, ClassFlags::FRIEND | ClassFlags::DEAD_BODY)});
	modelMap.insert({"hgrunt_medic.mdl", std::make_pair(EntityClass::HUMAN_GRUNT_OPFOR, ClassFlags::FRIEND | ClassFlags::DEAD_BODY)});

	modelMap.insert({"massn.mdl", std::make_pair(EntityClass::MALE_ASSASSIN, ClassFlags::ENEMY)});

	modelMap.insert({"gonome.mdl", std::make_pair(EntityClass::GONOME, ClassFlags::ENEMY)});

	modelMap.insert({"pit_drone.mdl", std::make_pair(EntityClass::PIT_DRONE, ClassFlags::ENEMY)});
	modelMap.insert({"strooper.mdl", std::make_pair(EntityClass::SHOCK_TROOPER, ClassFlags::ENEMY)});

	modelMap.insert({"voltigore.mdl", std::make_pair(EntityClass::VOLTIGORE, ClassFlags::ENEMY)});
	modelMap.insert({"baby_voltigore.mdl", std::make_pair(EntityClass::BABY_VOLTIGORE, ClassFlags::ENEMY)});
	modelMap.insert({"pit_worm_up.mdl", std::make_pair(EntityClass::PIT_WORM, ClassFlags::ENEMY)});

	modelMap.insert({"w_shock_rifle.mdl", std::make_pair(EntityClass::SHOCK_RIFLE, ClassFlags::ENEMY)});

	modelMap.insert({"spore_ammo.mdl", std::make_pair(EntityClass::SPORE_AMMO, ClassFlags::ENEMY)});

	modelMap.insert({"hwgrunt.mdl", std::make_pair(EntityClass::HEAVY_GRUNT, ClassFlags::ENEMY)});
	modelMap.insert({"rgrunt.mdl", std::make_pair(EntityClass::ROBOT_GRUNT, ClassFlags::ENEMY)});
}

void Info::Paint()
{
	for (std::size_t i = 2; i < Entity::EntNum; ++i) // 0 = world
	{
		Entity* ent = gEngineFuncs->GetEntityByIndex(i);

		auto it = entityInfo.find(i);
		if (it == entityInfo.end()) // Add
		{
			if (ent)
				entityInfo.insert({i, EntInfo(ent)});
		}
		else if (&it->second.ent != ent) // Changed
		{
			it->second.dormant = true;
			// NOTE: In order to have dormant working, in case the entity disappears, we wait a certain duration before putting another one (or simply erasing) it's entry from the list
			// This means that a newly spawned entity with the same ID as an entity that died recently won't be shown instantly
			if (SDL2::frameBegin - it->second.lastUpdated >= std::chrono::milliseconds(Settings::ESP::maxDormantTime))
			{
				entityInfo.erase(it);
				if (ent)
					entityInfo.insert({i, EntInfo(ent)});
			}
		}
		else // Entity still there
			it->second.lastUpdated = SDL2::frameBegin;
	}
}
