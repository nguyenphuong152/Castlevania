﻿#include "Item.h"
#include "Simon.h"

CItem::CItem(ItemType itemType)
{
	type = ObjectType::ITEM;
	item = itemType;
	timeDisplayed = 0;
	timeWaited = 0;
	isFinish = 0;
	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT - 120 - BIG_HEART_BBOX;
	LoadResources();
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isEnable) return;
	if (timeWaited < TIME_DELAY_ITEM)
	{
		timeWaited += dt;
		return;
	}

	timeDisplayed += dt;
	if (timeDisplayed >= TIME_APPEAR_ITEM)
	{
		isEnable = false;
		return;
	}

	// Simple fall down
	vy += BIG_HEART_GRAVITY * dt;
	CGameObject::Update(dt);

	if (CSimon::GetInstance()->isColisionItem(this))
	{
		DebugOut(L"[SIMON VÀ ITEM] Simon chạm item: %d\n");
		isEnable = false;
		CSimon::GetInstance()->colisionItem(this);
	}

	vector<LPGAMEOBJECT> listObject_Brick;
	listObject_Brick.clear();
	for (UINT i = 0; i < coObjects->size(); i++) {
		if (coObjects->at(i)->type == ObjectType::BRICK)
			listObject_Brick.push_back(coObjects->at(i));
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(&listObject_Brick, coEvents); // Lấy danh sách các va chạm 
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		y += min_ty * dy + ny * 0.4f;
		if (ny != 0)
		{
			vy = 0;
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void CItem::Render()
{
	if (timeWaited < TIME_DELAY_ITEM || timeDisplayed >= TIME_APPEAR_ITEM || !isEnable) return;
	animations[0]->Render(x, y);
	RenderBoundingBox();
}

void CItem::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BIG_HEART_BBOX;
	b = y + BIG_HEART_BBOX;
}


void CItem::LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_ITEM, L"textures\\Items.png", D3DCOLOR_XRGB(255, 0, 255));

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texItem = textures->Get(ID_TEX_ITEM);

	// heart
	sprites->Add(60000, 16,	0,	40,	20, texItem);

	LPANIMATION ani;

	ani = new CAnimation(100);	// heart
	ani->Add(60000);
	animations->Add(900, ani);

	AddAnimation(900);		// heart

}