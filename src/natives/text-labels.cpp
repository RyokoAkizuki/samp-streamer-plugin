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

#include <string>

int Natives::CreateDynamic3DTextLabel(const std::string& text, int color, float x, float y, float z, float drawdistance, int attachedplayer, int attachedvehicle, bool testlos, int worldid, int interiorid, int playerid, float streamdistance)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_3D_TEXT_LABEL) == core->getData()->textLabels.size())
	{
		return 0;
	}
	int textLabelID = Item::TextLabel::identifier.get();
	Item::SharedTextLabel textLabel(new Item::TextLabel);
	textLabel->amx = amx;
	textLabel->textLabelID = textLabelID;
	textLabel->text = text;
	textLabel->color = color;
	textLabel->position = Eigen::Vector3f(x, y, z);
	textLabel->drawDistance = streamdistance;
	if (attachedplayer != INVALID_GENERIC_ID || attachedvehicle != INVALID_GENERIC_ID)
	{
		textLabel->attach = boost::intrusive_ptr<Item::TextLabel::Attach>(new Item::TextLabel::Attach);
		textLabel->attach->player = attachedplayer;
		textLabel->attach->vehicle = attachedvehicle;
		if (textLabel->position.cwiseAbs().maxCoeff() > 50.0f)
		{
			textLabel->position.setZero();
		}
		core->getStreamer()->attachedTextLabels.insert(textLabel);
	}
	textLabel->testLOS = testlos != 0;
	Utility::addToContainer(textLabel->worlds, worldid);
	Utility::addToContainer(textLabel->interiors, interiorid);
	Utility::addToContainer(textLabel->players, playerid);
	textLabel->streamDistance = streamdistance * streamdistance;
	core->getGrid()->addTextLabel(textLabel);
	core->getData()->textLabels.insert(std::make_pair(textLabelID, textLabel));
	return textLabelID;
}

bool Natives::DestroyDynamic3DTextLabel(int id)
{
	boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
	if (t != core->getData()->textLabels.end())
	{
		Utility::destroyTextLabel(t);
		return true;
	}
	return false;
}

bool Natives::IsValidDynamic3DTextLabel(int id)
{
	boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
	if (t != core->getData()->textLabels.end())
	{
		return true;
	}
	return false;
}

bool Natives::GetDynamic3DTextLabelText(int id, std::string& text)
{
	boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
	if (t != core->getData()->textLabels.end())
	{
		text = t->second->text;
		return true;
	}
	return false;
}

bool Natives::UpdateDynamic3DTextLabelText(int id, int color, const std::string& text)
{
	boost::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
	if (t != core->getData()->textLabels.end())
	{
		t->second->color = color;
		t->second->text = text;
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.find(t->first);
			if (i != p->second.internalTextLabels.end())
			{
				UpdatePlayer3DTextLabelText(p->first, i->second, t->second->color, t->second->text.c_str());
			}
		}
		return true;
	}
	return false;
}
