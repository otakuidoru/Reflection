/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __LEVEL_SELECT_SCENE_H__
#define __LEVEL_SELECT_SCENE_H__

#include <map>
#include <string>
#include "cocos2d.h"

class LevelSelect : public cocos2d::Scene {
protected:
	int worldId;

public:
	std::map<cocos2d::Sprite*, std::string> levelSprites;
	std::map<int, std::string> levelNumFilePathMap;
	std::map<cocos2d::Sprite*, bool> levelSpriteLockedMap;
	std::map<int, cocos2d::Vec2> levelNumRelativePositionMap;
	std::map<cocos2d::Sprite*, int> levelSpriteLevelIdMap;

	static cocos2d::Scene* createScene();

	// implement the "static create()" method manually
	static cocos2d::Scene* createScene(const int worldId);

	// implement the "static create()" method manually
	static LevelSelect* create(int worldId);
	virtual bool init(int worldId);
};

#endif // __LEVEL_SELECT_SCENE_H__

