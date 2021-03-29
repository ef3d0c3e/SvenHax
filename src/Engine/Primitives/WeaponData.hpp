#ifndef ENGINE_PRIMITIVES_WEAPONDATA_HPP
#define ENGINE_PRIMITIVES_WEAPONDATA_HPP

struct WeaponData
{
	i32 id;
	i32 clip;

	f32 nextPrimaryAttack;
	f32 nextSecondaryAttack;
	f32 timeWeaponIdle;

	i32 inReload;
	i32 inSpecialReload;
	f32 nextReload;
	f32 pumpTime;
	f32 reloadTime;

	f32 aimedDamage;
	f32 nextAimBonus;
	i32 inZoom;
	i32 weaponState;

	Vector<i32, 4> iuser;
	Vector<f32, 4> fuser;
	//Vector<Vec, 4> vuser;
};

#endif // ENGINE_PRIMITIVES_WEAPONDATA_HPP
