#pragma once

#include "CommonHeader/CommonHeader.h"
#include "NNPlayer.h"

class NNPlayerCard
	: public cocos2d::Node
	, public FvSingleton<NNPlayerCard>
{
public:
	const static int CARD_DWON_POSY = 80;
	const static int CARD_UP_POSY = 110;
public:
	NNPlayerCard();
	~NNPlayerCard();

#pragma region ��ʼ��
public:
	bool init();
	void initLayout();
#pragma endregion ��ʼ��

#pragma region ��ʾ������
public:
	void show();
	void hide();

	void showPlayer(NNPlayer& player);
	void hidePlayer(WORD visioChairID);
	void calculatePlayer(NNPlayer& player);
#pragma endregion ��ʾ������

#pragma region ���ƶ���
	void sendPlayerCard();
	void sendPlayerCardAdd();
	void startOrbitAction(cocos2d::Sprite* sprite, int cardIndex);
	void removeLastCard(NNPlayer& player);
#pragma endregion ���ƶ���

#pragma region �����ж�
	int getTouchCard(cocos2d::Vec2 kTouchPos);
#pragma endregion �����ж�

private:
	std::vector<cocos2d::Sprite*> m_TempCards;
};