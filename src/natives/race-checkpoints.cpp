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

int Natives::CreateDynamicRaceCP(int type, float x, float y, float z, float nextx, float nexty, float nextz, float size, int worldid, int interiorid, int playerid, float streamdistance)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_RACE_CP) == core->getData()->raceCheckpoints.size())
	{
		return 0;
	}
	int raceCheckpointID = Item::RaceCheckpoint::identifier.get();
	Item::SharedRaceCheckpoint raceCheckpoint(new Item::RaceCheckpoint);
	raceCheckpoint->raceCheckpointID = raceCheckpointID;
	raceCheckpoint->type = type;
	raceCheckpoint->position = Eigen::Vector3f(x, y, z);
	raceCheckpoint->next = Eigen::Vector3f(nextx, nexty, nextz);
	raceCheckpoint->size = size;
	Utility::addToContainer(raceCheckpoint->worlds, worldid);
	Utility::addToContainer(raceCheckpoint->interiors, interiorid);
	Utility::addToContainer(raceCheckpoint->players, playerid);
	raceCheckpoint->streamDistance = streamdistance * streamdistance;
	core->getGrid()->addRaceCheckpoint(raceCheckpoint);
	core->getData()->raceCheckpoints.insert(std::make_pair(raceCheckpointID, raceCheckpoint));
	return raceCheckpointID;
}

bool Natives::DestroyDynamicRaceCP(int checkpointid)
{
	boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(checkpointid);
	if (r != core->getData()->raceCheckpoints.end())
	{
		Utility::destroyRaceCheckpoint(r);
		return true;
	}
	return false;
}

bool Natives::IsValidDynamicRaceCP(int checkpointid)
{
	boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(checkpointid);
	if (r != core->getData()->raceCheckpoints.end())
	{
		return true;
	}
	return false;
}

bool Natives::TogglePlayerDynamicRaceCP(int playerid, int checkpointid, bool toggle)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		boost::unordered_set<int>::iterator d = p->second.disabledRaceCheckpoints.find(checkpointid);
		if (toggle)
		{
			if (d != p->second.disabledRaceCheckpoints.end())
			{
				p->second.disabledRaceCheckpoints.quick_erase(d);
				return true;
			}
		}
		else
		{
			if (d == p->second.disabledRaceCheckpoints.end())
			{
				if (p->second.visibleRaceCheckpoint == checkpointid)
				{
					DisablePlayerRaceCheckpoint(p->first);
					p->second.activeRaceCheckpoint = 0;
					p->second.visibleRaceCheckpoint = 0;
				}
				p->second.disabledRaceCheckpoints.insert(checkpointid);
				return true;
			}
		}
	}
	return false;
}

bool Natives::TogglePlayerAllDynamicRaceCPs(int playerid, bool toggle)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		p->second.disabledRaceCheckpoints.clear();
		if (!toggle)
		{
			if (p->second.visibleRaceCheckpoint != 0)
			{
				DisablePlayerRaceCheckpoint(p->first);
				p->second.activeRaceCheckpoint = 0;
				p->second.visibleRaceCheckpoint = 0;
			}
			for (boost::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.begin(); r != core->getData()->raceCheckpoints.end(); ++r)
			{
				p->second.disabledRaceCheckpoints.insert(r->first);
			}
		}
		return true;
	}
	return false;
}

bool Natives::IsPlayerInDynamicRaceCP(int playerid, int checkpointid)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		if (p->second.activeRaceCheckpoint == checkpointid)
		{
			return true;
		}
	}
	return false;
}

int Natives::GetPlayerVisibleDynamicRaceCP(int playerid)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		return p->second.visibleRaceCheckpoint;
	}
	return 0;
}
