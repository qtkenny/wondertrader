/*!
 * \file TraderMAOpt.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * \brief
 */
#pragma once

#include <string>
#include <queue>
#include <stdint.h>

#include <boost/asio/io_service.hpp>

#include "../Includes/WTSTypes.h"
#include "../Includes/ITraderApi.h"
#include "../Includes/WTSCollection.hpp"

#include "../API/mCliApi3.7/maCliOptTradeApi.h"

#include "../Share/IniHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/WtKVCache.hpp"


 // ֤ȯҵ��
typedef enum
{
	MA_BuyOpen = 400,  // ��
	MA_SellClose,	   // ��ƽ
	MA_SellOpen,	  // ����
	MA_BuyClose       // ��ƽ
} StkBiz;

// ֤ȯҵ��
typedef enum
{
	MA_OPT_LIMITPRICE_GFD = 130,  // ��Ʊ��Ȩ�걨-�޼�GFD
	MA_OPT_LIMITPRICE_FOK = 131,  // ��Ʊ��Ȩ�걨-�޼�FOK
	MA_OPT_ANYPRICE_FOK = 133,  // ��Ʊ��Ȩ�걨-�м�FOK
	MA_OPT_ANYPRICE_ICO = 134,  // ��Ʊ��Ȩ�걨-�м�IOC
	MA_OPT_ANYPRICE_LEFT_LIMIT = 132,  // ��Ʊ��Ȩ�걨-�м�ʣת�޼�GFD
	MA_ORDER = 100,  // ί���걨
	MA_DRAWORDER = 101,  // �����걨
	MA_BESTFIVELEVELPRICE = 121,  // �м������嵵��ʱ�ɽ�ʣ�೷��
	MA_MARKEPRICE_ALLORDRAW = 122,  // �м�ȫ��ɽ�����
	MA_BESTPRICE_THISSIDE = 123,  // �м۱������ż۸�
	MA_BESTPRICE_OTHERSIDE = 124,  // �м۶��ַ����ż۸�
	MA_MARKETPRICE_LEFTDRAW = 125  // �мۼ�ʱ�ɽ�ʣ�೷��
} STtkBizAction;

typedef enum
{
	MA_GFD = 600,  // ������Ч
	MA_FOK,  // ȫ�ɻ���ȫ��
	MA_IOC  // �����ɽ�������ȡ��
} MATimeConditionType;

// ������־
typedef enum
{
	MA_ORDER_CANCEL = 'T',  // ����
	MA_ORDER_NORMAL = 'F'  // ����
} MAIsWithdraw;

// ί����Ч��־
typedef enum
{
	MA_ORDER_VALID = 0,  // ��Ч
	MA_ORDER_INVALID = 1  // ��Ч
} MAOrderValidFlag;

typedef enum tagOrderState
{
	MA_Nottouched = '0',  // δ��, д��ί�е�����׶Σ��ñ�ҵ��û�е��걨ʱ��
	MA_Submitting, // �������ñ�־��©��ɨ����м�״̬����ʾ�����ж���
	MA_Submitted, // �ѱ����Ѿ��ɹ�д��ӿڿ�
	MA_Withdraw_NotFilled, // �ѱ��������ñ�ҵ�񱻳�����������δ�ɽ�
	MA_PartFilled_ToWithdraw, // ���ɴ������ñ�ҵ�񲿷ֳɽ���ʣ�ಿ�ֵȴ�����
	MA_PartWithdraw, // ���������ֳ���
	MA_Withdawn, // �ѳ��������Ѿ��ɽ�
	MA_PartFilled_Withdrawn, // �����ѳ������ֳɽ�
	MA_AllFilled, // �ѳɣ��Ѿ��ɽ�
	MA_Canceled, // �ϵ������������Զ�����
	MA_ToSubmit = 'A', // ������д�뱨�̶���δ�ɹ�

}  MAOrderState;


USE_NAMESPACE_MACLI

USING_NS_WTP;

class TraderMAOpt : public ITraderApi, public IOptTraderApi, public CCliOptTradeSpi
{
public:
	TraderMAOpt();
	virtual ~TraderMAOpt();

public:
	typedef enum
	{
		WS_NOTLOGIN,		//δ��¼
		WS_LOGINING,		//���ڵ�¼
		WS_LOGINED,			//�ѵ�¼
		WS_LOGINFAILED,		//��¼ʧ��
		WS_CONFIRM_QRYED,
		WS_CONFIRMED,		//��ȷ��
		WS_ALLREADY			//ȫ������
	} WrapperState;

private:
	int doLogin();

	//////////////////////////////////////////////////////////////////////////
	//ITraderApi�ӿ�
public:
	virtual bool init(WTSVariant* params) override;

	virtual void release() override;

	virtual void registerSpi(ITraderSpi *listener) override;

	virtual bool makeEntrustID(char* buffer, int length) override;

	virtual void connect() override;

	virtual void disconnect() override;

	virtual bool isConnected() override;

	virtual int login(const char* user, const char* pass, const char* productInfo) override;

	virtual int logout() override;

	virtual int orderInsert(WTSEntrust* entrust) override;

	virtual int orderAction(WTSEntrustAction* action) override;

	virtual int queryAccount() override;

	virtual int queryPositions() override;

	virtual int queryOrders() override;

	virtual int queryTrades() override;

	virtual int querySettlement(uint32_t uDate) override;


	//////////////////////////////////////////////////////////////////////////
	//IOptTraderApi
	virtual int orderInsertOpt(WTSEntrust* entrust) override;

	virtual int orderActionOpt(WTSEntrustAction* action) override;

	virtual int	queryOrdersOpt(WTSBusinessType bType) override;


