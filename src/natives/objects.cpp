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

#include <boost/chrono.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>

#include <eigen3/Eigen/Core>

#include <limits>
#include <string>

int Natives::CreateDynamicObject(int modelid, float x, float y, float z, float rx, float ry, float rz, int worldid, int interiorid, int playerid, float streamdistance, float drawdistance)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_OBJECT) == core->getData()->objects.size())
	{
		return 0;
	}
	int objectID = Item::Object::identifier.get();
	Item::SharedObject object(new Item::Object);
	object->amx = amx;
	object->objectID = objectID;
	object->modelID = modelid;
	object->position = Eigen::Vector3f(x, y, z);
	object->rotation = Eigen::Vector3f(rx, ry, rz);
	Utility::addToContainer(object->worlds, worldid);
	Utility::addToContainer(object->interiors, interiorid);
	Utility::addToContainer(object->players, playerid);
	object->streamDistance = streamdistance * streamdistance;
	object->drawDistance = drawdistance;
	core->getGrid()->addObject(object);
	core->getData()->objects.insert(std::make_pair(objectID, object));
	return objectID;
}

bool Natives::DestroyDynamicObject(int objectid)
{
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		Utility::destroyObject(o);
		return true;
	}
	return false;
}

bool Natives::IsValidDynamicObject(int objectid)
{
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		return true;
	}
	return false;
}

bool Natives::SetDynamicObjectPos(int objectid, float x, float y, float z)
{
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		Eigen::Vector3f position = o->second->position;
		o->second->position = Eigen::Vector3f(x, y, z);
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				SetPlayerObjectPos(p->first, i->second, o->second->position[0], o->second->position[1], o->second->position[2]);
			}
		}
		if (position[0] != o->second->position[0] || position[1] != o->second->position[1])
		{
			if (o->second->cell)
			{
				core->getGrid()->removeObject(o->second, true);
			}
		}
		if (o->second->move)
		{
			o->second->move->duration = static_cast<int>((static_cast<float>(boost::geometry::distance(o->second->move->position.get<0>(), o->second->position) / o->second->move->speed) * 1000.0f));
			o->second->move->position.get<1>() = o->second->position;
			o->second->move->position.get<2>() = (o->second->move->position.get<0>() - o->second->position) / static_cast<float>(o->second->move->duration);
			if ((o->second->move->rotation.get<0>().maxCoeff() + 1000.0f) > std::numeric_limits<float>::epsilon())
			{
				o->second->move->rotation.get<1>() = o->second->rotation;
				o->second->move->rotation.get<2>() = (o->second->move->rotation.get<0>() - o->second->rotation) / static_cast<float>(o->second->move->duration);
			}
			o->second->move->time = boost::chrono::steady_clock::now();
		}
		return true;
	}
	return false;
}

bool Natives::GetDynamicObjectPos(int objectid, float& x, float& y, float& z)
{
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		if (o->second->move)
		{
			core->getStreamer()->processActiveItems();
		}
		x = o->second->position[0];
		y = o->second->position[1];
		z = o->second->position[2];
		return true;
	}
	return false;
}

bool Natives::SetDynamicObjectRot(int objectid, float rx, float ry, float rz)
{
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		o->second->rotation = Eigen::Vector3f(rx, ry, rz);
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				SetPlayerObjectRot(p->first, i->second, o->second->rotation[0], o->second->rotation[1], o->second->rotation[2]);
			}
		}
		if (o->second->move)
		{
			if ((o->second->move->rotation.get<0>().maxCoeff() + 1000.0f) > std::numeric_limits<float>::epsilon())
			{
				o->second->move->rotation.get<1>() = o->second->rotation;
				o->second->move->rotation.get<2>() = (o->second->move->rotation.get<0>() - o->second->rotation) / static_cast<float>(o->second->move->duration);
			}
		}
		return true;
	}
	return false;
}

bool Natives::GetDynamicObjectRot(int objectid, float& rx, float& ry, float& rz)
{
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		if (o->second->move)
		{
			core->getStreamer()->processActiveItems();
		}
		rx = o->second->rotation[0];
		ry = o->second->rotation[1];
		rz = o->second->rotation[2];
		return true;
	}
	return false;
}

