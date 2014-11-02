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

int Natives::CreateDynamicCircle(float x, float y, float size, int worldid, int interiorid, int playerid)
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
	area->position = Eigen::Vector2f(x, y);
	area->size = size * size;
	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players, playerid);
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

int Natives::CreateDynamicCylinder(float x, float y, float minz, float maxz, float size, int worldid, int interiorid, int playerid)
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
	area->position = Eigen::Vector2f(x, y);
	area->height = Eigen::Vector2f(minz, maxz);
	area->size = size * size;
	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players, playerid);
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

int Natives::CreateDynamicSphere(float x, float y, float z, float size, int worldid, int interiorid, int playerid)
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
	area->position = Eigen::Vector3f(x, y, z);
	area->size = size * size;
	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players, playerid);
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

int Natives::CreateDynamicRectangle(float minx, float miny, float maxx, float maxy,, int worldid, int interiorid, int playerid)
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
	area->position = Box2D(Eigen::Vector2f(minx, miny), Eigen::Vector2f(maxx, maxy));
	boost::geometry::correct(boost::get<Box2D>(area->position));
	area->size = static_cast<float>(boost::geometry::comparable_distance(boost::get<Box2D>(area->position).min_corner(), boost::get<Box2D>(area->position).max_corner()));
	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players, playerid);
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

int Natives::CreateDynamicCuboid(float minx, float miny, float minz, float maxx, float maxy, float maxz, int worldid, int interiorid, int playerid)
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
	area->position = Box3D(Eigen::Vector3f(minx, miny, minz), Eigen::Vector3f(maxx, maxy, maxz));
	boost::geometry::correct(boost::get<Box3D>(area->position));
	area->size = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(boost::get<Box3D>(area->position).min_corner()[0], boost::get<Box3D>(area->position).min_corner()[1]), Eigen::Vector2f(boost::get<Box3D>(area->position).max_corner()[0], boost::get<Box3D>(area->position).max_corner()[1])));
	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players, playerid);
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

int Natives::CreateDynamicPolygon(const std::vector<float>& points, float minz = -FLOAT_INFINITY, float maxz = FLOAT_INFINITY, int worldid, int interiorid, int playerid)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
	{
		return 0;
	}
	if (points.size() >= 2 && points.size() % 2 > 0)
	{
		sampgdk::logprintf("*** CreateDynamicPolygon: Number of points must be divisible by two");
		return 0;
	}
	int areaID = Item::Area::identifier.get();
	Item::SharedArea area(new Item::Area);
	area->amx = amx;
	area->areaID = areaID;
	area->type = STREAMER_AREA_TYPE_POLYGON;
	Utility::convertArrayToPolygon(points, boost::get<Polygon2D>(area->position));
	area->height = Eigen::Vector2f(minz, maxz);
	Box2D box = boost::geometry::return_envelope<Box2D>(boost::get<Polygon2D>(area->position));
	area->size = static_cast<float>(boost::geometry::comparable_distance(box.min_corner(), box.max_corner()));
	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players, playerid);
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

bool Natives::DestroyDynamicArea(int areaid)
{
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
	if (a != core->getData()->areas.end())
	{
		Utility::destroyArea(a);
		return true;
	}
	return false;
}

bool Natives::IsValidDynamicArea(int areaid)
{
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
	if (a != core->getData()->areas.end())
	{
		return true;
	}
	return false;
}

bool Natives::TogglePlayerDynamicArea(int playerid, int areaid, bool toggle)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		boost::unordered_set<int>::iterator d = p->second.disabledAreas.find(areaid);
		if (toggle)
		{
			if (d != p->second.disabledAreas.end())
			{
				p->second.disabledAreas.quick_erase(d);
				return true;
			}
		}
		else
		{
			if (d == p->second.disabledAreas.end())
			{
				p->second.disabledAreas.insert(areaid);
				p->second.internalAreas.erase(areaid);
				return true;
			}
		}
	}
	return false;
}

bool Natives::TogglePlayerAllDynamicAreas(int playerid, bool toggle)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		p->second.disabledAreas.clear();
		if (!toggle)
		{
			for (boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
			{
				p->second.disabledAreas.insert(a->first);
			}
			p->second.internalAreas.clear();
		}
		return true;
	}
	return false;
}

