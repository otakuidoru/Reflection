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

#ifndef __BOX2D_DEBUG_DRAW_H__
#define __BOX2D_DEBUG_DRAW_H__

#include "external/Box2D/include/Box2D/Box2D.h"
#include "cocos2d.h"

class Box2DDebugDraw : public b2Draw {
protected:
	float32 _ratio;
	cocos2d::DrawNode* _drawNode = nullptr;

public:
	Box2DDebugDraw(float32 ratio = 1.0f);
	virtual ~Box2DDebugDraw();

	void DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color) override;
	void DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color) override;
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
	void DrawTransform(const b2Transform& xf) override;
	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override;

	void setRatio(float32 ratio) { _ratio = ratio; }

	cocos2d::DrawNode* GetDrawNode() const { return _drawNode; }
};

#endif // __BOX2D_DEBUG_DRAW_H__

