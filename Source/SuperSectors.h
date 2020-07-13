#pragma once
#include "Defines.h"
#include "Checksum.h"
#include "Mesh.h"
#include "Node.h"

//Collision stuff 00501CE0
struct MovingObject;
extern void RemoveMovingObject(SuperSector* sector);


EXTERN struct SuperSector
{
	DWORD FFFFFFFF;
	WORD* indices;
	D3DXVECTOR3* vertices;
	DWORD* pUnk1;//always NULL?
	DWORD* pUnk2;//Looks like axis for CollisionTree?
	DWORD* pUnk3;//Huge data...
	DWORD* pUnk4;//Looks like colors?
	Object* object;
	DWORD* pUnk6[6];//Points to itself
	D3DXVECTOR3 bboxMax;
	D3DXVECTOR3 bboxMin;
	DWORD* pUnk7;//Always NULL?
	Mesh* mesh;
	DWORD* pUnk9;//Always NULL?
	WORD padding;
	WORD numVertices;
	WORD numIndices;
	WORD unk1;//maybe flags?
	DWORD* pUnk10;//maybe this is the leaf??
	DWORD* pUnk11;//Always NULL?
	DWORD flag;//always 6?
	DWORD* pUnk12;//Always NULL?
	WORD* pCollisionFlags;//the flags for skatable, trigger etc
	DWORD* pUnk13;//bunch of 0xFF and random data
	DWORD* pUnk14;//bunch of 00
	BYTE unknown2;//Used when collision checking
	BYTE padding2[3];
	DWORD unknownChecksum;// Checksum??
	DWORD* pUnk16;//bunch of floats, nothing happens when changing them maybe center/position?
	DWORD name;//crc32
	DWORD* pUnk17;//similar to pUnk16

	//004fea30 00412230
	EXTERN static SuperSector* GetSuperSector(DWORD checksum)
	{
		//code to get index from checksum
		static DWORD* pSectors = 0;
		_asm mov eax, [0x0085A4B8];
		_asm mov eax, [eax];
		_asm test eax, eax;
		_asm jne con;
		_asm xor eax, eax;
		_asm ret;
	con:
		_asm mov edx, checksum;
		_asm and edx, 0x00003FFF;
		_asm lea edx, [edx + edx * 2];
		_asm lea ecx, [eax + edx * 4];
		_asm mov pSectors, ecx;


		//the SuperSectors are stored in a list 
		//each item is 8 bytes, the first 4 bytes is checksum and last 4 bytes is pointer to the SuperSector
		//since 2 or more checksums can have the same index we need to loop until we get a checksum match
		//if we find an uninitialized item before we get a checksum match it means the checksum is not in the list
		while (*pSectors != 0)//Continue until found an uninitialized item
		{

			if (*pSectors == checksum)//Checksum match
			{
				pSectors++;//skip 4 bytes to get the pointer
				return (SuperSector*)*pSectors;//return the pointer
			}
			pSectors += 2;//skip 8 bytes to get next item in the list
		}
		MessageBox(0, "return NULL", "", 0);//Checksum is not in the list
		return NULL;
	}

	//Used in the scripts create/kill/shatter/visible/invisible, the state will get updated the next frame
	void SetState(MeshState state)
	{

		typedef void(__cdecl* const pSetMeshState)(DWORD index, DWORD state, CScript* pScript);
		CScript pScript;
		pSetMeshState(0x00418DD0)(Node::GetNodeIndex(name), state, &pScript);
	}
};

struct MovingObject
{
	enum Types
	{
		MOVE_TO_POS,
		MOVE_TO_NODE,
		FOLLOW_PATH_LINKED,
		ANGULAR_VELOCITY
	};
	Types Type;
	float timer;
	float end;
	float speed;
	D3DXVECTOR3 goal;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 angle;
	D3DXVECTOR3 goalAngle;
	SuperSector* sector;
	CStructHeader* link;
	CScript* pScript;
	D3DXVECTOR3 bboxMax;
	D3DXVECTOR3 bboxMin;
	D3DXVECTOR3* vertices;
	D3DXMATRIX orient;
	D3DXMATRIX position;


