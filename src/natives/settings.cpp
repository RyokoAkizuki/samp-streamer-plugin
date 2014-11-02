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

#include <boost/scoped_ptr.hpp>

int Natives::Streamer_GetTickRate()
{
	return core->getStreamer()->getTickRate();
}

bool Natives::Streamer_SetTickRate(std::size_t rate)
{
	if (rate > 0)
	{
		core->getStreamer()->setTickRate(rate);
		return true;
	}
	return false;
}

std::size_t Natives::Streamer_GetMaxItems(int type)
{
	return core->getData()->getMaxItems(static_cast<std::size_t>(type));
}

bool Natives::Streamer_SetMaxItems(int type, int items)
{
	return core->getData()->setMaxItems(type, items) != 0;
}

std::size_t Natives::Streamer_GetVisibleItems(int type)
{
	return core->getStreamer()->getVisibleItems(static_cast<std::size_t>(type));
}

bool Natives::Streamer_SetVisibleItems(int type, int items)
{
	return core->getStreamer()->setVisibleItems(type, items) != 0;
}

float Natives::Streamer_GetCellDistance()
{
	float cellDistance = core->getGrid()->getCellDistance();
	return cellDistance;
}

void Natives::Streamer_SetCellDistance(float streamdistance)
{
	core->getGrid()->setCellDistance(streamdistance * streamdistance);
	core->getGrid()->rebuildGrid();
}

float Natives::Streamer_GetCellSize()
{
	float cellSize = core->getGrid()->getCellSize();
	return cellSize;
}

void Natives::Streamer_SetCellSize(float size)
{
	core->getGrid()->setCellSize(size);
	core->getGrid()->rebuildGrid();
}