bool Natives::IsPlayerInDynamicArea(int playerid, int areaid, bool recheck)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		if (!recheck)
		{
			boost::unordered_set<int>::iterator i = p->second.internalAreas.find(areaid);
			if (i != p->second.internalAreas.end())
			{
				return true;
			}
		}
		else
		{
			boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
			return Utility::isPointInArea(p->second.position, a->second) != 0;
		}
	}
	return false;
}

bool Natives::IsPlayerInAnyDynamicArea(int playerid, bool recheck)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		if (!recheck)
		{
			if (!p->second.internalAreas.empty())
			{
				return true;
			}
		}
		else
		{
			for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
			{
				if (Utility::isPointInArea(p->second.position, a->second))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Natives::IsAnyPlayerInDynamicArea(int areaid, bool recheck)
{
	for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		if (!recheck)
		{
			boost::unordered_set<int>::iterator i = p->second.internalAreas.find(areaid);
			if (i != p->second.internalAreas.end())
			{
				return 1;
			}
		}
		else
		{
			boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
			return Utility::isPointInArea(p->second.position, a->second) != 0;
		}
	}
	return 0;
}

bool Natives::IsAnyPlayerInAnyDynamicArea(bool recheck)
{
	for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		if (!recheck)
		{
			if (!p->second.internalAreas.empty())
			{
				return true;
			}
		}
		else
		{
			for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
			{
				if (Utility::isPointInArea(p->second.position, a->second))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Natives::IsPointInDynamicArea(int areaid, float x, float y, float z)
{
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
	if (a != core->getData()->areas.end())
	{
		return Utility::isPointInArea(Eigen::Vector3f(x, y, z), a->second);
	}
	return false;
}

bool Natives::IsPointInAnyDynamicArea(float x, float y, float z)
{
	for (boost::unordered_map<int, Item::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
	{
		if (Utility::isPointInArea(Eigen::Vector3f(x, y, z), a->second))
		{
			return true;
		}
	}
	return false;
}

bool Natives::AttachDynamicAreaToObject(int areaid, int objectid, int type, int playerid)
{
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
	if (a != core->getData()->areas.end())
	{
		if (a->second->type != STREAMER_AREA_TYPE_CIRCLE && a->second->type != STREAMER_AREA_TYPE_SPHERE)
		{
			sampgdk::logprintf("*** AttachDynamicAreaToObject: Only circles and spheres may be attached to objects");
			return false;
		}
		if (objectid != INVALID_GENERIC_ID)
		{
			a->second->attach = boost::intrusive_ptr<Item::Area::Attach>(new Item::Area::Attach);
			a->second->attach->object = boost::make_tuple(objectid, type, playerid);
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
		return true;
	}
	return false;
}

bool Natives::AttachDynamicAreaToPlayer(int areaid, int playerid)
{
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
	if (a != core->getData()->areas.end())
	{
		if (a->second->type != STREAMER_AREA_TYPE_CIRCLE && a->second->type != STREAMER_AREA_TYPE_SPHERE)
		{
			sampgdk::logprintf("*** AttachDynamicAreaToPlayer: Only circles and spheres may be attached to players");
			return false;
		}
		if (playerid != INVALID_GENERIC_ID)
		{
			a->second->attach = boost::intrusive_ptr<Item::Area::Attach>(new Item::Area::Attach);
			a->second->attach->object.get<0>() = INVALID_GENERIC_ID;
			a->second->attach->player = playerid;
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
		return true;
	}
	return false;
}

int Natives::AttachDynamicAreaToVehicle(int areaid, int vehicleid)
{
	boost::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaid);
	if (a != core->getData()->areas.end())
	{
		if (a->second->type != STREAMER_AREA_TYPE_CIRCLE && a->second->type != STREAMER_AREA_TYPE_SPHERE)
		{
			sampgdk::logprintf("*** AttachDynamicAreaToVehicle: Only circles and spheres may be attached to vehicles");
			return false;
		}
		if (vehicleid != INVALID_GENERIC_ID)
		{
			a->second->attach = boost::intrusive_ptr<Item::Area::Attach>(new Item::Area::Attach);
			a->second->attach->object.get<0>() = INVALID_GENERIC_ID;
			a->second->attach->player = INVALID_GENERIC_ID;
			a->second->attach->vehicle = vehicleid;
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
		return true;
	}
	return false;
}