	MovingObject(SuperSector* _sector, D3DXVECTOR3 & _goal, CScript* _pScript)
	{
		Type = MOVE_TO_NODE;
		link = NULL;
		sector = _sector;
		this->pScript = _pScript;
		pos = (sector->bboxMax + sector->bboxMin) / 2.0f;
		/*if (type == MOVE_TO_POS)
		{*/
			angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			goal = _goal;
			timer = 0.0f;
			float distance = D3DXVec3Length(&(pos - goal));
			speed = 100.0f;
			end = distance / speed;
			
		/*}
		else
		{
			angle = _goal;
			goalAngle = _goal;
			angle *= 0.0015339794921875f;
			goalAngle *= 0.0015339794921875f;
			bboxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
			bboxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

			for (DWORD i = 0; i < sector->numVertices; i++)
			{
				if (sector->vertices[i].x > bboxMax.x)
					bboxMax.x = sector->vertices[i].x;
				if (sector->vertices[i].x < bboxMin.x)
					bboxMin.x = sector->vertices[i].x;

				if (sector->vertices[i].y > bboxMax.y)
					bboxMax.y = sector->vertices[i].y;
				if (sector->vertices[i].y < bboxMin.y)
					bboxMin.y = sector->vertices[i].y;

				if (sector->vertices[i].z > bboxMax.z)
					bboxMax.z = sector->vertices[i].z;
				if (sector->vertices[i].z < bboxMin.z)
					bboxMin.z = sector->vertices[i].z;
			}
			sector->bboxMax = bboxMax;
			sector->bboxMin = bboxMin;
			pos = (sector->bboxMax + sector->bboxMin) / 2.0f;
		}*/
		vertices = NULL;
	}

	MovingObject(SuperSector* _sector, D3DXVECTOR3& _angle, CScript* _pScript, DWORD checksum)
	{
		Type = ANGULAR_VELOCITY;
		sector = _sector;
		this->link = Node::GetNodeStruct(checksum);
		if (!this->link)
			MessageBox(0, "", "", 0);
		CStructHeader* _position;
		if (!this->link->GetStruct(Checksums::Position, &_position))
			pos = D3DXVECTOR3(0, 0, 0);
		else
			pos = *_position->pVec;


		angle = _angle;
		angle *= 0.0015339794921875f;

		bboxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		bboxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
		vertices = new D3DXVECTOR3[sector->numVertices];

		for (DWORD i = 0; i < sector->numVertices; i++)
		{
			if (sector->vertices[i].x > bboxMax.x)
				bboxMax.x = sector->vertices[i].x;
			if (sector->vertices[i].x < bboxMin.x)
				bboxMin.x = sector->vertices[i].x;

			if (sector->vertices[i].y > bboxMax.y)
				bboxMax.y = sector->vertices[i].y;
			if (sector->vertices[i].y < bboxMin.y)
				bboxMin.y = sector->vertices[i].y;

			if (sector->vertices[i].z > bboxMax.z)
				bboxMax.z = sector->vertices[i].z;
			if (sector->vertices[i].z < bboxMin.z)
				bboxMin.z = sector->vertices[i].z;
			vertices[i] = sector->vertices[i] - pos;
		}
		sector->bboxMax = bboxMax;
		sector->bboxMin = bboxMin;
		bboxMax -= pos;
		bboxMin -= pos;
		pos = (sector->bboxMax + sector->bboxMin) / 2.0f;
		D3DXMatrixTranslation(&position, pos.x, pos.y, pos.z);
		D3DXMatrixIdentity(&orient);
		//vertices = NULL;
	}

	MovingObject(SuperSector* _sector, D3DXVECTOR3 & _goal, CScript* _pScript, CStructHeader* _node)
	{

		Type = FOLLOW_PATH_LINKED;
		sector = _sector;
		
			angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			goal = _goal;
			pos = (sector->bboxMax + sector->bboxMin) / 2.0f;
			timer = 0.0f;
			float distance = D3DXVec3Length(&(pos - goal));
			speed = 120000.0f;
			end = distance / speed;
			this->link = _node;
			this->pScript = _pScript;
			vertices = new D3DXVECTOR3[sector->numVertices];
			bboxMax = sector->bboxMax - pos;
			bboxMin = sector->bboxMin - pos;
			for (DWORD i = 0; i < sector->numVertices; i++)
			{
				vertices[i] = sector->vertices[i] - pos;
			}
			CStructHeader* angles;
			if (!_pScript->node->GetNodeStruct()->GetStruct(Checksums::Angles, &angles))
				angle = D3DXVECTOR3(0, 0, 0);
			else
				angle = *angles->pVec;

			if (!link->GetStruct(Checksums::Angles, &angles))
				goalAngle = D3DXVECTOR3(0, 0, 0);
			else
				goalAngle = *angles->pVec;
	}