int Natives::MoveDynamicObject(int objectid, float x, float y, float z, float speed, float rx, float ry, float rz)
{
	if (!speed)
	{
		return 0;
	}
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		if (o->second->attach)
		{
			sampgdk::logprintf("MoveDynamicObject: Object is currently attached and cannot be moved");
			return 0;
		}
		Eigen::Vector3f position(x, y, z);
		Eigen::Vector3f rotation(rx, ry, rz);
		o->second->move = boost::intrusive_ptr<Item::Object::Move>(new Item::Object::Move);
		o->second->move->duration = static_cast<int>((static_cast<float>(boost::geometry::distance(position, o->second->position) / speed) * 1000.0f));
		o->second->move->position.get<0>() = position;
		o->second->move->position.get<1>() = o->second->position;
		o->second->move->position.get<2>() = (position - o->second->position) / static_cast<float>(o->second->move->duration);
		o->second->move->rotation.get<0>() = rotation;
		if ((o->second->move->rotation.get<0>().maxCoeff() + 1000.0f) > std::numeric_limits<float>::epsilon())
		{
			o->second->move->rotation.get<1>() = o->second->rotation;
			o->second->move->rotation.get<2>() = (rotation - o->second->rotation) / static_cast<float>(o->second->move->duration);
		}
		o->second->move->speed = speed;
		o->second->move->time = boost::chrono::steady_clock::now();
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				StopPlayerObject(p->first, i->second);
				MovePlayerObject(p->first, i->second, o->second->move->position.get<0>()[0], o->second->move->position.get<0>()[1], o->second->move->position.get<0>()[2], o->second->move->speed, o->second->move->rotation.get<0>()[0], o->second->move->rotation.get<0>()[1], o->second->move->rotation.get<0>()[2]);
			}
		}
		core->getStreamer()->movingObjects.insert(o->second);
		return o->second->move->duration;
	}
	return 0;
}

bool Natives::StopDynamicObject(int objectid)
{
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		if (o->second->move)
		{
			for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
			{
				boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
				if (i != p->second.internalObjects.end())
				{
					StopPlayerObject(p->first, i->second);
				}
			}
			o->second->move.reset();
			core->getStreamer()->movingObjects.erase(o->second);
			return true;
		}
	}
	return false;
}

bool Natives::IsDynamicObjectMoving(int objectid)
{
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		if (o->second->move)
		{
			return true;
		}
	}
	return false;
}

bool Natives::AttachCameraToDynamicObject(int playerid, int objectid)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectid);
		if (i != p->second.internalObjects.end())
		{
			AttachCameraToPlayerObject(p->first, i->second);
			return true;
		}
	}
	return false;
}

bool Natives::AttachDynamicObjectToVehicle(int objectid, int vehicleid, float offsetx, float offsety, float offsetz, float rx, float ry, float rz)
{
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		if (o->second->move)
		{
			sampgdk::logprintf("AttachDynamicObjectToVehicle: Object is currently moving and cannot be attached");
			return false;
		}
		o->second->attach = boost::intrusive_ptr<Item::Object::Attach>(new Item::Object::Attach);
		o->second->attach->vehicle = vehicleid;
		o->second->attach->offset = Eigen::Vector3f(offsetx, offsety, offsetz);
		o->second->attach->rotation = Eigen::Vector3f(rx, ry, rz);
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				AttachPlayerObjectToVehicle(p->first, i->second, o->second->attach->vehicle, o->second->attach->offset[0], o->second->attach->offset[1], o->second->attach->offset[2], o->second->attach->rotation[0], o->second->attach->rotation[1], o->second->attach->rotation[2]);
				for (boost::unordered_map<int, Item::Object::Material>::iterator m = o->second->materials.begin(); m != o->second->materials.end(); ++m)
				{
					if (m->second.main)
					{
						SetPlayerObjectMaterial(p->first, i->second, m->first, m->second.main->modelID, m->second.main->txdFileName.c_str(), m->second.main->textureName.c_str(), m->second.main->materialColor);
					}
					else if (m->second.text)
					{
						SetPlayerObjectMaterialText(p->first, i->second, m->second.text->materialText.c_str(), m->first, m->second.text->materialSize, m->second.text->fontFace.c_str(), m->second.text->fontSize, m->second.text->bold, m->second.text->fontColor, m->second.text->backColor, m->second.text->textAlignment);
					}
				}
			}
		}
		if (vehicleid != INVALID_GENERIC_ID)
		{
			core->getStreamer()->attachedObjects.insert(o->second);
		}
		else
		{
			o->second->attach.reset();
			core->getStreamer()->attachedObjects.erase(o->second);
			core->getGrid()->removeObject(o->second, true);
		}
		return true;
	}
	return false;
}

