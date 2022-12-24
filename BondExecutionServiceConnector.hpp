#pragma once
#pragma once

#ifndef	BONDEXECUTIONSERVICECONNECTOR_HPP
#define BONDEXECUTIONSERVICECONNECTOR_HPP

#include <string>
#include <iostream>

#include "BondExecutionService.hpp"
#include "soa.hpp"
#include "products.hpp"

using namespace std;

class BondExecutionServiceConnector :public Connector<ExecutionOrder<Bond> > {
public:
	// ctor
	BondExecutionServiceConnector();

	// print executionorder
	virtual void Publish(ExecutionOrder<Bond>& data) override;

	// no use
	virtual void Subscribe() override;
};

BondExecutionServiceConnector::BondExecutionServiceConnector() {}

void BondExecutionServiceConnector::Publish(ExecutionOrder<Bond>& data) {
	Bond bond = data.GetProduct();
	string oderType;
	switch (data.GetOrderType()) {
	case FOK: oderType = "FOK"; break;
	case MARKET: oderType = "MARKET"; break;
	case LIMIT: oderType = "LIMIT"; break;
	case STOP: oderType = "STOP"; break;
	case IOC: oderType = "IOC"; break;
	}
	cout << bond.GetProductId() << " OrderId: " << data.GetOrderId() << "\n"
		<< "	PricingSide: " << (data.GetSide() == BID ? "Bid" : "Ask")
		<< ", OrderType: " << oderType << ", IsChildOrder: " << (data.IsChildOrder() ? "True" : "False")
		<< "\n"
		<< "	Price: " << data.GetPrice() << ", VisibleQuantity: " << data.GetVisibleQuantity()
		<< ", HiddenQuantity: " << data.GetHiddenQuantity() << "\n" << endl;

	return;
}

void BondExecutionServiceConnector::Subscribe() {
	return;
}

void BondExecutionService::ExecuteOrder(ExecutionOrder<Bond>& order, Market market) {
	bondexecutionserviceconnector->Publish(order);
}


#endif