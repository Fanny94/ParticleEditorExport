#include "LuaBinds.h"
using namespace Importer;
using namespace Gear;

LuaBinds::LuaBinds()
	: lua( nullptr ), validState( false )
{
}

LuaBinds::~LuaBinds()
{
	unload();
}

void LuaBinds::load( GearEngine* gearEngine,
					Assets* assets,
					CollisionHandler* collisionHandler,
					Controls* controls,
					Transform* transforms,
					int* boundTransforms,
					std::vector<ModelInstance>* models,
					std::vector<AnimatedInstance>* animatedModels,
					Camera* camera,
					std::vector<Gear::ParticleSystem*>* ps,
					AGI::AGIEngine* AI)
{
	lua = luaL_newstate();
	luaL_openlibs( lua );
	LuaErebus::registerFunctions( lua, transforms, controls );
	LuaGear::registerFunctions( lua, gearEngine, models, animatedModels, assets);
	LuaAssets::registerFunctions( lua, assets );
	LuaCollision::registerFunctions( lua, collisionHandler );
	LuaTransform::registerFunctions( lua, transforms, boundTransforms);
	LuaControls::registerFunctions( lua );
	LuaCamera::registerFunctions(lua, camera, transforms);
	LuaParticles::registerFunctions(lua, ps);
	LuaAI::registerFunctions(lua, transforms, AI);

	if( luaL_dofile( lua, "Scripts/main.lua" ) )
		std::cout << lua_tostring( lua, -1 ) << std::endl;
	else
	{
		lua_getglobal( lua, "Load" );
		if( lua_isfunction( lua, -1 ) )
		{
			if( lua_pcall( lua, 0, 0, 0 ) )
				std::cout << lua_tostring( lua, -1 ) << std::endl;
			else
			{
				// Get update function and store it as a reference.
				// This provides a faster lookup than calling lua_getglobal every frame.
				lua_getglobal( lua, "Update" );
				if( lua_isfunction( lua, -1 ) )
				{
					updateReference = luaL_ref( lua, LUA_REGISTRYINDEX );
					validState = true;
				}
			}
		}
	}
}

void LuaBinds::unload()
{
	if( validState )
	{
		lua_getglobal( lua, "Unload" );
		if( lua_isfunction( lua, -1 ) )
		{
			if( lua_pcall( lua, 0, 0, 0 ) )
				std::cout << lua_tostring( lua, -1 ) << std::endl;
		}
	}

	if( lua )
	{
		lua_close( lua );
		lua = nullptr;
	}

	validState = false;
}

void LuaBinds::update( Controls* controls, float deltaTime )
{
	if( validState )
	{
		LuaControls::sendControls( lua, controls );

		lua_rawgeti( lua, LUA_REGISTRYINDEX, updateReference );
		lua_pushnumber( lua, deltaTime );
		if( lua_pcall( lua, 1, 0, 0 ) )
		{
			std::cout << lua_tostring( lua, -1 ) << std::endl;
			validState = false;
		}
	}
}

void LuaBinds::printLuaTop() const
{
	std::cout << lua_gettop(lua) << "\n";
}

/*namespace LuaBinds
{
	void registerFunctions( lua_State* lua,
							GearEngine* gearEngine,
							Assets* assets,
							CollisionHandler* collisionHandler,
							Transform* transforms,
							std::vector<ModelInstance>* models )
	{
		LuaAssets::registerFunctions( lua, assets );
		LuaCollision::registerFunctions( lua, collisionHandler );
		LuaControls::registerFunctions( lua );
		LuaGear::registerFunctions( lua, gearEngine, models );
		LuaParticles::registerFunctions( lua );
		LuaTransform::registerFunctions( lua, transforms );
	}

	bool update( lua_State* lua, float deltaTime )
	{
		bool result = true;

		lua_getglobal( lua, "Update" );
		lua_pushnumber( lua, deltaTime );
		if( lua_pcall( lua, 1, 0, 0 ) )
		{
			std::cout << lua_tostring( lua, -1 ) << std::endl;
			result = false;
		}

		return result;
	}
}*/