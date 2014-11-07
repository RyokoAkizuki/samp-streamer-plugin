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

#ifndef NATIVES_H
#define NATIVES_H

#include <vector>
#include <limits>

#include "common.h"

namespace Natives
{
	// Settings
	int  Streamer_GetTickRate();
	bool Streamer_SetTickRate(std::size_t rate);
	std::size_t Streamer_GetMaxItems(int type);
	bool Streamer_SetMaxItems(int type, int items);
	std::size_t Streamer_GetVisibleItems(int type);
	bool Streamer_SetVisibleItems(int type, int items);
	float Streamer_GetCellDistance();
	void Streamer_SetCellDistance(float streamdistance);
	float Streamer_GetCellSize();
	void Streamer_SetCellSize(float size);
	// Updates
	void Streamer_ProcessActiveItems();
	bool Streamer_ToggleIdleUpdate(int playerid, bool toggle);
	bool Streamer_IsToggleIdleUpdate(int playerid);
	bool Streamer_ToggleItemUpdate(int playerid, std::size_t type, bool toggle);
	bool Streamer_IsToggleItemUpdate(int playerid, std::size_t type);
	bool Streamer_Update(int playerid);
	bool Streamer_UpdateEx(int playerid, float x, float y, float z, int worldid = -1, int interiorid = -1);
	// Objects
	int  CreateDynamicObject(int modelid, float x, float y, float z, float rx, float ry, float rz, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0, float drawdistance = 0.0);
	bool DestroyDynamicObject(int objectid);
	bool IsValidDynamicObject(int objectid);
	bool SetDynamicObjectPos(int objectid, float x, float y, float z);
	bool GetDynamicObjectPos(int objectid, float& x, float& y, float& z);
	bool SetDynamicObjectRot(int objectid, float rx, float ry, float rz);
	bool GetDynamicObjectRot(int objectid, float& rx, float& ry, float& rz);
	int  MoveDynamicObject(int objectid, float x, float y, float z, float speed, float rx = -1000.0, float ry = -1000.0, float rz = -1000.0);
	bool StopDynamicObject(int objectid);
	bool IsDynamicObjectMoving(int objectid);
	bool AttachCameraToDynamicObject(int playerid, int objectid);
	bool AttachDynamicObjectToVehicle(int objectid, int vehicleid, float offsetx, float offsety, float offsetz, float rx, float ry, float rz);
	bool EditDynamicObject(int playerid, int objectid);
	bool GetDynamicObjectMaterial(int objectid, int materialindex, int& modelid, std::string& txdname, std::string& texturename, int& materialcolor);
	bool SetDynamicObjectMaterial(int objectid, int materialindex, int modelid, const std::string& txdname, const std::string& texturename, int materialcolor = 0);
	bool GetDynamicObjectMaterialText(int objectid, int materialindex, std::string& text, int& materialsize, std::string& fontface, int& fontsize, bool& bold, int& fontcolor, int& backcolor, int& textalignment);
	bool SetDynamicObjectMaterialText(int objectid, int materialindex, const std::string& text, int materialsize = OBJECT_MATERIAL_SIZE_256x128, const std::string& fontface = "Arial", int fontsize = 24, bool bold = true, int fontcolor = 0xFFFFFFFF, int backcolor = 0, int textalignment = 0);
	// Pickups
	int  CreateDynamicPickup(int modelid, int type, float x, float y, float z, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0);
	bool DestroyDynamicPickup(int pickupid);
	bool IsValidDynamicPickup(int pickupid);
	// Checkpoints
	int  CreateDynamicCP(float x, float y, float z, float size, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0);
	bool DestroyDynamicCP(int checkpointid);
	bool IsValidDynamicCP(int checkpointid);
	bool TogglePlayerDynamicCP(int playerid, int checkpointid, bool toggle);
	bool TogglePlayerAllDynamicCPs(int playerid, bool toggle);
	bool IsPlayerInDynamicCP(int playerid, int checkpointid);
	int  GetPlayerVisibleDynamicCP(int playerid);
	// Race Checkpoints
	int  CreateDynamicRaceCP(int type, float x, float y, float z, float nextx, float nexty, float nextz, float size, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0);
	bool DestroyDynamicRaceCP(int checkpointid);
	bool IsValidDynamicRaceCP(int checkpointid);
	bool TogglePlayerDynamicRaceCP(int playerid, int checkpointid, bool toggle);
	bool TogglePlayerAllDynamicRaceCPs(int playerid, bool toggle);
	bool IsPlayerInDynamicRaceCP(int playerid, int checkpointid);
	int  GetPlayerVisibleDynamicRaceCP(int playerid);
	// Map Icons
	int  CreateDynamicMapIcon(float x, float y, float z, int type, int color, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0, int style = MAPICON_LOCAL);
	bool DestroyDynamicMapIcon(int iconid);
	bool IsValidDynamicMapIcon(int iconid);
	// 3D Text Labels
	int  CreateDynamic3DTextLabel(const std::string& text, int color, float x, float y, float z, float drawdistance, int attachedplayer = INVALID_PLAYER_ID, int attachedvehicle = INVALID_VEHICLE_ID, bool testlos = false, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0);
	bool DestroyDynamic3DTextLabel(int id);
	bool IsValidDynamic3DTextLabel(int id);
	bool GetDynamic3DTextLabelText(int id, std::string& text);
	bool UpdateDynamic3DTextLabelText(int id, int color, const std::string& text);
	// Areas
	int  CreateDynamicCircle(float x, float y, float size, int worldid = -1, int interiorid = -1, int playerid = -1);
	int  CreateDynamicCylinder(float x, float y, float minz, float maxz, float size, int worldid = -1, int interiorid = -1, int playerid = -1);
	int  CreateDynamicSphere(float x, float y, float z, float size, int worldid = -1, int interiorid = -1, int playerid = -1);
	int  CreateDynamicRectangle(float minx, float miny, float maxx, float maxy, int worldid = -1, int interiorid = -1, int playerid = -1);
	int  CreateDynamicCuboid(float minx, float miny, float minz, float maxx, float maxy, float maxz, int worldid = -1, int interiorid = -1, int playerid = -1);
	int  CreateDynamicPolygon(const std::vector<float>& points, float minz = std::numeric_limits<float>::min(), float maxz = std::numeric_limits<float>::max(), int worldid = -1, int interiorid = -1, int playerid = -1);
	bool DestroyDynamicArea(int areaid);
	bool IsValidDynamicArea(int areaid);
	bool TogglePlayerDynamicArea(int playerid, int areaid, bool toggle);
	bool TogglePlayerAllDynamicAreas(int playerid, bool toggle);
	bool IsPlayerInDynamicArea(int playerid, int areaid, bool recheck = false);
	bool IsPlayerInAnyDynamicArea(int playerid, bool recheck = false);
	bool IsAnyPlayerInDynamicArea(int areaid, bool recheck = false);
	bool IsAnyPlayerInAnyDynamicArea(bool recheck = false);
	bool IsPointInDynamicArea(int areaid, float x, float y, float z);
	bool IsPointInAnyDynamicArea(float x, float y, float z);
	bool AttachDynamicAreaToObject(int areaid, int objectid, int type = STREAMER_OBJECT_TYPE_DYNAMIC, int playerid = INVALID_PLAYER_ID);
	bool AttachDynamicAreaToPlayer(int areaid, int playerid);
	bool AttachDynamicAreaToVehicle(int areaid, int vehicleid);
}

#endif
