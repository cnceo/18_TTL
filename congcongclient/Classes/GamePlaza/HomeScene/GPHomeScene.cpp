#include "GPHomeScene.h"
#include "cocos2d/cocos/editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "GameLib/Game/Game/NoticeMsg.h"
#include "GameLib/Game/Game/UserInfo.h"
#include "GamePlaza/GameManager/GPGameLink.h"
#include "CommonHeader/LocalConstant.h"

USING_NS_CC;
using namespace experimental;

FV_SINGLETON_STORAGE(GPHomeScene);

GPHomeScene::GPHomeScene()
	: m_selectedItemIndex(0)
	//,m_pSpriteSpeaker(NULL)
	, avatar(NULL)
	, m_kPopularizeMission(ScriptData<std::string>("address").Value().c_str(), ScriptData<int>("Port").Value())
	, _nCurrentGameKindID(ScriptData<int>("GameKind").Value())
{
	m_kPopularizeMission.setMissionSink(this);
	UserInfo::Instance().addUpPlayerInfoCB(this, QY_CALLFUNC_SELECTOR(GPHomeScene::flushUserInfo));
	UserInfo::Instance().addLoginSucessCB(this, QY_CALLFUNC_SELECTOR(GPHomeScene::onLogonSucess));
	init();
}

GPHomeScene::~GPHomeScene()
{
}

bool GPHomeScene::init()
{
	if (!cocos2d::Node::init()) 
	{
		return false;
	}

	WidgetScenceXMLparse homeScript("GamePlaza/Script/GPHomeScene.xml", this);
	initButton();
	initPaoMaDeng();
	initPopupPanels();

	auto pos = WidgetFun::getChildWidgetByName(this,"Img_HeaderBG")->getPosition();
	avatar = GPSceneManager::getCircleAvatar(WidgetFun::getChildWidgetByName(this,"Img_HeaderBG"), "GamePlaza/HomeScene/avatar_male.png", Size(65, 65),pos);//Vec2(65, 647)
	WidgetFun::setImagic(WidgetFun::getChildWidgetByName(this,"Img_HeaderBG"),"GamePlaza/HomeScene/avatar_male.png",true);
	float sound_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("sound_volume", LocalContant::DEFAULT_SOUND);
	float effect_volume = cocos2d::UserDefault::getInstance()->getFloatForKey("effect_volume", LocalContant::DEFAULT_EFFECT);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(sound_volume);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(effect_volume);

	return true;
}

void GPHomeScene::showGameRoomList(void* data, size_t dataSize)
{
	cocos2d::log("GPHomeRoomListPanel_NN::RefreshListView");

	cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(this, "View_RoomList");
	pList->removeAllChildren();

	cocos2d::Node* pItemNode = nullptr;
	cocos2d::Node* pRoomInfo = nullptr;
	CMD_GR_INQURE_TABLES_INFO_DATA_BODY* body;

	int offset = 0;
	void *pData = data;
	char* current_data = static_cast<char*>(pData);

	//解析头
	int iGameID = (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_HEAD*>(pData))->dwGameKindID;
	if (iGameID != _nCurrentGameKindID)
		return;
	int iTableNum = (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_HEAD*>(pData))->dwTablesTotal;
	if (iTableNum == 0)
		return;
	offset = sizeof(CMD_GR_INQURE_TABLES_INFO_DATA_HEAD);	pData = current_data + offset;

	//解析每个子控件
	for (int i = 0; i < iTableNum; i++)
	{
		body = (static_cast<CMD_GR_INQURE_TABLES_INFO_DATA_BODY*>(pData));

		pItemNode = WidgetManager::Instance().createWidget("GameRoomInfoListViewItem", pList);
		pRoomInfo = WidgetFun::getChildWidgetByName(pItemNode, "Btn_RoomList_Join");

		if (pItemNode == nullptr)
			continue;
		//房间号
		std::string strRoomNo = utility::toString(body->dwTableNum);
		//局数
		std::string strRoundCount = utility::toString(body->dwOutMatchNum);
		//底分
		std::string strBaseSorce = utility::toString(body->dwBaseScore);
		//人数
		std::string strSitCout = utility::toString((dword)body->bSitUserSum, "/", (dword)body->bChairSum, utility::a_u8("人"));

		WidgetFun::setText(pRoomInfo, "Txt_RoomNo", strRoomNo);
		WidgetFun::setText(pRoomInfo, "Txt_RoundCount", strRoundCount);
		WidgetFun::setText(pRoomInfo, "Txt_Score", strBaseSorce);
		WidgetFun::setText(pRoomInfo, "Txt_PlayerCount", strSitCout);

		WidgetFun::setWidgetUserInfo(pItemNode, "Btn_Invitation", "RoomID", strRoomNo);
		WidgetFun::setWidgetUserInfo(pItemNode, "Btn_RoomList_Join", "RoomID", strRoomNo);

		WidgetFun::setVisible(pItemNode, "GameRoomInfoListViewItem", true);

		offset += sizeof(CMD_GR_INQURE_TABLES_INFO_DATA_BODY);	pData = current_data + offset;
	}
	pList->forceDoLayout();
}

