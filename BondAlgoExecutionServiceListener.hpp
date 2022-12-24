#pragma once

#ifndef BONDALGOEXECUTIONSERVICELISTENER_HPP
#define BONDALGOEXECUTIONSERVICELISTENER_HPP

#include "BondAlgoExecutionService.hpp"
#include "soa.hpp"

using namespace std;

class BondAlgoExecutionServiceListener : public ServiceListener<OrderBook<Bond> > {
private:
	BondAlgoExecutionService* bondalgoexecutionservice;
public:
	//ctor
	BondAlgoExecutionServiceListener(BondAlgoExecutionService* _bondalgoexecutionservice);

	// called to add orderbook to BondAlgoExecutionService
	virtual void ProcessAdd(OrderBook<Bond>& data) override;

	// no use
	virtual void ProcessRemove(OrderBook<Bond>& data) override {};

	// no use
	virtual void ProcessUpdate(OrderBook<Bond>& data) override {};
};

BondAlgoExecutionServiceListener::BondAlgoExecutionServiceListener(BondAlgoExecutionService* _bondalgoexecutionservice) {
	bondalgoexecutionservice = _bondalgoexecutionservice;
}

void BondAlgoExecutionServiceListener::ProcessAdd(OrderBook<Bond>& data) {
	bondalgoexecutionservice->AddOrderBook(data);
	return;
}

#endif