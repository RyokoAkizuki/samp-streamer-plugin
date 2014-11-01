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

#include "common.h"

namespace Natives
{
	// Settings
	int Streamer_GetTickRate();
	int Streamer_SetTickRate();
	int Streamer_GetMaxItems();
	int Streamer_SetMaxItems();
	int Streamer_GetVisibleItems();
	int Streamer_SetVisibleItems();
	int Streamer_GetCellDistance();
	int Streamer_SetCellDistance();
	int Streamer_GetCellSize();
	int Streamer_SetCellSize();
	// Updates
	int Streamer_ProcessActiveItems();
	int Streamer_ToggleIdleUpdate();
	int Streamer_IsToggleIdleUpdate();
	int Streamer_ToggleItemUpdate();
	int Streamer_IsToggleItemUpdate();
	int Streamer_Update();
	int Streamer_UpdateEx();
	// Data Manipulation
	int Streamer_GetFloatData();
	int Streamer_SetFloatData();
	int Streamer_GetIntData();
	int Streamer_SetIntData();
	int Streamer_GetArrayData();
	int Streamer_SetArrayData();
	int Streamer_IsInArrayData();
	int Streamer_AppendArrayData();
	int Streamer_RemoveArrayData();
	int Streamer_GetUpperBound();
	// Miscellaneous
	int Streamer_GetDistanceToItem();
	int Streamer_GetItemInternalID();
	int Streamer_GetItemStreamerID();
	int Streamer_IsItemVisible();
	int Streamer_DestroyAllVisibleItems();
	int Streamer_CountVisibleItems();
	int Streamer_DestroyAllItems();
	int Streamer_CountItems();
	// Objects
	int CreateDynamicObject();
	int DestroyDynamicObject();
	int IsValidDynamicObject();
	int SetDynamicObjectPos();
	int GetDynamicObjectPos();
	int SetDynamicObjectRot();
	int GetDynamicObjectRot();
	int MoveDynamicObject();
	int StopDynamicObject();
	int IsDynamicObjectMoving();
	int AttachCameraToDynamicObject();
	int AttachDynamicObjectToVehicle();
	int EditDynamicObject();
	int GetDynamicObjectMaterial();
	int SetDynamicObjectMaterial();
	int GetDynamicObjectMaterialText();
	int SetDynamicObjectMaterialText();
	// Pickups
	int CreateDynamicPickup();
	int DestroyDynamicPickup();
	int IsValidDynamicPickup();
	// Checkpoints
	int CreateDynamicCP();
	int IsValidDynamicCP();
	int DestroyDynamicCP();
	int TogglePlayerDynamicCP();
	int TogglePlayerAllDynamicCPs();
	int IsPlayerInDynamicCP();
	int GetPlayerVisibleDynamicCP();
	// Race Checkpoints
	int CreateDynamicRaceCP();
	int DestroyDynamicRaceCP();
	int IsValidDynamicRaceCP();
	int TogglePlayerDynamicRaceCP();
	int TogglePlayerAllDynamicRaceCPs();
	int IsPlayerInDynamicRaceCP();
	int GetPlayerVisibleDynamicRaceCP();
	// Map Icons
	int CreateDynamicMapIcon();
	int DestroyDynamicMapIcon();
	int IsValidDynamicMapIcon();
	// 3D Text Labels
	int CreateDynamic3DTextLabel();
	int DestroyDynamic3DTextLabel();
	int IsValidDynamic3DTextLabel();
	int GetDynamic3DTextLabelText();
	int UpdateDynamic3DTextLabelText();
	// Areas
	int CreateDynamicCircle();
	int CreateDynamicCylinder();
	int CreateDynamicSphere();
	int CreateDynamicRectangle();
	int CreateDynamicCuboid();
	int CreateDynamicPolygon();
	int DestroyDynamicArea();
	int IsValidDynamicArea();
	int GetDynamicPolygonPoints();
	int GetDynamicPolygonNumberPoints();
	int TogglePlayerDynamicArea();
	int TogglePlayerAllDynamicAreas();
	int IsPlayerInDynamicArea();
	int IsPlayerInAnyDynamicArea();
	int IsAnyPlayerInDynamicArea();
	int IsAnyPlayerInAnyDynamicArea();
	int IsPointInDynamicArea();
	int IsPointInAnyDynamicArea();
	int GetPlayerDynamicAreas();
	int GetPlayerNumberDynamicAreas();
	int AttachDynamicAreaToObject();
	int AttachDynamicAreaToPlayer();
	int AttachDynamicAreaToVehicle();
	// Extended
	int CreateDynamicObjectEx();
	int CreateDynamicPickupEx();
	int CreateDynamicCPEx();
	int CreateDynamicRaceCPEx();
	int CreateDynamicMapIconEx();
	int CreateDynamic3DTextLabelEx();
	int CreateDynamicCircleEx();
	int CreateDynamicCylinderEx();
	int CreateDynamicSphereEx();
	int CreateDynamicRectangleEx();
	int CreateDynamicCuboidEx();
	int CreateDynamicPolygonEx();
	// Internal
	int Streamer_CallbackHook();
	// Deprecated
	int DestroyAllDynamicObjects();
	int CountDynamicObjects();
	int DestroyAllDynamicPickups();
	int CountDynamicPickups();
	int DestroyAllDynamicCPs();
	int CountDynamicCPs();
	int DestroyAllDynamicRaceCPs();
	int CountDynamicRaceCPs();
	int DestroyAllDynamicMapIcons();
	int CountDynamicMapIcons();
	int DestroyAllDynamic3DTextLabels();
	int CountDynamic3DTextLabels();
	int DestroyAllDynamicAreas();
	int CountDynamicAreas();
}

#endif