void GPHomeScene::initPaoMaDeng()
{
	playPaoMaDeng(WidgetFun::getChildWidgetByName(this,"Speaker"),"SpeakerTxt","SpeakerLayoutNode", utility::a_u8("果酱娱乐欢迎您！"),NULL);
}

void GPHomeScene::flushPlayerLevel()
{
	int experInt = CGlobalUserInfo::GetInstance()->GetGlobalUserData()->dwExperience;
	std::string levelName ="禁军";
	if ( experInt >= 6400) 
		levelName ="太尉";
	else if ( experInt >= 3200) 
		levelName ="司马";
	else if ( experInt >= 1600) 
		levelName ="大将军";
	else if ( experInt >= 800) 
			levelName ="参军";
	else if (experInt >= 400) 
		levelName = "都尉";
	else if (experInt >= 200) 
		levelName = "抚军";
	else if (experInt >= 100) 
		levelName = "教头";
	else if (experInt >= 50) 
		levelName = "提辖";
	else levelName = "禁军";
	WidgetFun::setText(this,"Home_UserLV",utility::a_u8(utility::toString(levelName)));
}

void GPHomeScene::playPaoMaDeng(cocos2d::Node* pRootNode,std::string kTxtNode,std::string kLayoutNode,std::string kTxt,cocos2d::FiniteTimeAction* pNetAction)
{
	cocos2d::Node* pTxt = WidgetFun::getChildWidget(pRootNode,kTxtNode);
	cocos2d::Node* pLayoutNode = WidgetFun::getChildWidget(pRootNode,kLayoutNode);
	WidgetFun::setText(pTxt,kTxt);
	float fMaxHeight = pLayoutNode->getContentSize().height;
	float fMaxWidth = pLayoutNode->getContentSize().width;
	pTxt->setPositionX(fMaxWidth);
	pTxt->stopAllActions();
	pTxt->setVisible(true);
	auto funBack = cocos2d::CCCallFunc::create([=](){
		std::string msg = NoticeMsg::Instance().getSystemMessage();
		WidgetFun::setText(pTxt,utility::a_u8(msg));
		float fTxtWidth = pTxt->getContentSize().width;
		float fPosX =pTxt->getPositionX();
		if (fPosX < -fTxtWidth){
			pTxt->setPositionX(fMaxWidth);
		}else{
			pTxt->setPositionX(fPosX-1);
		}
	});
	cocos2d::CCAction * seq1 = cocos2d::CCSequence::create(
		cocos2d::CCDelayTime::create(0.001f),
		funBack,
		NULL);
	auto rep = cocos2d::CCRepeatForever::create(dynamic_cast<cocos2d::ActionInterval*>(seq1));
	pTxt->runAction(rep);
}

//////////////////////////////////////////////////////////////////////////

void GPHomeScene::onEnterScene()
{
	initPublicPanel();
	//////////////////////////////////////////////////////////////////////////
	hideAllPanels();
	flushUserInfo();

	SoundFun::Instance().playBackMusic("bgplay.mp3");
}

bool GPHomeScene::initPublicPanel()
{
	if (GPHomeSettingPanel::pInstance()->getParent() != this) {
		GPHomeSettingPanel::pInstance()->removeFromParent();
		addPanel(GPHomeSettingPanel::pInstance());
	}
	if (GPHomeMallPanel::pInstance()->getParent() != this) {
		GPHomeMallPanel::pInstance()->removeFromParent();
		addPanel(GPHomeMallPanel::pInstance());
	}
	if (GPHomeSharePanel::pInstance()->getParent() != this) {
		GPHomeSharePanel::pInstance()->removeFromParent();
		addPanel(GPHomeSharePanel::pInstance());
	}
	return true;
}

