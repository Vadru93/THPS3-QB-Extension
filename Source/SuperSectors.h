#pragma once
#include "Defines.h"
#include "Checksum.h"
#include "Mesh.h"
#include "Node.h"

//Collision stuff 00501CE0
struct MovingObject;
extern void RemoveMovingObject(SuperSector* sector);

struct UnknownStruct
{
	BYTE unknown[0x38C];
	DWORD ptr;

	UnknownStruct()
	{
		ptr = 0xFFFFFFFF;
	}
};

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
	DWORD* pUnk10;//Or maybe this is the leaf??
	DWORD* pUnk11;//Always NULL?
	DWORD flag;//always 6?
	DWORD* pUnk12;//Always NULL?
	WORD* pCollisionFlags;
	DWORD* pUnk13;//bunch of 0xFF and random data
	DWORD* pUnk14;//bunch of 00
	BYTE unknown2;//Used when collision checking
	BYTE padding2[3];
	DWORD unknownChecksum;// Checksum??
	DWORD* pUnk16;//bunch of floats, nothing happens when changing them maybe center/position?
	DWORD name;//crc32
	DWORD* pUnk17;//same as pUnk16

	//004fea30 00412230
	EXTERN static SuperSector* GetSuperSector(DWORD checksum)
	{
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

		while (*pSectors != 0)
		{

			if (*pSectors == checksum)
			{
				pSectors++;
				return (SuperSector*)*pSectors;
			}
			pSectors += 2;
		}
		MessageBox(0, "return NULL", "", 0);
		return NULL;
	}

	void SetState(MeshState state)
	{

		typedef void(__cdecl* const pSetMeshState)(DWORD index, DWORD state, UnknownStruct* unk1);
		UnknownStruct unk1;
		pSetMeshState(0x00418DD0)(Node::GetNodeIndex(name), state, &unk1);
	}
};

struct MovingObject
{
	enum Types
	{
		MOVE_TO_POS,
		MOVE_TO_NODE,
		FOLLOW_PATH_LINKED
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


	MovingObject(SuperSector* _sector, D3DXVECTOR3 _goal, CScript* _pScript)
	{
		Type = MOVE_TO_POS;
		link = NULL;
		sector = _sector;
		angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		goal = _goal;
		pos = (sector->bboxMax + sector->bboxMin) / 2.0f;
		timer = 0.0f;
		float distance = D3DXVec3Length(&(pos - goal));
		speed = 100.0f;
		end = distance / speed;
		this->pScript = _pScript;
		vertices = NULL;
	}

	MovingObject(SuperSector* _sector, D3DXVECTOR3 _goal, CScript* _pScript, CStructHeader* _node)
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
		this->pScript = _pScript;
		this->link = _node;
		vertices = new D3DXVECTOR3[sector->numVertices];
		bboxMax = sector->bboxMax - pos;
		bboxMin = sector->bboxMin - pos;
		for (DWORD i = 0; i < sector->numVertices; i++)
		{
			vertices[i] = sector->vertices[i]-pos;
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

		sector->mesh->Update();//Set state to update vertexbuffer
		printf("MovingObject on the move(%f %f %f) dt:%f timer:%f end:%f\n", pos.x, pos.y, pos.z, delta, timer, end);

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
				}
				else
				{
					printf("Moving ANgle FInal\n");
					//move the object to origin and then rotate the object and translate it back to the new position
					D3DXMATRIX nodeTranslation;
					D3DXMATRIX nodeRotation;
					D3DXMATRIX world;
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
				printf("Moving ANgle\n");
				D3DXMATRIX nodeTranslation;
				D3DXMATRIX nodeRotation;
				D3DXMATRIX world;
				D3DXVECTOR3 lookAt = goalAngle - angle;
				D3DXVec3Normalize(&lookAt, &lookAt);
				lookAt *= delta;
				D3DXMatrixRotationYawPitchRoll(&nodeRotation, -angle.y + D3DX_PI, angle.x, angle.z);

				direction = goal - pos;
				D3DXVec3Normalize(&velocity, &direction);
				velocity *= speed * delta;

				D3DXMatrixTranslation(&nodeTranslation, velocity.x, velocity.y, velocity.z);
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

			break;
		}
	}


};

extern std::vector<MovingObject> movingObjects;//List of Objects on the move