	~MovingObject()
	{
		if (vertices)
			delete[] vertices;
	}


	void Update(float delta)
	{
		static D3DXVECTOR3 direction, velocity;
		static D3DXMATRIX nodeRotation, nodeTranslation, world;
		D3DXMatrixIdentity(&nodeRotation);
		D3DXMatrixIdentity(&nodeTranslation);
		D3DXMatrixIdentity(&world);

		
		//printf("MovingObject on the move(%f %f %f) dt:%f timer:%f end:%f\n", pos.x, pos.y, pos.z, delta, timer, end);

		timer += delta;
		switch (Type)
		{
		case MOVE_TO_POS:
			if (timer >= end)
			{
				if (pos != goal)
				{
					D3DXVECTOR3 relPos = goal - pos;

					sector->bboxMax += relPos;
					sector->bboxMin += relPos;
					for (DWORD i = 0; i < sector->numVertices; i++)
					{
						sector->vertices[i] += relPos;
					}
					//Send state to update vertexbuffer
					sector->mesh->Update();
				}

				RemoveMovingObject(sector);
				printf("MovingObject final destination\n");
				return;
			}

			direction = goal - pos;
			D3DXVec3Normalize(&velocity, &direction);
			velocity *= speed * delta;

			sector->bboxMax += velocity;
			sector->bboxMin += velocity;
			for (DWORD i = 0; i < sector->numVertices; i++)
			{
				sector->vertices[i] += velocity;
			}
			pos += velocity;

			//Send state to update vertexbuffer
			sector->mesh->Update();
			break;

		case FOLLOW_PATH_LINKED:
			if (timer >= end)
			{
				if (angle == goalAngle)
				{
					if (pos != goal)
					{
						D3DXVECTOR3 relPos = goal - pos;

						sector->bboxMax += relPos;
						sector->bboxMin += relPos;
						for (DWORD i = 0; i < sector->numVertices; i++)
						{
							sector->vertices[i] += relPos;
						}

					}
					else
						return;
				}
				else
				{
					//move the object to origin and then rotate the object and translate it back to the new position
					D3DXMatrixRotationYawPitchRoll(&nodeRotation, -goalAngle.y + D3DX_PI, goalAngle.x, goalAngle.z);

					D3DXMatrixTranslation(&nodeTranslation, goal.x, goal.y, goal.z);
					D3DXMatrixMultiply(&world, &nodeRotation, &nodeTranslation);


					sector->bboxMax = Transform(bboxMax, world);
					sector->bboxMin = Transform(bboxMin, world);

					for (DWORD i = 0; i < sector->numVertices; i++)
					{
						sector->vertices[i] = Transform(vertices[i], world);
					}

				}

				//Send state to update vertexbuffer
				sector->mesh->Update();

				CArray* links = link->GetArray(Checksums::Links);
				if (!links)
				{
					RemoveMovingObject(sector);
					printf("MovingObject final destination\n");
					return;
				}

				link = Node::GetNodeStructByIndex((*links)[0]);
				if (!link)
				{
					RemoveMovingObject(sector);
					printf("Couldn't find link[%d]...\n", (*links)[0]);
					return;
				}

				CStructHeader* _pos;
				link->GetStruct(Checksums::Position, &_pos);
				goal = *_pos->pVec;
				timer = 0;
				float distance = D3DXVec3Length(&(pos - goal));
				speed = 100.0f;
				end = distance / speed;
				return;
			}

			if (angle == goalAngle)
			{
				direction = goal - pos;
				D3DXVec3Normalize(&velocity, &direction);
				velocity *= speed * delta;

				sector->bboxMax += velocity;
				sector->bboxMin += velocity;
				for (DWORD i = 0; i < sector->numVertices; i++)
				{
					sector->vertices[i] += velocity;
				}
				pos += velocity;
			}
			else
			{
				D3DXVECTOR3 lookAt = goalAngle - angle;
				D3DXVec3Normalize(&lookAt, &lookAt);
				lookAt *= delta;
				D3DXMatrixRotationYawPitchRoll(&nodeRotation, -angle.y + D3DX_PI, angle.x, angle.z);

				direction = goal - pos;
				D3DXVec3Normalize(&velocity, &direction);
				velocity *= speed * delta;

				D3DXMatrixTranslation(&nodeTranslation, pos.x, pos.y, pos.z);
				D3DXMatrixMultiply(&world, &nodeRotation, &nodeTranslation);


				sector->bboxMax = Transform(bboxMax, world);
				sector->bboxMin = Transform(bboxMin, world);

				for (DWORD i = 0; i < sector->numVertices; i++)
				{
					sector->vertices[i] = Transform(vertices[i], world);
				}

				pos += velocity;
				angle = lookAt;
			}

			//Send state to update vertexbuffer
			sector->mesh->Update();
			break;
		case ANGULAR_VELOCITY:
			if (angle.x || angle.y || angle.z)
			{
				//same matrix function I use in Level Editor
				D3DXMatrixRotationYawPitchRoll(&nodeRotation, -angle.y, angle.x, angle.z);
				D3DXMatrixMultiply(&orient, &orient, &nodeRotation);
				D3DXMatrixMultiply(&world, &orient, &position);

				//make a new bbox in a temp location so we can keep the actual bbox
				bboxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
				bboxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

				//temporarly make the actual bbox very big
				sector->bboxMax = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
				sector->bboxMin = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

				for (DWORD i = 0; i < sector->numVertices; i++)
				{
					//Use the vertices that's translated to origin and transform them with the rotation*position matrix
					sector->vertices[i] = Transform(vertices[i], world);

					//calculate the new bbox into the temp location
					if (bboxMax.x < sector->vertices[i].x)
						bboxMax.x = sector->vertices[i].x;
					if (bboxMin.x > sector->vertices[i].x)
						bboxMin.x = sector->vertices[i].x;

					if (bboxMax.y < sector->vertices[i].y)
						bboxMax.y = sector->vertices[i].y;
					if (bboxMin.y > sector->vertices[i].y)
						bboxMin.y = sector->vertices[i].y;

					if (bboxMax.z < sector->vertices[i].z)
						bboxMax.z = sector->vertices[i].z;
					if (bboxMin.z > sector->vertices[i].z)
						bboxMin.z = sector->vertices[i].z;
				}

				//set the actual bbox to be same as temp bbox
				sector->bboxMax = bboxMax;
				sector->bboxMin = bboxMin;

				//Check if pos changed
				D3DXVECTOR3 newPos = (sector->bboxMax + sector->bboxMin) / 2.0f;
				if (newPos != pos)
				{
					//Why does the pos change???
					printf("newPos %f %f %f pos %f %f %f\n", newPos.x, newPos.y, newPos.z, pos.x, pos.y, pos.z);
					newPos = pos - newPos;
					sector->bboxMax += newPos;
					sector->bboxMin += newPos;
					for (DWORD i = 0; i < sector->numVertices; i++)
					{
						sector->vertices[i] += newPos;
					}
					newPos = (sector->bboxMax + sector->bboxMin) / 2.0f;
				}
				if (newPos != pos)//Even now the pos is changed, rounding errors or my math is wrong?????
					printf("AGAIN newPos %f %f %f pos %f %f %f\n", newPos.x, newPos.y, newPos.z, pos.x, pos.y, pos.z);
				pos = newPos;

				printf("bboxMax %f %f %f bboxMin %f %f %f\n", sector->bboxMax.x, sector->bboxMax.y, sector->bboxMax.z, sector->bboxMin.x, sector->bboxMin.y, sector->bboxMin.z);
				
				//Send state to update vertexbuffer
				sector->mesh->Update();
			}
			return;
		}
	}


};

extern std::vector<MovingObject> movingObjects;//List of Objects on the move