/*
 * Copyright (C) 2014 Incognito
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../natives.h"

#include "../core.h"
#include "../main.h"
#include "../utility.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/variant.hpp>

#include <eigen3/Eigen/Core>

int Natives::CreateDynamicCircle(AMX *amx, cell *params)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
	{
		return 0;
	}
	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	area->amx = amx;
	area->areaID = areaID;
	area->type = STREAMER_AREA_TYPE_CIRCLE;
	area->position = Eigen::Vector2f(amx_ctof(params[1]), amx_ctof(params[2]));
	area->size = amx_ctof(params[3]) * amx_ctof(params[3]);
	Utility::addToContainer(area->worlds, static_cast<int>(params[4]));
	Utility::addToContainer(area->interiors, static_cast<int>(params[5]));
	Utility::addToContainer(area->players, static_cast<int>(params[6]));
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return static_cast<cell>(areaID);
}

int Natives::CreateDynamicCylinder(AMX *amx, cell *params)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
	{
		return 0;
	}
	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	area->amx = amx;
	area->areaID = areaID;
	area->type = STREAMER_AREA_TYPE_CYLINDER;
	area->position = Eigen::Vector2f(amx_ctof(params[1]), amx_ctof(params[2]));
	area->height = Eigen::Vector2f(amx_ctof(params[3]), amx_ctof(params[4]));
	area->size = amx_ctof(params[5]) * amx_ctof(params[5]);
	Utility::addToContainer(area->worlds, static_cast<int>(params[6]));
	Utility::addToContainer(area->interiors, static_cast<int>(params[7]));
	Utility::addToContainer(area->players, static_cast<int>(params[8]));
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return static_cast<cell>(areaID);
}

int Natives::CreateDynamicSphere(AMX *amx, cell *params)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
	{
		return 0;
	}
	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	area->amx = amx;
	area->areaID = areaID;
	area->type = STREAMER_AREA_TYPE_SPHERE;
	area->position = Eigen::Vector3f(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]));
	area->size = amx_ctof(params[4]) * amx_ctof(params[4]);
	Utility::addToContainer(area->worlds, static_cast<int>(params[5]));
	Utility::addToContainer(area->interiors, static_cast<int>(params[6]));
	Utility::addToContainer(area->players, static_cast<int>(params[7]));
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return static_cast<cell>(areaID);
}

int Natives::CreateDynamicRectangle(AMX *amx, cell *params)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
	{
		return 0;
	}
	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	area->amx = amx;
	area->areaID = areaID;
	area->type = STREAMER_AREA_TYPE_RECTANGLE;
	area->position = Box2D(Eigen::Vector2f(amx_ctof(params[1]), amx_ctof(params[2])), Eigen::Vector2f(amx_ctof(params[3]), amx_ctof(params[4])));
	boost::geometry::correct(boost::get<Box2D>(area->position));
	area->size = static_cast<float>(boost::geometry::comparable_distance(boost::get<Box2D>(area->position).min_corner(), boost::get<Box2D>(area->position).max_corner()));
	Utility::addToContainer(area->worlds, static_cast<int>(params[5]));
	Utility::addToContainer(area->interiors, static_cast<int>(params[6]));
	Utility::addToContainer(area->players, static_cast<int>(params[7]));
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return static_cast<cell>(areaID);
}

int Natives::CreateDynamicCuboid(AMX *amx, cell *params)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
	{
		return 0;
	}
	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	area->amx = amx;
	area->areaID = areaID;
	area->type = STREAMER_AREA_TYPE_CUBOID;
	area->position = Box3D(Eigen::Vector3f(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3])), Eigen::Vector3f(amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6])));
	boost::geometry::correct(boost::get<Box3D>(area->position));
	area->size = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(boost::get<Box3D>(area->position).min_corner()[0], boost::get<Box3D>(area->position).min_corner()[1]), Eigen::Vector2f(boost::get<Box3D>(area->position).max_corner()[0], boost::get<Box3D>(area->position).max_corner()[1])));
	Utility::addToContainer(area->worlds, static_cast<int>(params[7]));
	Utility::addToContainer(area->interiors, static_cast<int>(params[8]));
	Utility::addToContainer(area->players, static_cast<int>(params[9]));
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return static_cast<cell>(areaID);
}

int Natives::CreateDynamicPolygon(AMX *amx, cell *params)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
	{
		return 0;
	}
	if (static_cast<int>(params[4] >= 2 && static_cast<int>(params[4]) % 2))
	{
		sampgdk::logprintf("*** CreateDynamicPolygon: Number of points must be divisible by two");
		return 0;
	}
	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	area->amx = amx;
	area->areaID = areaID;
	area->type = STREAMER_AREA_TYPE_POLYGON;
	Utility::convertArrayToPolygon(amx, params[1], params[4], boost::get<Polygon2D>(area->position));
	area->height = Eigen::Vector2f(amx_ctof(params[2]), amx_ctof(params[3]));
	Box2D box = boost::geometry::return_envelope<Box2D>(boost::get<Polygon2D>(area->position));
	area->size = static_cast<float>(boost::geometry::comparable_distance(box.min_corner(), box.max_corner()));
	Utility::addToContainer(area->worlds, static_cast<int>(params[5]));
	Utility::addToContainer(area->interiors, static_cast<int>(params[6]));
	Utility::addToContainer(area->players, static_cast<int>(params[7]));
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return static_cast<cell>(areaID);
}

int Natives::DestroyDynamicArea(AMX *amx, cell *params)
{
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(static_cast<int>(params[1]));
	if (a != core->getData()->areas.end())
	{
		Utility::destroyArea(a);
		return 1;
	}
	return 0;
}

int Natives::IsValidDynamicArea(AMX *amx, cell *params)
{
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(static_cast<int>(params[1]));
	if (a != core->getData()->areas.end())
	{
		return 1;
	}
	return 0;
}

int Natives::GetDynamicPolygonPoints(AMX *amx, cell *params)
{
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(static_cast<int>(params[1]));
	if (a != core->getData()->areas.end())
	{
		return static_cast<cell>(Utility::convertPolygonToArray(amx, params[2], params[3], boost::get<Polygon2D>(a->second->position)) != 0);
	}
	return 0;
}

int Natives::GetDynamicPolygonNumberPoints(AMX *amx, cell *params)
{
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(static_cast<int>(params[1]));
	if (a != core->getData()->areas.end())
	{
		return static_cast<cell>(boost::get<Polygon2D>(a->second->position).outer().size());
	}
	return 0;
}

int Natives::TogglePlayerDynamicArea(AMX *amx, cell *params)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		boost::unordered_set<int>::iterator d = p->second.disabledAreas.find(static_cast<int>(params[2]));
		if (static_cast<int>(params[3]))
		{
			if (d != p->second.disabledAreas.end())
			{
				p->second.disabledAreas.quick_erase(d);
				return 1;
			}
		}
		else
		{
			if (d == p->second.disabledAreas.end())
			{
				p->second.disabledAreas.insert(static_cast<int>(params[2]));
				p->second.internalAreas.erase(static_cast<int>(params[2]));
				return 1;
			}
		}
	}
	return 0;
}

int Natives::TogglePlayerAllDynamicAreas(AMX *amx, cell *params)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		p->second.disabledAreas.clear();
		if (!static_cast<int>(params[2]))
		{
			for (boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
			{
				p->second.disabledAreas.insert(a->first);
			}
			p->second.internalAreas.clear();
		}
		return 1;
	}
	return 0;
}

int Natives::IsPlayerInDynamicArea(AMX *amx, cell *params)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		bool recheck = static_cast<int>(params[3]) != 0;
		if (!recheck)
		{
			boost::unordered_set<int>::iterator i = p->second.internalAreas.find(static_cast<int>(params[2]));
			if (i != p->second.internalAreas.end())
			{
				return 1;
			}
		}
		else
		{
			boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(static_cast<int>(params[2]));
			return static_cast<cell>(Utility::isPointInArea(p->second.position, a->second)) != 0;
		}
	}
	return 0;
}

int Natives::IsPlayerInAnyDynamicArea(AMX *amx, cell *params)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		bool recheck = static_cast<int>(params[2]) != 0;
		if (!recheck)
		{
			if (!p->second.internalAreas.empty())
			{
				return 1;
			}
		}
		else
		{
			for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
			{
				if (Utility::isPointInArea(p->second.position, a->second))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

int Natives::IsAnyPlayerInDynamicArea(AMX *amx, cell *params)
{
	for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		bool recheck = static_cast<int>(params[2]) != 0;
		if (!recheck)
		{
			boost::unordered_set<int>::iterator i = p->second.internalAreas.find(static_cast<int>(params[1]));
			if (i != p->second.internalAreas.end())
			{
				return 1;
			}
		}
		else
		{
			boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(static_cast<int>(params[1]));
			return static_cast<cell>(Utility::isPointInArea(p->second.position, a->second)) != 0;
		}
	}
	return 0;
}

int Natives::IsAnyPlayerInAnyDynamicArea(AMX *amx, cell *params)
{
	for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		bool recheck = static_cast<int>(params[1]) != 0;
		if (!recheck)
		{
			if (!p->second.internalAreas.empty())
			{
				return 1;
			}
		}
		else
		{
			for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
			{
				if (Utility::isPointInArea(p->second.position, a->second))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

int Natives::IsPointInDynamicArea(AMX *amx, cell *params)
{
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(static_cast<int>(params[1]));
	if (a != core->getData()->areas.end())
	{
		return Utility::isPointInArea(Eigen::Vector3f(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4])), a->second);
	}
	return 0;
}

int Natives::IsPointInAnyDynamicArea(AMX *amx, cell *params)
{
	for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
	{
		if (Utility::isPointInArea(Eigen::Vector3f(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3])), a->second))
		{
			return 1;
		}
	}
	return 0;
}

int Natives::GetPlayerDynamicAreas(AMX *amx, cell *params)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		return static_cast<cell>(Utility::convertContainerToArray(amx, params[2], params[3], p->second.internalAreas) != 0);
	}
	return 0;
}

int Natives::GetPlayerNumberDynamicAreas(AMX *amx, cell *params)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		return static_cast<cell>(p->second.internalAreas.size());
	}
	return 0;
}

int Natives::AttachDynamicAreaToObject(AMX *amx, cell *params)
{
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(static_cast<int>(params[1]));
	if (a != core->getData()->areas.end())
	{
		if (a->second->type != STREAMER_AREA_TYPE_CIRCLE && a->second->type != STREAMER_AREA_TYPE_SPHERE)
		{
			sampgdk::logprintf("*** AttachDynamicAreaToObject: Only circles and spheres may be attached to objects");
			return 0;
		}
		if (static_cast<int>(params[2]) != INVALID_GENERIC_ID)
		{
			a->second->attach = boost::intrusive_ptr<Item::Area::Attach>(new Item::Area::Attach);
			a->second->attach->object = boost::make_tuple(static_cast<int>(params[2]), static_cast<int>(params[3]), static_cast<int>(params[4]));
			a->second->attach->player = INVALID_GENERIC_ID;
			a->second->attach->vehicle = INVALID_GENERIC_ID;
			core->getStreamer()->attachedAreas.insert(a->second);
		}
		else
		{
			if (a->second->attach)
			{
				if (a->second->attach->object.get<0>() != INVALID_GENERIC_ID)
				{
					a->second->attach.reset();
					core->getStreamer()->attachedAreas.erase(a->second);
					core->getGrid()->removeArea(a->second, true);
				}
			}
		}
		return 1;
	}
	return 0;
}

int Natives::AttachDynamicAreaToPlayer(AMX *amx, cell *params)
{
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(static_cast<int>(params[1]));
	if (a != core->getData()->areas.end())
	{
		if (a->second->type != STREAMER_AREA_TYPE_CIRCLE && a->second->type != STREAMER_AREA_TYPE_SPHERE)
		{
			sampgdk::logprintf("*** AttachDynamicAreaToPlayer: Only circles and spheres may be attached to players");
			return 0;
		}
		if (static_cast<int>(params[2]) != INVALID_GENERIC_ID)
		{
			a->second->attach = boost::intrusive_ptr<Item::Area::Attach>(new Item::Area::Attach);
			a->second->attach->object.get<0>() = INVALID_GENERIC_ID;
			a->second->attach->player = static_cast<int>(params[2]);
			a->second->attach->vehicle = INVALID_GENERIC_ID;
			core->getStreamer()->attachedAreas.insert(a->second);
		}
		else
		{
			if (a->second->attach)
			{
				if (a->second->attach->player != INVALID_GENERIC_ID)
				{
					a->second->attach.reset();
					core->getStreamer()->attachedAreas.erase(a->second);
					core->getGrid()->removeArea(a->second, true);
				}
			}
		}
		return 1;
	}
	return 0;
}

int Natives::AttachDynamicAreaToVehicle(AMX *amx, cell *params)
{
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(static_cast<int>(params[1]));
	if (a != core->getData()->areas.end())
	{
		if (a->second->type != STREAMER_AREA_TYPE_CIRCLE && a->second->type != STREAMER_AREA_TYPE_SPHERE)
		{
			sampgdk::logprintf("*** AttachDynamicAreaToVehicle: Only circles and spheres may be attached to vehicles");
			return 0;
		}
		if (static_cast<int>(params[2]) != INVALID_GENERIC_ID)
		{
			a->second->attach = boost::intrusive_ptr<Item::Area::Attach>(new Item::Area::Attach);
			a->second->attach->object.get<0>() = INVALID_GENERIC_ID;
			a->second->attach->player = INVALID_GENERIC_ID;
			a->second->attach->vehicle = static_cast<int>(params[2]);
			core->getStreamer()->attachedAreas.insert(a->second);
		}
		else
		{
			if (a->second->attach)
			{
				if (a->second->attach->vehicle != INVALID_GENERIC_ID)
				{
					a->second->attach.reset();
					core->getStreamer()->attachedAreas.erase(a->second);
					core->getGrid()->removeArea(a->second, true);
				}
			}
		}
		return 1;
	}
	return 0;
}
