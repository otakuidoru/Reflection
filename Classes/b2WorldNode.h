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

#ifndef __B2WORLD_NODE_H__
#define __B2WORLD_NODE_H__

#include <memory>
#include "cocos2d.h"
#include "external/Box2D/include/Box2D/Box2D.h"
#include "Box2DDebugDraw.h"
#include "Globals.h"

class b2WorldNode : public cocos2d::Node {
protected:
	std::unique_ptr<b2World> world;

	b2WorldNode();

//#if (CC_BOX2D_DEBUG)
	std::unique_ptr<Box2DDebugDraw> _debugDrawInst = nullptr;
//#endif

public:
	static b2WorldNode* create();
	virtual ~b2WorldNode();

	virtual bool init() override;

	b2World* getb2World() const { return world.get(); }
	float getPixelsPerMeter() const { return Globals::getInstance().getBox2DScale(); }

//#if (CC_BOX2D_DEBUG)
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
//#endif

	virtual void update(float dt) override;
};

#endif // __B2WORLD_NODE_H__