bool Natives::EditDynamicObject(int playerid, int objectid)
{
	boost::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		core->getStreamer()->startManualUpdate(p->second, true);
		boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectid);
		if (i != p->second.internalObjects.end())
		{
			EditPlayerObject(p->first, i->second);
			return true;
		}
	}
	return false;
}

bool Natives::GetDynamicObjectMaterial(int objectid, int materialindex, int& modelid, std::string& txdname, std::string& texturename, int& materialcolor)
{
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		boost::unordered_map<int, Item::Object::Material>::iterator m = o->second->materials.find(materialindex);
		if (m != o->second->materials.end())
		{
			if (m->second.main)
			{
				modelid = m->second.main->modelID;
				txdname = m->second.main->txdFileName;
				texturename = m->second.main->textureName;
				materialcolor = m->second.main->materialColor;
				return true;
			}
		}
	}
	return false;
}

bool Natives::SetDynamicObjectMaterial(int objectid, int materialindex, int modelid, const std::string& txdname, const std::string& texturename, int materialcolor)
{
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		int index = materialindex;
		o->second->materials[index].main = boost::intrusive_ptr<Item::Object::Material::Main>(new Item::Object::Material::Main);
		o->second->materials[index].main->modelID = modelid;
		o->second->materials[index].main->txdFileName = txdname;
		o->second->materials[index].main->textureName = texturename;
		o->second->materials[index].main->materialColor = materialcolor;
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				SetPlayerObjectMaterial(p->first, i->second, index, o->second->materials[index].main->modelID, o->second->materials[index].main->txdFileName.c_str(), o->second->materials[index].main->textureName.c_str(), o->second->materials[index].main->materialColor);
			}
		}
		o->second->materials[index].text.reset();
		return true;
	}
	return false;
}

bool Natives::GetDynamicObjectMaterialText(int objectid, int materialindex, std::string& text, int& materialsize, std::string& fontface, int& fontsize, bool& bold, int& fontcolor, int& backcolor, int& textalignment)
{
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		boost::unordered_map<int, Item::Object::Material>::iterator m = o->second->materials.find(materialindex);
		if (m != o->second->materials.end())
		{
			if (m->second.text)
			{
				text = m->second.text->materialText;
				materialsize = m->second.text->materialSize;
				fontface = m->second.text->fontFace;
				fontsize = m->second.text->fontSize;
				bold = m->second.text->bold != 0;
				fontcolor = m->second.text->fontColor;
				backcolor = m->second.text->backColor;
				textalignment = m->second.text->textAlignment;
				return true;
			}
		}
	}
	return false;
}

bool Natives::SetDynamicObjectMaterialText(int objectid, int materialindex, const std::string& text, int materialsize, const std::string& fontface, int fontsize, bool bold, int fontcolor, int backcolor, int textalignment)
{
	boost::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		int index = materialindex;
		o->second->materials[index].text = boost::intrusive_ptr<Item::Object::Material::Text>(new Item::Object::Material::Text);
		o->second->materials[index].text->materialText = text;
		o->second->materials[index].text->materialSize = materialsize;
		o->second->materials[index].text->fontFace = fontface;
		o->second->materials[index].text->fontSize = fontsize;
		o->second->materials[index].text->bold = bold;
		o->second->materials[index].text->fontColor = fontcolor;
		o->second->materials[index].text->backColor = backcolor;
		o->second->materials[index].text->textAlignment = textalignment;
		for (boost::unordered_map<int, Player>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				SetPlayerObjectMaterialText(p->first, i->second, o->second->materials[index].text->materialText.c_str(), index, o->second->materials[index].text->materialSize, o->second->materials[index].text->fontFace.c_str(), o->second->materials[index].text->fontSize, o->second->materials[index].text->bold, o->second->materials[index].text->fontColor, o->second->materials[index].text->backColor, o->second->materials[index].text->textAlignment);
			}
		}
		o->second->materials[index].main.reset();
		return true;
	}
	return false;
}
