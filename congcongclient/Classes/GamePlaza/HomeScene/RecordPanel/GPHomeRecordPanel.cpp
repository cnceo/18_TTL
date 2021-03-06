#include "GPHomeRecordPanel.h"
#include "Game/GameLib.h"


FV_SINGLETON_STORAGE(GPHomeRecordPanel);
GPHomeRecordPanel::GPHomeRecordPanel()
	: m_GameRecordMission(ScriptData<std::string>("address").Value().c_str(), ScriptData<int>("Port").Value())
{
	init();
	m_GameRecordMission.setMissionSink(this);
}

GPHomeRecordPanel::~GPHomeRecordPanel()
{

}

bool GPHomeRecordPanel::init()
{
	if (!cocos2d::Node::init()) {
		return false;
	}
	
	WidgetScenceXMLparse exitPanel("GamePlaza/Script/GPHomeRecordPanel.xml", this);
	WidgetManager::addButtonCB("Button_Close", this, button_selector(GPHomeRecordPanel::Button_Cancel));
	return true;
}

void GPHomeRecordPanel::show()
{
	setVisible(true);
	//m_GameRecordMission.GetGameRecordList(UserInfo::Instance().getUserID());
	m_GameRecordMission.GetGameRecordListEx(UserInfo::Instance().getUserID());
}

void GPHomeRecordPanel::hide()
{
	setVisible(false);
}

#pragma region 回放数据回调

void GPHomeRecordPanel::onGPBackGameRecordListEx(tagGameRecordListEx* pNetInfo)
{
	cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(this, "RecordList0");
	pList->removeAllChildren();
	for (int i = 0; i < (int)pNetInfo->kList.size(); i++) {
		tagGameRecordListItem& kInfo = pNetInfo->kList.at(i);
		if (kInfo.dwKindID!=21 && kInfo.dwKindID!=888  && kInfo.dwKindID!=999 && kInfo.dwKindID!=317 && kInfo.dwKindID!=302 && kInfo.dwKindID!=303)
		{
			continue;
		}
		cocos2d::Node* pItemNode = WidgetManager::Instance().createWidget("GameReocrdListItem", pList);
		pItemNode->setPositionX(100);
		//WidgetFun::setButtonImagic(pItemNode, "Button_ShowTotal",WidgetFun::getWidgetUserInfo(WidgetFun::getChildWidgetByName(pItemNode, "Button_ShowTotal"),utility::toString("GameKINDID_",kInfo.dwKindID)),true);
		WidgetFun::setText(pItemNode, "Txt_GameType",WidgetFun::getWidgetUserInfo(WidgetFun::getChildWidgetByName(pItemNode, "Button_ShowTotal"),utility::toString("GameKINDID_",kInfo.dwKindID)));
		WidgetFun::setText(pItemNode, "ChallengeTime", timeToString(kInfo.kPlayTime));
		WidgetFun::setText(pItemNode, "RoomNum", utility::toString(kInfo.iRoomNum));
		if (kInfo.kNickName.size() != kInfo.kScore.size()) {
			continue;
		}
		for (int m = 0; m < (int)kInfo.kNickName.size(); m++) {
			if (m >= 6) {
				continue;
			}
			WidgetFun::setText(pItemNode, utility::toString("PlayerScore", m),scoreToString(kInfo.kScore[m]));
			WidgetFun::setText(pItemNode, utility::toString("PlayerTxt", m),utility::toString(utility::a_u8(kInfo.kNickName[m])));
		}
	}
	pList->forceDoLayout();

}


void GPHomeRecordPanel::onGPBackGameRecordList(tagPrivateRandTotalRecordList* pNetInfo)
{
	m_RecordList = *pNetInfo;	
	cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(this, "RecordList0");
	pList->removeAllChildren();
	for (int i = 0; i < (int)pNetInfo->kList.size(); i++) {
		tagPrivateRandTotalRecord& kInfo = pNetInfo->kList[i];
		cocos2d::Node* pItemNode = WidgetManager::Instance().createWidget("GameReocrdListItem", pList);
		WidgetFun::setWidgetUserInfo(pItemNode, "Button_ShowTotal", "RecordID", utility::toString(kInfo.iRecordID));
		WidgetFun::setText(pItemNode, "ChallengeTime", timeToString(kInfo.kPlayTime));
		WidgetFun::setText(pItemNode, "RoomNum", utility::toString(kInfo.iRoomNum));
		if (kInfo.kNickName.size() != kInfo.kScore.size()) {
			continue;
		}
		for (int m = 0; m < (int)kInfo.kNickName.size(); m++) {
			if (m >= 4) {
				continue;
			}
			WidgetFun::setText(pItemNode, utility::toString("PlayerScore", m),scoreToString(kInfo.kScore[m]));
			WidgetFun::setText(pItemNode, utility::toString("PlayerTxt", m),utility::toString(utility::a_u8(kInfo.kNickName[m])));
		}
	}
	pList->forceDoLayout();
}

