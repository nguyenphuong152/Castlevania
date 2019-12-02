﻿#pragma once

#include <map>
#include <unordered_map>

#include "define.h"
#include "Game.h"
#include "Camera.h"
#include "GameObject.h"
#include "Simon.h"
#include "tilemap.h"
#include "Grid.h"

#define MAX_TIME_STOP_WATCH 5000
using namespace std;

enum SCENEID
{
	SCENEID_0,
	SCENEID_1,
	SCENEID_2,
	SCENEID_3,
};

class CScene
{
public:

	string linkObjects;
	SCENEID sceneID = SCENEID::SCENEID_1; // mặc định là scene 1
	int mapID;
	CScene(SCENEID sceneID, int mapID, string link);

};

typedef CScene * LPSCENE;

class CScenes
{
	static CScenes * __instance; // Singleton Patern
	unordered_map<int, LPGAMEOBJECT> objects; //các object của map
	vector<LPGAMEOBJECT> onCamObjects; //các object trong camera
	unordered_map<SCENEID, LPSCENE> scenes;
	SCENEID currentScene;
	int curentMap;
	DWORD accutime = 0;
	CGrid *grid;
	LPCAMERA camera; // lưu địa chỉ cam lại để khỏi phải gọi getinstance()
	LPSIMON simon; //lưu luôn con simon cho lẹ
public:
	bool isStopWatchInUse = false;
	vector<LPGAMEOBJECT> stairs; //các cầu thang trong camera
	CScenes();
	void Update(DWORD dt);
	void Render();
	void Add(SCENEID sceneID, int mapID, string linkObjects);
	LPSCENE getCurrentScene() { return scenes[SCENEID(currentScene)]; };
	static CScenes * GetInstance();
	// Get, Set
	void insertObject(LPGAMEOBJECT object);
	void clearAllObject();
	void updateCamPos();
	void changeScene(LPGAMEOBJECT obj);
	void changeScene(SCENEID newScene);
	void loadObjectToGrid(string path);
	void getObjectsFromGrid(int xCam, int widthCam);
	void stopObject();
	int getObjectsSize() { return objects.size(); };

};

