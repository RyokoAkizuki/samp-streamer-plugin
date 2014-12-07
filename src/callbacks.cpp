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

#include "core.h"

#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <set>

#include "callbacks.hpp"

/**
 * Removed AMX interface handling. Call callbacks directly.
 * @author Yukino Hayakawa
 */

bool Streamer_OnPlayerConnect(int playerid)
{
	if (playerid >= 0 && playerid < MAX_PLAYERS)
	{
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p == core->getData()->players.end())
		{
			Player player(playerid);
			core->getData()->players.insert(std::make_pair(playerid, player));
		}
	}
	return true;
}

bool Streamer_OnPlayerDisconnect(int playerid, int /* reason */)
{
	core->getData()->players.erase(playerid);
	return true;
}

bool Streamer_OnPlayerEnterCheckpoint(int playerid)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		if (p->second.activeCheckpoint != p->second.visibleCheckpoint)
		{
			int checkpointid = p->second.visibleCheckpoint;
			p->second.activeCheckpoint = checkpointid;
			OnPlayerEnterDynamicCP(playerid, checkpointid);
		}
	}
	return true;
}

bool Streamer_OnPlayerLeaveCheckpoint(int playerid)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		if (p->second.activeCheckpoint == p->second.visibleCheckpoint)
		{
			int checkpointid = p->second.activeCheckpoint;
			p->second.activeCheckpoint = 0;
			OnPlayerLeaveDynamicCP(playerid, checkpointid);
		}
	}
	return true;
}

bool Streamer_OnPlayerEnterRaceCheckpoint(int playerid)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		if (p->second.activeRaceCheckpoint != p->second.visibleRaceCheckpoint)
		{
			int checkpointid = p->second.visibleRaceCheckpoint;
			p->second.activeRaceCheckpoint = checkpointid;
			OnPlayerEnterDynamicRaceCP(playerid, checkpointid);
		}
	}
	return true;
}

bool Streamer_OnPlayerLeaveRaceCheckpoint(int playerid)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		if (p->second.activeRaceCheckpoint == p->second.visibleRaceCheckpoint)
		{
			int checkpointid = p->second.activeRaceCheckpoint;
			p->second.activeRaceCheckpoint = 0;
			OnPlayerLeaveDynamicRaceCP(playerid, checkpointid);
		}
	}
	return true;
}

bool Streamer_OnPlayerPickUpPickup(int playerid, int pickupid)
{
	for (boost::unordered_map<int, int>::iterator i = core->getStreamer()->internalPickups.begin(); i != core->getStreamer()->internalPickups.end(); ++i)
	{
		if (i->second == pickupid)
		{
			int pickupid = i->first;
			OnPlayerPickUpDynamicPickup(playerid, pickupid);
			break;
		}
	}
	return true;
}

bool Streamer_OnPlayerEditObject(int playerid, bool playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ)
{
	if (playerobject)
	{
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			for (boost::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i)
			{
				if (i->second == objectid)
				{
					int objectid = i->first;
					OnPlayerEditDynamicObject(playerid, objectid, response, fX, fY, fZ, fRotX, fRotY, fRotZ);
					break;
				}
			}
		}
	}
	return true;
}

bool Streamer_OnPlayerSelectObject(int playerid, int type, int objectid, int modelid, float x, float y, float z)
{
	if (type == SELECT_OBJECT_PLAYER_OBJECT)
	{
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			for (boost::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i)
			{
				if (i->second == objectid)
				{
					int objectid = i->first;
					OnPlayerSelectDynamicObject(playerid, objectid, modelid, x, y, z);
					break;
				}
			}
		}
	}
	return true;
}

bool Streamer_OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float x, float y, float z)
{
	if (hittype == BULLET_HIT_TYPE_PLAYER_OBJECT)
	{
		boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
		if (p != core->getData()->players.end())
		{
			for (boost::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i)
			{
				if (i->second == hitid)
				{
					int objectid = i->first;
					OnPlayerShootDynamicObject(playerid, weaponid, objectid, x, y, z);
					break;
				}
			}
		}
	}
	return true;
}
