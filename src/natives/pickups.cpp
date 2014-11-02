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
#include <boost/unordered_map.hpp>

#include <eigen3/Eigen/Core>

int Natives::CreateDynamicPickup(int modelid, int type, float x, float y, float z, int worldid, int interiorid, int playerid, float streamdistance)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_PICKUP) == core->getData()->pickups.size())
	{
		return 0;
	}
	int pickupID = Item::Pickup::identifier.get();
	Item::SharedPickup pickup(new Item::Pickup);
	pickup->amx = amx;
	pickup->pickupID = pickupID;
	pickup->worldID = 0;
	pickup->modelID = modelid;
	pickup->type = type;
	pickup->position = Eigen::Vector3f(x, y, z);
	Utility::addToContainer(pickup->worlds, worldid);
	Utility::addToContainer(pickup->interiors, interiorid);
	Utility::addToContainer(pickup->players, playerid);
	pickup->streamDistance = streamdistance * streamdistance;
	core->getGrid()->addPickup(pickup);
	core->getData()->pickups.insert(std::make_pair(pickupID, pickup));
	return pickupID;
}

bool Natives::DestroyDynamicPickup(int pickupid)
{
	boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(pickupid);
	if (p != core->getData()->pickups.end())
	{
		Utility::destroyPickup(p);
		return true;
	}
	return false;
}

bool Natives::IsValidDynamicPickup(int pickupid)
{
	boost::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(pickupid);
	if (p != core->getData()->pickups.end())
	{
		return true;
	}
	return false;
}
