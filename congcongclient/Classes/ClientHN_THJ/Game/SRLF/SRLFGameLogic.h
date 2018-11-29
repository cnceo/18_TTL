#pragma once

#include "CMD_SRLF.h"
#include "Game/Game/GameDefine.h"
#include "Game/FV/FvSingleton.h"

//////////////////////////////////////////////////////////////////////////
using namespace SRLF;
//////////////////////////////////////////////////////////////////////////
namespace SRLFLOGIC
{

	//�߼�����
#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

	//////////////////////////////////////////////////////////////////////////
	//��������

	//������־
#define WIK_NULL					0x00								//û������
#define WIK_LEFT					0x01								//�������
#define WIK_CENTER					0x02								//�г�����
#define WIK_RIGHT					0x04								//�ҳ�����
#define WIK_PENG					0x08								//��������
#define WIK_GANG					0x10								//��������
#define WIK_LISTEN					0x20								//��������
#define WIK_CHI_HU					0x40								//�Ժ�����
#define WIK_ZI_MO					0x80								//����

//////////////////////////////////////////////////////////////////////////
//���ƶ���

//����
#define CHK_NULL					0x00										//�Ǻ�����
#define CHK_CHI_HU				    0x01										//������

#define CHR_DA_DUI_ZI				0x00000001									//�����		1��
#define CHR_QING_YI_SE				0x00000002									//��һɫ		2��
#define CHR_QI_XIAO_DUI			    0x00000004									//�߶�		2��
#define CHR_LONG_QI_DUI			    0x00000008									//���߶�		3��
#define CHR_QING_DUI				0x00000010									//���		3��
#define CHR_DAI_YAO				    0x00000020									//ȫ�۾�		3��
#define CHR_JIANG_DUI				0x00000040									//����		3��

#define CHR_QING_QI_DUI			    0x00000080									//���߶�		4��
#define CHR_QING_LONG_QI_DUI		0x00000100									//�����߶�	5��
#define CHR_JIANG_QI_DUI			0x00000200									//��	�߶�		5��
#define CHR_TIAN_HU				    0x00000400									//���		5��
#define CHR_DI_HU					0x00000800									//�غ�		5��
#define CHR_QING_YAO_JIU			0x00001000									//���۾�		5��

#define CHR_QIANG_GANG				0x00002000									//���ܺ�		1��
#define CHR_GANG_SHANG_PAO			0x00004000									//������		1��
#define CHR_GANG_KAI				0x00008000									//���ϻ�		1��
#define CHR_JIN_GOU_DAO			    0x00010000									//�𹳵�		1��
#define CHR_HAI_DI_HU				0x00020000									//������		1��
#define CHR_HAI_DI_PAO				0x00040000									//������		1��

#define CHR_ZI_MO					0x00080000									//����		
#define CHR_FANG_PAO				0x00100000									//����	
#define CHR_PIN_HU			    	0x00200000									//ƽ��	
#define CHR_CHA_DA_JIAO			    0x00400000									//����

#define CHR_PENGPENGHU				0x00800000									//��	
#define CHR_QINGYISE			    0x01000000									//��	
#define CHR_QINGPENG			    0x02000000									//����
#define CHR_MENQiNG			        0x04000000									//����

	//�˿˶���
#define HEAP_FULL_COUNT				26									//����ȫ��

#define MAX_RIGHT_COUNT				1	
	//////////////////////////////////////////////////////////////////////////



	//��������
	struct tagKindItem
	{
		BYTE							cbWeaveKind;						//�������
		BYTE							cbCenterCard;						//�����˿�
		BYTE							cbCardIndex[3];						//�˿�����
		BYTE							cbValidIndex[3];					//ʵ���˿�����
	};

	//�������
	struct tagWeaveItem
	{
		BYTE							cbWeaveKind;						//�������
		BYTE							cbCenterCard;						//�����˿�
		BYTE							cbPublicCard;						//������־
		WORD							wProvideUser;						//��Ӧ�û�
	};

	//���ƽ��
	struct tagGangCardResult
	{
		BYTE							cbCardCount;						//�˿���Ŀ
		BYTE							cbCardData[4];						//�˿�����
	};

	//��������
	struct tagAnalyseItem
	{
		BYTE							cbCardEye;							//�����˿�
		bool                            bMagicEye;                          //�����Ƿ�������
		BYTE							cbWeaveKind[4];						//�������
		BYTE							cbCenterCard[4];					//�����˿�
		BYTE                            cbCardData[4][4];                   //ʵ���˿�
	};

	//////////////////////////////////////////////////////////////////////////

	/*
	//	Ȩλ�ࡣ
	//  ע�⣬�ڲ�����λʱ���ֻ��������Ȩλ.����
	//  CChiHuRight chr;
	//  chr |= (chr_zi_mo|chr_peng_peng)������������޶���ġ�
	//  ֻ�ܵ�������:
	//  chr |= chr_zi_mo;
	//  chr |= chr_peng_peng;
	*/
	class CChiHuRight
	{	
		//��̬����
	private:
		static bool						m_bInit;
		static dword					m_dwRightMask[MAX_RIGHT_COUNT];

		//Ȩλ����
	private:
		dword							m_dwRight[MAX_RIGHT_COUNT];

	public:
		//���캯��
		CChiHuRight();

		//���������
	public:
		//��ֵ��
		CChiHuRight & operator = ( dword dwRight );

		//�����
		CChiHuRight & operator &= ( dword dwRight );
		//�����
		CChiHuRight & operator |= ( dword dwRight );

		//��
		CChiHuRight operator & ( dword dwRight );
		CChiHuRight operator & ( dword dwRight ) const;

