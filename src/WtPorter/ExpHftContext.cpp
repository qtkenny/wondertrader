#include "ExpHftContext.h"
#include "WtRtRunner.h"
#include "../Share/StrUtil.hpp"

extern WtRtRunner& getRunner();

void ExpHftContext::on_bar(const char* code, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (newBar == NULL)
		return;

	std::string realPeriod;
	if (period[0] == 'd')
		realPeriod = StrUtil::printf("%s%u", period, times);
	else
		realPeriod = StrUtil::printf("m%u", times);

	getRunner().ctx_on_bar(_context_id, code, realPeriod.c_str(), newBar, ET_HFT);

	HftStraBaseCtx::on_bar(code, period, times, newBar);
}

void ExpHftContext::on_channel_lost()
{
	getRunner().hft_on_channel_lost(_context_id, _trader->id());

	HftStraBaseCtx::on_channel_lost();
}

void ExpHftContext::on_channel_ready()
{
	getRunner().hft_on_channel_ready(_context_id, _trader->id());

	HftStraBaseCtx::on_channel_ready();
}

void ExpHftContext::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	getRunner().hft_on_entrust(_context_id, localid, stdCode, bSuccess, message);

	HftStraBaseCtx::on_entrust(localid, stdCode, bSuccess, message);
}

void ExpHftContext::on_init()
{
	//���ⲿ�ص�
	getRunner().ctx_on_init(_context_id, ET_HFT);

	HftStraBaseCtx::on_init();
}

void ExpHftContext::on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled /*= false*/)
{
	getRunner().hft_on_order(_context_id, localid, stdCode, isBuy, totalQty, leftQty, price, isCanceled);

	HftStraBaseCtx::on_order(localid, stdCode, isBuy, totalQty, leftQty, price, isCanceled);
}

void ExpHftContext::on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail)
{
	getRunner().hft_on_position(_context_id, stdCode, isLong, prevol, preavail, newvol, newavail);

	HftStraBaseCtx::on_position(stdCode, isLong, prevol, preavail, newvol, newavail);
}

void ExpHftContext::on_tick(const char* code, WTSTickData* newTick)
{
	getRunner().ctx_on_tick(_context_id, code, newTick, ET_HFT);

	HftStraBaseCtx::on_tick(code, newTick);
}

void ExpHftContext::on_trade(const char* stdCode, bool isBuy, double vol, double price)
{
	getRunner().hft_on_trade(_context_id, stdCode, isBuy, vol, price);

	HftStraBaseCtx::on_trade(stdCode, isBuy, vol, price);
}