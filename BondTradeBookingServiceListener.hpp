#pragma once
#ifndef BONDTRADEBOOKINGSERVICELISTENER_HPP
#define BONDTRADEBOOKINGSERVICELISTENER_HPP

#include "BondTradeBookingService.hpp"
#include "BondExecutionService.hpp"
#include "soa.hpp"
#include <string>

using namespace std;

class BondTradeBookingServiceListener : public ServiceListener<ExecutionOrder<Bond> > {
private:
	BondTradeBookingService* bondTradeBookingService;
	static int bookorder;

public:
	// ctor
	BondTradeBookingServiceListener(BondTradeBookingService* _bondTradeBookingService);

	
	// convert ExecutionOrder to Trade and pass to BondTradeBookingServce
	virtual void ProcessAdd(ExecutionOrder<Bond>& data) override;

	// no use
	virtual void ProcessRemove(ExecutionOrder<Bond>& data) override {}

	// no use
	virtual void ProcessUpdate(ExecutionOrder<Bond>& data) override {}
};

static int orderid = 0;

BondTradeBookingServiceListener::BondTradeBookingServiceListener(BondTradeBookingService* _bondTradeBookingService) {
	bondTradeBookingService = _bondTradeBookingService;
}

void BondTradeBookingServiceListener::ProcessAdd(ExecutionOrder<Bond>& data) {
	Bond bond = data.GetProduct();
	string tradeID = "Self_Execution";
	double price = data.GetPrice();

	string book;
	switch (orderid%3) {
	case 0: book = "TRSY1"; break;
	case 1: book = "TRSY2"; break;
	case 2: book = "TRSY1"; break;
	}
	auto quantity = data.GetVisibleQuantity();
	Side side = BUY;
	if (data.GetSide() == BID)
		side = SELL;
	Trade<Bond> trade(bond, tradeID, price, book, quantity, side);

	bondTradeBookingService->BookTrade(trade);
	orderid += 1;
	return;
}



#endif