	//////////////////////////////////////////////////////////////////////////
	//maCliOpt���׽ӿ�ʵ��
public:
	// �ͻ�����������ɹ�����ͨ�����Ӻ󣬸÷���������
	virtual int OnConnected(void) override;

	// �ͻ�����������ɹ���ͨ�����ӶϿ�ʱ���÷���������
	virtual int OnDisconnected(int p_nReason, const char *p_pszErrInfo) override;

	// ������Ӧ
	virtual int OnRspHeartBeat(CFirstSetField *p_pFirstSet, CRspHeartBeatField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// �û���¼������Ӧ
	virtual int OnRspUserLogin(CFirstSetField *p_pFirstSet, CRspOptUserLoginField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// �����ʽ��ѯ��Ӧ
	virtual int OnRspQryExpendableFund(CFirstSetField* p_pFirstSetField, CRspOptExpendableFundField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// ���ú�Լ�ʲ���ѯ��Ӧ
	virtual int OnRspQryExpendableCu(CFirstSetField* p_pFirstSetField, CRspOptExpendableCuField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// ������Ȩ��Լ�˻���ѯ��Ӧ
	virtual int OnRspQryAcct(CFirstSetField* p_pFirstSetField, CRspOptAcctField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;


	// ������Ȩί�г�����Ӧ
	virtual int OnRspCancelOrder(CFirstSetField* p_pFirstSetField, CRspOptCancelOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// ȷ�ϻر�
	virtual int OnRtnOrderConfirm(CRtnOptOrderConfirmField* p_pRspField) override;

	// �ɽ��ر�--ί����Ϣ
	virtual int OnRtnOrder(CRtnOptOrderField* p_pRspField) override;

	// �ɽ��ر�--�ɽ���Ϣ
	virtual int OnRtnOrderFill(CRtnOptOrderFillField* p_pRtnField) override;

	// �ɽ��ر�--��Լ��Ϣ
	virtual int OnRtnContract(CRtnOptContractField* p_pRspField) override;

	// ������Ȩ����ί�в�ѯ��Ӧ
	virtual int OnRspQryCurrDayOrder(CFirstSetField* p_pFirstSetField, CRspOptCurrDayOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	// ������Ȩ���ճɽ���ѯ��Ӧ
	virtual int OnRspQryCurrDayFill(CFirstSetField* p_pFirstSetField, CRspOptCurrDayFillField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	//������ϳֲ���ϸ��ѯ��Ӧ 2015/8/5
	virtual int OnRspQryCombStraPosDetail(CFirstSetField* p_pFirstSetField, CRspOptCombStraPosDetailField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) override;

	/////////////////// �Զ���ӿ�
	// �ʽ��˻���ѯ
	int OnRspQryTradingAccount(void);

	// �ֲֲ�ѯ
	int OnRspQryPosition(void);

protected:
	/*
	*	��������Ϣ
	*/
	//bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);


	WTSOrderInfo*	makeOrderInfo(CRspOptOrderField* orderField);
	WTSOrderInfo*	makeOrderInfo(CRtnOptOrderField* orderField);
	WTSOrderInfo*   makeOrderInfo(CRspOptCurrDayOrderField* daiOrderField);
	//WTSOrderInfo*   makeOrderInfo(CRspOptCancelOrderField* cancelOrderField);
	//WTSEntrust*		makeEntrust(CThostFtdcInputOrderField *entrustField);
	//WTSError*		makeError(CThostFtdcRspInfoField* rspInfo);
	WTSTradeInfo*	makeTradeRecord(CRtnOptOrderFillField *tradeField);
	WTSTradeInfo*   makeTradeRecord(CRspOptCurrDayFillField *dayTradeField);
	//WTSEntrust*		makeEntrust(CThostFtdcInputExecOrderField *entrustField);

	//void			generateEntrustID(char* buffer, uint32_t frontid, uint32_t sessionid, uint32_t orderRef);
	//bool			extractEntrustID(const char* entrustid, uint32_t &frontid, uint32_t &sessionid, uint32_t &orderRef);

	uint32_t		genRequestID();

protected:
	//std::string		m_strBroker;
	//std::string		m_strFront;

	int64_t			m_llCustCode;  // �ͻ�����
	int64_t			m_llCuacctCode;
	int				m_iInitOrg;  // ��������
	std::string		m_strchChannel;  // ��������

	std::string		m_strHost;  // IP��ַ
	int				m_iPort;  // �˿�

	std::string		m_strUser;
	std::string		m_strPass;

	std::string		m_strAuthData;
	std::string		m_strEncryptKey;

	std::string		m_strUseScope;
	std::string		m_strAuthType;
	std::string		m_strEncryptType;
	std::string		m_strAcctType;
	std::string		m_strChannel;

	ITraderSpi*		m_bscSink;
	IOptTraderSpi*	m_optSink;

	uint32_t					m_lDate;
	std::atomic<uint32_t>		m_orderRef;		//��������

	WrapperState				m_wrapperState;

	CCliOptTradeApi*			m_pUserAPI;
	std::atomic<uint32_t>		m_iRequestID;

	typedef WTSHashMap<std::string> PositionMap;
	PositionMap*				m_mapPosition;
	WTSArray*					m_ayTrades;
	WTSArray*					m_ayOrders;

	IBaseDataMgr*				m_bdMgr;

	boost::asio::io_service		_asyncio;
	StdThreadPtr				_thrd_worker;

	std::string		m_strModule;
	DllHandle		m_hInstMA;

	WTSAccountInfo*         m_accountInfo;

	//ί�е���ǻ�����
	WtKVCache		m_eidCache;
	//������ǻ�����
	WtKVCache		m_oidCache;
};