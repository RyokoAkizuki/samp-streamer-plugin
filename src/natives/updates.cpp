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

#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <eigen3/Eigen/Core>

#include <bitset>

void Natives::Streamer_ProcessActiveItems()
{
	core->getStreamer()->processActiveItems();
}

bool Natives::Streamer_ToggleIdleUpdate(int playerid, bool toggle)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		p->second.updateWhenIdle = toggle;
		return true;
	}
	return false;
}

bool Natives::Streamer_IsToggleIdleUpdate(int playerid)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		return p->second.updateWhenIdle;
	}
	return false;
}

bool Natives::Streamer_ToggleItemUpdate(int playerid, std::size_t type, bool toggle)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		if (type < STREAMER_MAX_TYPES)
		{
			p->second.enabledItems.set(type, toggle);
			return false;
		}
	}
	return true;
}

bool Natives::Streamer_IsToggleItemUpdate(int playerid, std::size_t type)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		if (type < STREAMER_MAX_TYPES)
		{
			return p->second.enabledItems.test(type);
		}
	}
	return false;
}

bool Natives::Streamer_Update(int playerid)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		core->getStreamer()->startManualUpdate(p->second, true);
		return true;
	}
	return false;
}

bool Natives::Streamer_UpdateEx(int playerid, float x, float y, float z, int worldid, int interiorid)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		p->second.position = Eigen::Vector3f(x, y, z);
		if (worldid >= 0)
		{
			p->second.worldID = worldid;
		}
		else
		{
			p->second.worldID = GetPlayerVirtualWorld(p->first);
		}
		if (interiorid >= 0)
		{
			p->second.interiorID = interiorid;
		}
		else
		{
			p->second.interiorID = GetPlayerInterior(p->first);
		}
		core->getStreamer()->startManualUpdate(p->second, false);
		return true;
	}
	return false;
}
