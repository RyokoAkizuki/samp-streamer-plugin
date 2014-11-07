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

#include "main.h"

#include "core.h"
#include "natives.h"
#include "utility.h"

#include <boost/scoped_ptr.hpp>

#include <set>

#define SUPPORTS_PROCESS_TICK 0x20000

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return sampgdk::Supports() | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	core.reset(new Core);
	bool load = sampgdk::Load(ppData);
	sampgdk::logprintf("\n\n*** Streamer Plugin v%s by Incognito loaded ***\n", PLUGIN_VERSION);
	return load;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	core.reset();
	sampgdk::logprintf("\n\n*** Streamer Plugin v%s by Incognito unloaded ***\n", PLUGIN_VERSION);
	sampgdk::Unload();
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	sampgdk::ProcessTick();
	core->getStreamer()->startAutomaticUpdate();
}