void GPHomeScene::initPopupPanels()
{
	initPublicPanel();

	//////////////////////////////////////////////////////////////////////////

	addPanel(GPHomeUserInfoPanel::pInstance());
	addPanel(GPHomeEnterRoomPanel::pInstance());
	addPanel(GPHomeRecordPanel::pInstance());
	addPanel(GPHomeRankPanel::pInstance());
	addPanel(GPHomeEditNumPanel::pInstance());

	//////////////////////////////////////////////////////////////////////////

	addPanel(GPHomeCreateRoomPanel_NN::pInstance());
	//DONE:增加结算面板
	addPanel(NNCalculate::pInstance());
	NNCalculate::pInstance()->hide();

	//////////////////////////////////////////////////////////////////////////

	//DONE:没有用的
	//addPanel(GPHomePromotionPanel::pInstance());
	//addPanel(GPHomeMorePanel::pInstance());
	//addPanel(GPHomeServicePanel::pInstance());
}

//DONE:实际没有被调用过
void GPHomeScene::initGamePanels()
{
	//三人两房
	addPanel(GPHomeRoomListPanel_SRLF::pInstance());
	addPanel(GPHomeCreateRoomPanel_SRLF::pInstance());
	//牛牛
	addPanel(GPHomeRoomListPanel_NN::pInstance());
	//addPanel(GPHomeCreateRoomPanel_NN::pInstance());
	//经典三张
	addPanel(GPHomeRoomListPanel_TLJ::pInstance());
	addPanel(GPHomeCreateRoomPanel_TLJ::pInstance());
	//三公房间
	addPanel(GPHomeRoomListPanel_SG::pInstance());
	addPanel(GPHomeCreateRoomPanel_SG::pInstance());
	//五子棋房间
	addPanel(GPHomeRoomListPanel_WZQ::pInstance());
	//换三张房间
	addPanel(GPHomeRoomListPanel_XZDD::pInstance());
	addPanel(GPHomeCreateRoomPanel_XZDD::pInstance());
}

void GPHomeScene::addPanel(cocos2d::Node* pNode)
{
	this->addChild(pNode);
}

void GPHomeScene::hideAllPanels()
{
	if (GPHomeSettingPanel::pInstance()->getParent() == this) {
		GPHomeSettingPanel::pInstance()->hide();
	}
	if (GPHomeSharePanel::pInstance()->getParent() == this) {
		GPHomeSharePanel::pInstance()->hide();
	}
	if (GPHomeUserInfoPanel::pInstance()->getParent() == this) {
		GPHomeUserInfoPanel::pInstance()->hide();
	}
	if (GPHomeEnterRoomPanel::pInstance()->getParent() == this) {
		GPHomeEnterRoomPanel::pInstance()->hide();
	}
	if (GPHomeMallPanel::pInstance()->getParent() == this) {
		GPHomeMallPanel::pInstance()->hide();
	}
	if (GPHomeRecordPanel::pInstance()->getParent() == this) {
		GPHomeRecordPanel::pInstance()->hide();
	}
	if (GPHomeRankPanel::pInstance()->getParent() == this) {
		GPHomeRankPanel::pInstance()->hide();
	}
	if (GPHomeCreateRoomPanel_NN::pInstance()->getParent() == this) {
		GPHomeCreateRoomPanel_NN::pInstance()->hide();
	}
	
	//输入房间键盘
	if (GPHomeEditNumPanel::pInstance()->getParent() == this) {
		GPHomeEditNumPanel::pInstance()->hide();
	}

	//DONE:不需要隐藏结算面板
	//if (NNCalculate::pInstance()->getParent() == this) {
	//	NNCalculate::pInstance()->hide();
	//}

	//if (GPHomePromotionPanel::pInstance()->getParent() == this) {
	//	GPHomePromotionPanel::pInstance()->hide();
	//}
	//if (GPHomeMorePanel::pInstance()->getParent() == this) {
	//	GPHomeMorePanel::pInstance()->hide();
	//}
	//if (GPHomeServicePanel::pInstance()->getParent() == this) {
	//	GPHomeServicePanel::pInstance()->hide();
	//}
}

