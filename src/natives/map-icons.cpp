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
#include <boost/unordered_set.hpp>

#include <eigen3/Eigen/Core>

int Natives::CreateDynamicMapIcon(float x, float y, float z, int type, int color, int worldid, int interiorid, int playerid, float streamdistance, int style)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_MAP_ICON) == core->getData()->mapIcons.size())
	{
		return 0;
	}
	int mapIconID = Item::MapIcon::identifier.get();
	Item::SharedMapIcon mapIcon(new Item::MapIcon);
	mapIcon->amx = amx;
	mapIcon->mapIconID = mapIconID;
	mapIcon->position = Eigen::Vector3f(x, y, z);
	mapIcon->type = type;
	mapIcon->color = color;
	Utility::addToContainer(mapIcon->worlds, worldid);
	Utility::addToContainer(mapIcon->interiors, interiorid);
	Utility::addToContainer(mapIcon->players, playerid);
	mapIcon->streamDistance = streamdistance * streamdistance;
	mapIcon->style = style;
	core->getGrid()->addMapIcon(mapIcon);
	core->getData()->mapIcons.insert(std::make_pair(mapIconID, mapIcon));
	return mapIconID;
}

bool Natives::DestroyDynamicMapIcon(int iconid)
{
	boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(iconid);
	if (m != core->getData()->mapIcons.end())
	{
		Utility::destroyMapIcon(m);
		return true;
	}
	return false;
}

bool Natives::IsValidDynamicMapIcon(int iconid)
{
	boost::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(iconid);
	if (m != core->getData()->mapIcons.end())
	{
		return true;
	}
	return false;
}
