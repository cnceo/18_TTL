#include "NNPlayer.h"
#include "NNPlayerPanel.h"
#include "NNPlayerCard.h"
#include "ClientHN_THJ/Game/TTLNN/GameScene/NNGameScene.h"
#include "ClientHN_THJ/Game/TTLNN/Protocol/CMD_NN.h"
#include "ClientHN_THJ/Game/TTLNN/GameScene/NNRoomInfo.h"

NNPlayer::NNPlayer()
	:GamePlayer(NULL)
{
	m_Valid = false;
	m_VisioChairID = INVALID_CHAIR;
	zeromemory(&m_PlayerBets, sizeof(m_PlayerBets));
	m_Status = TTLNN::NNPlayerStatus_Invalid;
	m_SnatchBankerRatio = TTLNN::NNSnatchBanker_Invalid;

	m_wPlayCount = 0;
}

NNPlayer::~NNPlayer()
{

}

#pragma region 数据处理
void NNPlayer::clearGameData()
{
	zeromemory(&m_PlayerBets, sizeof(m_PlayerBets));
	m_SnatchBankerRatio = TTLNN::NNSnatchBanker_Invalid;
	m_Status = TTLNN::NNPlayerStatus_Invalid;
	zeromemory(&m_PlayerCards, sizeof(m_PlayerCards));
	zeromemory(&m_CardType, sizeof(m_CardType));
	zeromemory(&m_Calculate, sizeof(m_Calculate));
}

void NNPlayer::setValid(bool valid)
{
	m_Valid = valid;
}

bool NNPlayer::isValid()
{
	return m_Valid;
}

int NNPlayer::getVisioChairID()
{
	return m_VisioChairID;
}

void NNPlayer::setPlayerBets(TTLNN::NNPlayerBet& bets)
{
	m_PlayerBets = bets;
}

WORD NNPlayer::getPlayerBets()
{
	return m_PlayerBets.wBet*NNRoomInfo::Instance().getRoomInfo().dwBaseScore * NNGameScene::Instance().getBankerRatio() ;
}

void NNPlayer::setSnatchBankerRatio(WORD ratio)
{
	m_SnatchBankerRatio = ratio;
}

WORD NNPlayer::getSnatchBankerRatio()
{
	return m_SnatchBankerRatio;
}

void NNPlayer::setPlayerCards(BYTE cards[MAX_HAND_CARD], WORD count)
{
	m_PlayerCards = NNPlayerCard_Entity(cards, count);
}

void NNPlayer::addPlayerCard(BYTE card)
{
	if (m_PlayerCards.isValid) {
		m_PlayerCards.addCard(card);
	}
}

NNPlayerCard_Entity NNPlayer::getPlayerCards()
{
	return m_PlayerCards;
}

void NNPlayer::setPlayerCardType(TTLNN::NNCardType_Result result)
{
	m_CardType = result;
}

TTLNN::NNCardType_Result NNPlayer::getPlayerCardType()
{
	return m_CardType;
}

void NNPlayer::setPlayerSingleCalculate(int score)
{
	m_Calculate = NNPlayerSingleCalculate(score);
}

NNPlayerSingleCalculate NNPlayer::getPlayerSingleCalculate()
{
	return m_Calculate;
}

void NNPlayer::setPlayerStatus(TTLNN::NNPlayerStatus status)
{
	m_Status = status;
}

TTLNN::NNPlayerStatus NNPlayer::getPlayerStatus()
{
	return m_Status;
}

//游戏局数
void NNPlayer::setPlayCount(WORD wCount)
{
	m_wPlayCount = wCount;
}
WORD NNPlayer::getPlayCount()
{
	return m_wPlayCount;
}
#pragma endregion 数据处理


#pragma region 框架消息
void NNPlayer::PlayerEnter()
{
	m_wPlayCount = 0;
	m_Valid = true; 
	m_VisioChairID = NNGameScene::Instance().getVisioChairID(*this);
	m_Status = TTLNN::NNPlayerStatus_Sitting;
	upPlayerInfo();
}

void NNPlayer::PlayerLeave()
{
	m_Status = TTLNN::NNPlayerStatus_Invalid;
	NNPlayerPanel::Instance().hidePlayer(m_VisioChairID);
	NNPlayerCard::Instance().hidePlayer(m_VisioChairID);
	m_Valid = false;
	m_VisioChairID = INVALID_CHAIR;
}

void NNPlayer::upPlayerInfo()
{
	if (m_Valid) {
		NNPlayerPanel::Instance().showPlayer(*this);
		NNPlayerCard::Instance().showPlayer(*this);
	}
}

void NNPlayer::calculateInfo()
{
	if (m_Valid) {
		NNPlayerCard::Instance().calculatePlayer(*this);
	}
}

void NNPlayer::removeLastCard()
{
	if (m_Valid) {
		NNPlayerCard::Instance().removeLastCard(*this);
	}
}

void NNPlayer::upPlayerState()
{
	if (m_Valid) {
		if (GetUserID() == UserInfo::Instance().getUserID() && GetUserStatus() == US_READY) {
			NNGameScene::Instance().onReady();
		} else {
			NNPlayerPanel::Instance().showPlayer(*this);
			NNPlayerCard::Instance().showPlayer(*this);
		}
	}
}

void NNPlayer::showTalkState(CMD_GR_C_TableTalk* pNetInfo)
{
	NNPlayerPanel::Instance().playerTalk(*this, pNetInfo);
}
#pragma endregion 框架消息


