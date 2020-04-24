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

#include "b2WorldNode.h"

USING_NS_CC;

/**
 *
 */
b2WorldNode::b2WorldNode() : Node() {
}

/**
 *
 */
b2WorldNode::~b2WorldNode() {
//#if (CC_BOX2D_DEBUG)
	_debugDrawInst = nullptr;
//#endif
}

/**
 *
 */
b2WorldNode* b2WorldNode::create() {
	b2WorldNode* node = new (std::nothrow) b2WorldNode();
	if (node && node->init()) {
		node->autorelease();
		return node;
	}
	CC_SAFE_DELETE(node);
	return nullptr;
}

/**
 *
 */
bool b2WorldNode::init() {
	this->world = std::unique_ptr<b2World>(new b2World(b2Vec2(0.0f, 0.0f)));

//#if (CC_BOX2D_DEBUG)
	this->_debugDrawInst = std::unique_ptr<Box2DDebugDraw>(new Box2DDebugDraw(this->getPixelsPerMeter()));

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	this->_debugDrawInst->SetFlags(flags);

	this->getb2World()->SetDebugDraw(this->_debugDrawInst.get());
//#endif

	this->scheduleUpdate();

	return true;
}

//#if (CC_BOX2D_DEBUG)
/**
 *
 */
void b2WorldNode::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t parentFlags) {
	Node::draw(renderer, transform, parentFlags);

	auto director = Director::getInstance();
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	this->_debugDrawInst->GetDrawNode()->clear();
	this->getb2World()->DrawDebugData();
	this->_debugDrawInst->GetDrawNode()->draw(renderer, transform, parentFlags);
	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
//#endif

/**
 *
 */
void b2WorldNode::update(float dt) {
//	this->getb2World()->Step(1/60.0f, 6, 2);
}

