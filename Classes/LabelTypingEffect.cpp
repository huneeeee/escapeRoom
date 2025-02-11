//
//  LabelTypingEffect.cpp
//  
//
//  Created by Chanwoo Noh on 2016. 7. 4..
//
//

#include "LabelTypingEffect.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif
#define DEFAULT_INTERVAL 0.02f
#define DEFAULT_COLOR    Color3B::WHITE

void LabelTypingEffect::typeMessage(cocos2d::Label* txtBox, const std::string& strChatMessage) {
	typeMessage(txtBox, strChatMessage, DEFAULT_INTERVAL, DEFAULT_COLOR, []() {});
}

void LabelTypingEffect::typeMessage(cocos2d::Label* txtBox, const std::string& strChatMessage, cocos2d::Color3B txtColor) {
	typeMessage(txtBox, strChatMessage, DEFAULT_INTERVAL, txtColor, []() {});
}

void LabelTypingEffect::typeMessage(cocos2d::Label* txtBox, const std::string& strChatMessage, float interval) {
	typeMessage(txtBox, strChatMessage, interval, DEFAULT_COLOR, []() {});
}
void LabelTypingEffect::typeMessage(Label* txtBox, const std::string& strChatMessage, float interval, Color3B txtColor) {
	typeMessage(txtBox, strChatMessage, interval, txtColor, []() {});
}

void LabelTypingEffect::typeMessage(cocos2d::Label* txtBox, const std::string& strChatMessage, float interval, const std::function<void()>& fncCallback) {
	typeMessage(txtBox, strChatMessage, interval, DEFAULT_COLOR, fncCallback);
}

void LabelTypingEffect::typeMessage(cocos2d::Label* txtBox, const std::string& strChatMessage, float interval, cocos2d::Color3B txtColor, const std::function<void()>& fncCallback) {
	txtBox->setVisible(false);
	txtBox->unschedule("typeMessage");
	txtBox->setString(strChatMessage);

	static std::map<Label*, int> mapTypeIdx;
	std::map<Label*, int>::iterator it;
	it = mapTypeIdx.find(txtBox);
	if (it == mapTypeIdx.end()) {
		std::pair<std::map<Label*, int>::iterator, bool> ret = mapTypeIdx.insert(std::pair<Label*, int>(txtBox, 0));
		if (!ret.second) {
			cocos2d::log("std::map insert failed");
		}
		else {
			it = mapTypeIdx.find(txtBox);
			if (it == mapTypeIdx.end()) {
				cocos2d::log("oops, we have some problem");
			}
		}
	}
	else {
		it->second = 0;
	}

	int max = txtBox->getStringLength();
	for (int i = 0; i < max; ++i) {
		if (txtBox->getLetter(i) != nullptr) // 스페이스바 예외처리
			txtBox->getLetter(i)->setOpacity(0);
		else if (txtBox->getLetter(i) == nullptr)
			continue;

		txtBox->getLetter(i)->setOpacity(0);
	}
	txtBox->setVisible(true);

	txtBox->schedule([txtBox, max, fncCallback, txtColor, it](float dt) {
		if (txtBox->getLetter(it->second) != nullptr) // 스페이스바 예외처리
		{
			txtBox->getLetter(it->second)->setOpacity(255);
			txtBox->getLetter(it->second)->setColor(txtColor);
		}
		it->second++;
		if (it->second == max) {
			txtBox->unschedule("typeMessage");
			mapTypeIdx.erase(it);
			fncCallback();
		}
	}, interval, max - 1, 0, "typeMessage");
}