//DONE:没被调用过了
void GPHomeScene::hideGamePanels()
{
	//三人两房房间列表
	if (GPHomeRoomListPanel_SRLF::pInstance()->getParent() == this) {
		GPHomeRoomListPanel_SRLF::pInstance()->hide();
	}
	//三人两房房间列表
	if (GPHomeCreateRoomPanel_SRLF::pInstance()->getParent() == this) {
		GPHomeCreateRoomPanel_SRLF::pInstance()->hide();
	}
	//牛牛房间
	if (GPHomeRoomListPanel_NN::pInstance()->getParent() == this) {
		GPHomeRoomListPanel_NN::pInstance()->hide();
	}
	//拼十创建房间
	if (GPHomeCreateRoomPanel_NN::pInstance()->getParent() == this) {
		GPHomeCreateRoomPanel_NN::pInstance()->hide();
	}
	//经典三张房间
	if (GPHomeRoomListPanel_TLJ::pInstance()->getParent() == this) {
		GPHomeRoomListPanel_TLJ::pInstance()->hide();
	}
	//经典三张创建房间
	if (GPHomeCreateRoomPanel_TLJ::pInstance()->getParent() == this) {
		GPHomeCreateRoomPanel_TLJ::pInstance()->hide();
	}
	//三公房间
	if (GPHomeRoomListPanel_SG::pInstance()->getParent() == this) {
		GPHomeRoomListPanel_SG::pInstance()->hide();
	}
	if (GPHomeCreateRoomPanel_SG::pInstance()->getParent() == this) {
		GPHomeCreateRoomPanel_SG::pInstance()->hide();
	}
	//五子棋房间
	if (GPHomeRoomListPanel_WZQ::pInstance()->getParent() == this) {
		GPHomeRoomListPanel_WZQ::pInstance()->hide();
	}
	//五子棋房间
	if (GPHomeRoomListPanel_XZDD::pInstance()->getParent() == this) {
		GPHomeRoomListPanel_XZDD::pInstance()->hide();
	}
	if (GPHomeCreateRoomPanel_XZDD::pInstance()->getParent() == this) {
		GPHomeCreateRoomPanel_XZDD::pInstance()->hide();
	}
}

//DONE:没被调用过了
void GPHomeScene::removeAllPanels()
{
	if (GPHomeSettingPanel::pInstance()->getParent() == this) {
		removeChild(GPHomeSettingPanel::pInstance());
	}
	if (GPHomeSharePanel::pInstance()->getParent() == this) {
		removeChild(GPHomeSharePanel::pInstance());
	}
	if (GPHomeUserInfoPanel::pInstance()->getParent() == this) {
		removeChild(GPHomeUserInfoPanel::pInstance());
	}
	if (GPHomeEnterRoomPanel::pInstance()->getParent() == this) {
		removeChild(GPHomeEnterRoomPanel::pInstance());
	}
	if (GPHomeMallPanel::pInstance()->getParent() == this) {
		removeChild(GPHomeMallPanel::pInstance());
	}
	if (GPHomeRecordPanel::pInstance()->getParent() == this) {
		removeChild(GPHomeRecordPanel::pInstance());
	}
	if (GPHomeRankPanel::pInstance()->getParent() == this) {
		removeChild(GPHomeRankPanel::pInstance());
	}
	//牛牛还是要干掉
	if (GPHomeCreateRoomPanel_NN::pInstance()->getParent() == this) {
		removeChild(GPHomeCreateRoomPanel_NN::pInstance());
	}

	//DONE:需要删除掉下列房间列表
	//removeGamepanels();
	//if (GPHomePromotionPanel::pInstance()->getParent() == this) {
	//	removeChild(GPHomePromotionPanel::pInstance());
	//}
	//if (GPHomeMorePanel::pInstance()->getParent() == this) {
	//	removeChild(GPHomeMorePanel::pInstance());
	//}
	//if (GPHomeServicePanel::pInstance()->getParent() == this) {
	//	removeChild(GPHomeServicePanel::pInstance());
	//}
}

void GPHomeScene::showGameCalculate(CMD_GF_Private_End_Info* pNetInfo)
{
	NNCalculate::Instance().show(pNetInfo);
}

