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

#include "cocos2d.h"
#include "WinCondition.h"

USING_NS_CC;

/**
 *
 */
WinCondition::WinCondition() {
}

/**
 *
 */
WinCondition::~WinCondition() {
}

/**
 *
 */
void WinCondition::addEmitterActivation(Emitter* emitter, bool active) {
	this->emitterActiveWinConditions[emitter] = active;
}

/**
 *
 */
void WinCondition::addMirrorDirection(Mirror* mirror, Direction direction) {
	this->mirrorDirectionWinConditions[mirror] = direction;
}

/**
 *
 */
void WinCondition::addBonusStar(BonusStar* bonusStar) {
	this->bonusStars.insert(bonusStar);
}

/**
 *
 */
std::set<BonusStar*> WinCondition::getBonusStars() {
	return this->bonusStars;
}

/**
 * Check all objects for their win condition
 */
bool WinCondition::evaluate() {
	// check the emitters active state for win condition
	for (std::map<Emitter*, bool>::iterator itr=emitterActiveWinConditions.begin(); itr!=emitterActiveWinConditions.end(); ++itr) {
		Emitter* const emitter = itr->first;
		if (emitter->isActive() != itr->second) {
			return false;
		}
	}

	// check the mirrors direction state for win condition
	for (std::map<Mirror*, Direction>::iterator itr=mirrorDirectionWinConditions.begin(); itr!=mirrorDirectionWinConditions.end(); ++itr) {
		Mirror* const mirror = itr->first;
		if (mirror->getDirection() != itr->second) {
			return false;
		}
	}

	return true;
}

