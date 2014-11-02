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

int Natives::CreateDynamicCP(float x, float y, float z, float size, int worldid, int interiorid, int playerid, float streamdistance)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_CP) == core->getData()->checkpoints.size())
	{
		return 0;
	}
	int checkpointID = Item::Checkpoint::identifier.get();
	Item::SharedCheckpoint checkpoint(new Item::Checkpoint);
	checkpoint->amx = amx;
	checkpoint->checkpointID = checkpointID;
	checkpoint->position = Eigen::Vector3f(x, y, z);
	checkpoint->size = size;
	Utility::addToContainer(checkpoint->worlds, worldid);
	Utility::addToContainer(checkpoint->interiors, interiorid);
	Utility::addToContainer(checkpoint->players, playerid);
	checkpoint->streamDistance = streamdistance * streamdistance;
	core->getGrid()->addCheckpoint(checkpoint);
	core->getData()->checkpoints.insert(std::make_pair(checkpointID, checkpoint));
	return checkpointID;
}

bool Natives::DestroyDynamicCP(int checkpointid)
{
	boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(checkpointid);
	if (c != core->getData()->checkpoints.end())
	{
		Utility::destroyCheckpoint(c);
		return true;
	}
	return false;
}

bool Natives::IsValidDynamicCP(int checkpointid)
{
	boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(checkpointid);
	if (c != core->getData()->checkpoints.end())
	{
		return true;
	}
	return false;
}

bool Natives::TogglePlayerDynamicCP(int playerid, int checkpointid, bool toggle)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		boost::unordered_set<int>::iterator d = p->second.disabledCheckpoints.find(checkpointid);
		if (toggle)
		{
			if (d != p->second.disabledCheckpoints.end())
			{
				p->second.disabledCheckpoints.quick_erase(d);
				return true;
			}
		}
		else
		{
			if (d == p->second.disabledCheckpoints.end())
			{
				if (p->second.visibleCheckpoint == checkpointid)
				{
					DisablePlayerCheckpoint(p->first);
					p->second.activeCheckpoint = 0;
					p->second.visibleCheckpoint = 0;
				}
				p->second.disabledCheckpoints.insert(checkpointid);
				return true;
			}
		}
	}
	return false;
}

bool Natives::TogglePlayerAllDynamicCPs(int playerid, bool toggle)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		p->second.disabledCheckpoints.clear();
		if (!toggle)
		{
			if (p->second.visibleCheckpoint != 0)
			{
				DisablePlayerCheckpoint(p->first);
				p->second.activeCheckpoint = 0;
				p->second.visibleCheckpoint = 0;
			}
			for (boost::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.begin(); c != core->getData()->checkpoints.end(); ++c)
			{
				p->second.disabledCheckpoints.insert(c->first);
			}
		}
		return true;
	}
	return false;
}

bool Natives::IsPlayerInDynamicCP(int playerid, int checkpointid)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		if (p->second.activeCheckpoint == checkpointid)
		{
			return true;
		}
	}
	return false;
}

int Natives::GetPlayerVisibleDynamicCP(int playerid)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		return p->second.visibleCheckpoint;
	}
	return 0;
}