//DONE:没被调用过了
void GPHomeScene::removeGamepanels()
{
	if (GPHomeRoomListPanel_SRLF::pInstance()->getParent() == this) {
		removeChild(GPHomeRoomListPanel_SRLF::pInstance());
	}
	if (GPHomeCreateRoomPanel_SRLF::pInstance()->getParent() == this) {
		removeChild(GPHomeCreateRoomPanel_SRLF::pInstance());
	}
	if (GPHomeRoomListPanel_NN::pInstance()->getParent() == this) {
		removeChild(GPHomeRoomListPanel_NN::pInstance());
	}
	if (GPHomeCreateRoomPanel_NN::pInstance()->getParent() == this) {
		removeChild(GPHomeCreateRoomPanel_NN::pInstance());
	}
	if (GPHomeRoomListPanel_TLJ::pInstance()->getParent() == this) {
		removeChild(GPHomeRoomListPanel_TLJ::pInstance());
	}
	if (GPHomeCreateRoomPanel_TLJ::pInstance()->getParent() == this) {
		removeChild(GPHomeCreateRoomPanel_TLJ::pInstance());
	}
	//三公房间
	if (GPHomeRoomListPanel_SG::pInstance()->getParent() == this) {
		removeChild(GPHomeRoomListPanel_SG::pInstance());
	}
	if (GPHomeCreateRoomPanel_SG::pInstance()->getParent() == this) {
		removeChild(GPHomeCreateRoomPanel_SG::pInstance());
	}
	//五子棋房间
	if (GPHomeRoomListPanel_WZQ::pInstance()->getParent() == this) {
		removeChild(GPHomeRoomListPanel_WZQ::pInstance());
	}
	//五子棋房间
	if (GPHomeRoomListPanel_XZDD::pInstance()->getParent() == this) {
		removeChild(GPHomeRoomListPanel_XZDD::pInstance());
	}
	if (GPHomeCreateRoomPanel_XZDD::pInstance()->getParent() == this) {
		removeChild(GPHomeCreateRoomPanel_XZDD::pInstance());
	}
	if (GPHomeEditNumPanel::pInstance()->getParent() == this) {
		removeChild(GPHomeEditNumPanel::pInstance());
	}
}

void GPHomeScene::onLogonSucess()
{
	UserInfo::Instance().reqAccountInfo();
}

void GPHomeScene::flushUserInfo()
{
	WidgetFun::setText(this, "Home_UserID", utility::paseInt(UserInfo::Instance().getUserID(), 6));
	WidgetFun::setText(this, "Home_UserName",UserInfo::Instance().getUserNicName());
	//WidgetFun::setText(this, "Home_RoomCardNum",UserInfo::Instance().getUserInsure());
	WidgetFun::setText(this, "Home_GoldNum",UserInfo::Instance().getUserScore());
	flushPlayerLevel();
	ImagicDownManager::Instance().addDown(avatar,UserInfo::Instance().getHeadHttp(), UserInfo::Instance().getUserID());
}

#pragma region CGPopularizeSink
void GPHomeScene::onGPNoticeResult(tagGameMatch&	pGameMatchInfo, bool bSucess, const char* pStr)
{
	NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pStr));
}

//DONE:没被调用过了
void GPHomeScene::onGPPopularizeHttpRes(const CMD_GP_PopularizeHttpRes& pNetInfo)
{
	auto pNode = WidgetFun::getChildWidgetByName(this,"Img_NoticeView");
	auto pNode_Notice = WidgetFun::getChildWidgetByName(this,"Node_Notice");
	pNode->setVisible(false);
	auto Pos   = pNode->getPosition();
	return;

	Size visibleSize = Director::getInstance()->getVisibleSize();  
	Vec2 origin = Director::getInstance()->getVisibleOrigin();  
	Size winSize = Director::getInstance()->getWinSize(); 
	Size curPageSize = Size(300,419);
	auto pPageView = cocos2d::ui::PageView::create();
	pPageView->setTouchEnabled(true);  
	//pPageView->setSize(Size(winSize.width/2, winSize.height/2));  
	pPageView->setSize(curPageSize);  
	pPageView->setPosition(-Vec2(curPageSize.width/2,curPageSize.height/2));
	//创建4个page  
	for (int i=1; i<6; i++) {  
		cocos2d::ui::Layout* layout = cocos2d::ui::Layout::create();  
		//layout->setSize(Size(winSize.width, winSize.height));  
		layout->setSize(curPageSize);
		auto kFileName = utility::toString(cocos2d::FileUtils::getInstance()->getWritablePath(),"GuangGao_",i,".png");
		cocos2d::ui::ImageView* imageView = cocos2d::ui::ImageView::create(kFileName);
		//ImagicDownManager::Instance().addDown(imageView,utility::toString(vStr[0],i,".png"),utility::toString("GuangGao_",i),true);
		//imageView->setScale9Enabled(true);  
		//imageView->setSize(Size(400, 400));  
		imageView->setPosition(Point(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));  
		layout->addChild(imageView);  
		pPageView->addPage(layout);  
	}
	//pPageView->addEventListener((cocos2d::ui::PageView::ccPageViewCallback)CC_CALLBACK_2(GPHomeScene::pageViewEvent,this));
	//pPageView->addEventListenerPageView(this,pagevieweventselector(GPHomeScene::pageViewEvent));  
	pNode_Notice->addChild(pPageView);  
}

#pragma endregion
