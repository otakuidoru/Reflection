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

#include <cmath>
#include "Globals.h"
#include "Laser.h"

USING_NS_CC;

static const float DEGTORAD = 0.0174532925199432957f;
static const float RADTODEG = 57.295779513082320876f;

/**
 *
 */
Laser::Laser(int id, b2World* world) : Sprite(), id(id), world(world) {
}

/**
 *
 */
Laser::~Laser() {
	if (this->ray) {
		delete this->ray;
	}
}

/**
 *
 */
Laser* Laser::create(int id, b2World* world) {
	Laser* laser = new (std::nothrow) Laser(id, world);
	if (laser && laser->initWithFile("blue_laser.png", world)) {
		laser->autorelease();
		return laser;
	}
	CC_SAFE_DELETE(laser);
	return nullptr;
}

/**
 * on "init" you need to initialize your instance
 */
bool Laser::initWithFile(const std::string& filename, b2World* world) {
	//////////////////////////////
	// 1. super init first
	if (!Sprite::initWithFile(filename)) {
		return false;
	}

	this->setAnchorPoint(Vec2(0.0f, 0.5f));

	this->ray = new Ray(
		Vec3(std::cosf(this->getRotation()*M_PI/180.0f)*(180.0f/M_PI), -std::sinf(this->getRotation()*M_PI/180.0f)*(180.0f/M_PI), 0.0f),
		Vec3(this->getPositionX(), this->getPositionY(), 0.0f)
	);
	this->updateNeeded = false;

	//////////////////////////////////////////////////////////////////////
	//
	//  Box2D
	//
	//////////////////////////////////////////////////////////////////////

	// define the body definition

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.SetZero();
	bodyDef.angle = 0.0f;
	auto body = world->CreateBody(&bodyDef);

	// define the shape (rect)

	b2PolygonShape shape;
	std::vector<b2Vec2> vertices;
	vertices.push_back(b2Vec2(0.0f, 0.0f));
	vertices.push_back(b2Vec2(1.0f, 0.0f));
	vertices.push_back(b2Vec2(0.0f, 1.0f));
	shape.Set(vertices.data(), vertices.size());

	// define the filter

//	b2Filter filter;
//	filter.categoryBits;
//	filter.maskBits;
//	filter.groupIndex;

	// define the fixture definition

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.userData = nullptr;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.density = 1.0f;
	fixtureDef.isSensor = true;
//	fixtureDef.filter = filter;

	this->fixture = body->CreateFixture(&fixtureDef);

	return true;
}

/**
 *
 */
void Laser::setPosition(const Vec2& position) {
	Sprite::setPosition(position);
	this->getBox2DBody()->SetTransform(b2Vec2(position.x / Globals::getInstance().getBox2DScale(), position.y / Globals::getInstance().getBox2DScale()), -this->getRotation()*DEGTORAD);
	this->updateNeeded = true;
}

/**
 *
 */
void Laser::setPositionNormalized(const Vec2& position) {
	Sprite::setPositionNormalized(position);
	this->getBox2DBody()->SetTransform(b2Vec2(this->getParent()->getContentSize().width * position.x / Globals::getInstance().getBox2DScale(), this->getParent()->getContentSize().height * position.y / Globals::getInstance().getBox2DScale()), -this->getRotation()*DEGTORAD);
	this->updateNeeded = true;
}

/**
 *
 */
void Laser::setPosition(float x, float y) {
	Sprite::setPosition(x, y);
	this->getBox2DBody()->SetTransform(b2Vec2(x / Globals::getInstance().getBox2DScale(), y / Globals::getInstance().getBox2DScale()), -this->getRotation()*DEGTORAD);
	this->updateNeeded = true;
}

/**
 *
 */
void Laser::setPositionX(float x) {
	Sprite::setPositionX(x);
	this->getBox2DBody()->SetTransform(b2Vec2(x / Globals::getInstance().getBox2DScale(), this->getPositionY() / Globals::getInstance().getBox2DScale()), -this->getRotation()*DEGTORAD);
	this->updateNeeded = true;
}

/**
 *
 */
void Laser::setPositionY(float y) {
	Sprite::setPositionY(y);
	this->getBox2DBody()->SetTransform(b2Vec2(this->getPositionX() / Globals::getInstance().getBox2DScale(), y / Globals::getInstance().getBox2DScale()), -this->getRotation()*DEGTORAD);
	this->updateNeeded = true;
}

/**
 *
 */
void Laser::setRotation(float rotation) {
	Sprite::setRotation(rotation);
	this->getBox2DBody()->SetTransform(b2Vec2(this->getPositionX() / Globals::getInstance().getBox2DScale(), this->getPositionY() / Globals::getInstance().getBox2DScale()), -rotation*DEGTORAD);
	this->updateNeeded = true;
}

/**
 *
 */
float Laser::dist(const Plane& plane) const {
	return this->ray->dist(plane);
}

/**
 *
 */
Vec3 Laser::intersects(const Plane& plane) const {
	return this->ray->intersects(plane);
}

/**
 *
 */
void Laser::update(float dt) {
	if (this->needsUpdate()) {
		this->ray->set(Vec3(std::cosf(this->getRotation()*DEGTORAD)*RADTODEG, -std::sinf(this->getRotation()*DEGTORAD)*RADTODEG, 0.0f), Vec3(this->getPositionX(), this->getPositionY(), 0.0f));

		this->updateNeeded = false;
	}
}