		//��
		CChiHuRight operator | ( dword dwRight );
		CChiHuRight operator | ( dword dwRight ) const;

		//���ܺ���
	public:
		//�Ƿ�ȨλΪ��
		bool IsEmpty();

		//����ȨλΪ��
		void SetEmpty();

		//��ȡȨλ��ֵ
		BYTE GetRightData( dword dwRight[], BYTE cbMaxCount );

		//����Ȩλ��ֵ
		bool SetRightData( const dword dwRight[], BYTE cbRightCount );

	private:
		//���Ȩλ�Ƿ���ȷ
		bool IsValidRight( dword dwRight );
	};


	//////////////////////////////////////////////////////////////////////////

	//��Ϸ�߼���
	class CGameLogic:public FvSingleton<CGameLogic>
	{
		//��������
	protected:
		static const BYTE				m_cbCardDataArray[MAX_REPERTORY_SRLF];	//�˿�����
		BYTE							m_cbMagicIndex;						//��������

		//��������
	public:
		//���캯��
		CGameLogic();
		//��������
		virtual ~CGameLogic();

		//���ƺ���
	public:
		//�����˿�
		void RandCardData(BYTE cbCardData[], BYTE cbMaxCount);
		//ɾ���˿�
		bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
		//ɾ���˿�
		bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount);
		//ɾ���˿�
		bool RemoveValueCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount);
		//ɾ���˿�
		BYTE RemoveValueCardAll(BYTE cbCardData[], BYTE cbCardCount,BYTE cbRemoveCard);
		//ɾ���˿�
		bool RemoveValueCardOne(BYTE cbCardData[], BYTE cbCardCount,BYTE cbRemoveCard);
		//��������
		void SetMagicIndex( BYTE cbMagicIndex ) { m_cbMagicIndex = cbMagicIndex; }
		//����
		bool IsMagicCard( BYTE cbCardData );

		//������
		bool AddCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbAddCard[], BYTE cbAddCount);

		// �滻���cbAddCount ������  ��Ϻ�����RemoveValueCard ʹ��
		bool ReplaceCardData(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbAddCardData[], BYTE cbAddCount);

		//��������
	public:
		//��Ч�ж�
		bool IsValidCard(BYTE cbCardData);
		//�˿���Ŀ
		BYTE GetCardCount(const BYTE cbCardIndex[MAX_INDEX]);
		//����˿�
		BYTE GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4]);

		//�ȼ�����
	public:
		//�����ȼ�
		BYTE GetUserActionRank(BYTE cbUserAction);
		//���Ƶȼ�
		WORD GetChiHuActionRank(const CChiHuRight & ChiHuRight);

		//�����ж�
	public:
		//�����ж�
		BYTE EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
		//�����ж�
		BYTE EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
		//�����ж�
		BYTE EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);

		//�����ж�
	public:
		//���Ʒ���
		BYTE AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult);
		//�Ժ�����
		BYTE AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight);
		//���Ʒ���
		BYTE AnalyseTingCard( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
		//�Ƿ�����
		bool IsTingCard( const BYTE cbCardIndex[MAX_INDEX]);
		//�Ƿ���
		bool IsHuaZhu( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
		//������ʾ
		bool AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount);
		
		bool CheckHu(const BYTE cbCardIndexTemp[MAX_INDEX]);
		BYTE getAnyCardsCount( BYTE cbCardIndex[MAX_INDEX]);
		void removeAnyCard( BYTE cbCardIndex[MAX_INDEX],BYTE cbCardData,BYTE cardCount);
		bool isHaveAnyCard( BYTE cbCardIndex[MAX_INDEX],BYTE cbCardData,BYTE cardCount );
		bool isHaveAnyCards( BYTE cbCardIndex[MAX_INDEX]);
		bool Check3nWang(BYTE cbCardIndex[MAX_INDEX]);
		bool Check3nTiao(BYTE cbCardIndex[MAX_INDEX]);
		bool Check3nTong(BYTE cbCardIndex[MAX_INDEX]);
		bool IsQiDui(const BYTE cbCardIndex[MAX_INDEX]);
		//���Ʒ���
		BYTE AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const SRLF::CMD_WeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult);
		//ת������
	public:
		//�˿�ת��
		BYTE SwitchToCardData(BYTE cbCardIndex);
		//�˿�ת��
		BYTE SwitchToCardIndex(BYTE cbCardData);
		//�˿�ת��
		BYTE SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT]);
		//�˿�ת��
		BYTE SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);


		//����,������ֵ����
		bool SortCardList( BYTE cbCardData[MAX_COUNT], BYTE cbCardCount );

		//�ֱ��ȡ��ɫ����
		void GetCardWithColor(BYTE cbCardData[MAX_COUNT],BYTE cbCardCount,std::vector<BYTE>& CardList1,std::vector<BYTE>& CardList2,std::vector<BYTE>& CardList3);
		//��������
	protected:
		//�����
		bool IsPengPeng( const tagAnalyseItem *pAnalyseItem );
		//��һɫ��
		bool IsQingYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount,const BYTE cbCurrentCard);
		//��С����
		bool IsQiXiaoDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount,const BYTE cbCurrentCard);
		//����
		bool IsDaiYao( const tagAnalyseItem *pAnalyseItem );
		//����
		bool IsJiangDui( const tagAnalyseItem *pAnalyseItem );

	public:
		BYTE GetCardColor(BYTE cbCardDat);
		BYTE GetCardValue(BYTE cbCardDat);

		void GetCardInfoList(BYTE* pHandCardData, int nHandCardCount,std::vector<Card_Info>& kCardInfoList);
	};
}