void GPHomeRecordPanel::onGPBackGameTotalRecord(tagPrivateRandTotalRecord* pNetInfo)
{
	m_kPrivateRandTotalRecord = *pNetInfo;
	cocos2d::Node* pRootPlane = WidgetFun::getChildWidget(this, "RecordTotalPlane");
	cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(pRootPlane, "RecordList1");
	pList->removeAllChildren();

	for (int i = 0; i < (int)pNetInfo->kNickName.size(); i++) {
		if (i < 4) {
			WidgetFun::setText(pRootPlane, utility::toString("PlayerName", i), utility::a_u8(pNetInfo->kNickName[i]));
		}
	}

	for (int i = 0; i < (int)pNetInfo->kRecordChild.size(); i++) {
		tagPrivateRandRecordChild& kInfo = pNetInfo->kRecordChild[i];
		cocos2d::Node* pItemNode = WidgetManager::Instance().createWidget("GameReocrdTotalItem", pList);
		WidgetFun::setText(pItemNode, "IdexTxt", utility::toString(i + 1));
		WidgetFun::setText(pItemNode, "PlayTime", timeToString(kInfo.kPlayTime));
		WidgetFun::setWidgetUserInfo(pItemNode, "Button_StartGameRecord", "RecordChildID", utility::toString(kInfo.iRecordChildID));

		for (int m = 0; m < (int)kInfo.kScore.size(); m++) {
			if (m >= 4) {
				continue;
			}

			WidgetFun::setText(pItemNode, utility::toString("PlayerTxt", m), scoreToString(kInfo.kScore[m]));
		}

	}

	pList->forceDoLayout();
}

void GPHomeRecordPanel::onGPBackGameChildRecord(tagPrivateRandRecordChild* pNetInfo)
{
	GPGameManager::Instance().StartHNMJRecord(pNetInfo->kRecordGame);
}
#pragma endregion 回放数据回调


void GPHomeRecordPanel::Button_Cancel(cocos2d::Ref* pRef, WidgetUserInfo * pUserInfo)
{
	hide();
}

std::string GPHomeRecordPanel::timeToString(systemtime kSystem)
{
	std::string kStr;
	kStr = utility::toString(kStr, kSystem.wYear);
	kStr = utility::toString(kStr, "-");
	kStr = utility::toString(kStr, utility::paseInt(kSystem.wMonth, 2));
	kStr = utility::toString(kStr, "-");
	kStr = utility::toString(kStr, utility::paseInt(kSystem.wDay, 2));
	kStr = utility::toString(kStr, '\n');
	kStr = utility::toString(kStr, utility::paseInt(kSystem.wHour, 2));
	kStr = utility::toString(kStr, ":");
	kStr = utility::toString(kStr, utility::paseInt(kSystem.wMinute, 2));
	kStr = utility::toString(kStr, ":");
	kStr = utility::toString(kStr, utility::paseInt(kSystem.wSecond, 2));
	return kStr;
}

std::string GPHomeRecordPanel::scoreToString(int iScore)
{
	if (iScore > 0) {
		return utility::toString("+", iScore);
	}

	return utility::toString(iScore);
}

void GPHomeRecordPanel::Button_ShowTotal(cocos2d::Ref*, WidgetUserInfo* pUserInfo)
{
	//WidgetFun::setVisible(this, "RecordTotalPlane", true);
	//WidgetFun::setVisible(this, "RecordListPlane", false);
	//m_GameRecordMission.GetPrivateRandTotalRecord(utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo, "RecordID")));
}

void GPHomeRecordPanel::Button_StartGameRecord(cocos2d::Ref*, WidgetUserInfo* pUserInfo)
{
	int iRecordChildID = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo, "RecordChildID"));
	for (int i = 0; i < (int)m_kPrivateRandTotalRecord.kRecordChild.size(); i++) {
		tagPrivateRandRecordChild& kChild = m_kPrivateRandTotalRecord.kRecordChild[i];
		if (kChild.iRecordChildID == iRecordChildID) {
			GPGameManager::Instance().StartHNMJRecord(kChild.kRecordGame);
		}
	}
}
