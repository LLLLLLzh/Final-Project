#pragma once

#ifndef BONDHISTORICALDATASERVICECONNECTOR_HPP
#define BONDHISTORICALDATASERVICECONNECTOR_HPP

#include <string>
#include <iostream>

#include "BondHistoricalDataService.hpp"
#include "GUIServiceConnector.hpp"

#include "products.hpp"
#include "soa.hpp"

using namespace std;

class BondHistoricalPositionConnector : public Connector<Position<Bond> > {
public:
	// ctor
	BondHistoricalPositionConnector() {};

	// do nothing, publish only
	virtual void Subscribe() override {}

	// print position
	virtual void Publish(Position<Bond>& data) override;
};

void BondHistoricalPositionConnector::Publish(Position<Bond>& data) {
	auto bond = data.GetProduct();
	auto position = data.GetAggregatePosition();

	ofstream out;
	out.open("positions.txt", ios::app);
	out << bond.GetProductId() << "," << position << endl;

	return;
}


class BondHistoricalRiskConnector : public Connector<PV01<Bond> > {
public:
	// ctor
	BondHistoricalRiskConnector() {};

	// do nothing, publish only
	virtual void Subscribe() override {}

	// print position
	virtual void Publish(PV01<Bond>& data) override;
};

void BondHistoricalRiskConnector::Publish(PV01<Bond>& data) {
	auto bond = data.GetProduct();
	auto risk = data.GetPV01();

	ofstream out;
	out.open("risk.txt", ios::app);
	out << bond.GetProductId() << "," << risk << endl;

	return;
}


class BondHistoricalExecutionConnector : public Connector<ExecutionOrder<Bond> > {
public:
	// ctor
	BondHistoricalExecutionConnector() {};

	// do nothing, publish only
	virtual void Subscribe() override {}

	// print position
	virtual void Publish(ExecutionOrder<Bond>& data) override;
};

void BondHistoricalExecutionConnector::Publish(ExecutionOrder<Bond>& data) {
	auto bond = data.GetProduct();
	string oderType;
	switch (data.GetOrderType()) {
	case FOK: oderType = "FOK"; break;
	case MARKET: oderType = "MARKET"; break;
	case LIMIT: oderType = "LIMIT"; break;
	case STOP: oderType = "STOP"; break;
	case IOC: oderType = "IOC"; break;
	}

	ofstream out;
	out.open("executions.txt", ios::app);
	out << bond.GetProductId() << "," << data.GetOrderId()
		<< "," << (data.GetSide() == BID ? "Bid" : "Ask")
		<< "," << oderType << "," << (data.IsChildOrder() ? "True" : "False")
		<< "," << data.GetPrice()
		<< "," << data.GetVisibleQuantity()
		<< "," << data.GetHiddenQuantity() << endl;

	return;
}


class BondHistoricalStreamingConnector : public Connector<PriceStream<Bond> > {
public:
	// ctor
	BondHistoricalStreamingConnector() {};

	// do nothing, publish only
	virtual void Subscribe() override {}

	// print position
	virtual void Publish(PriceStream<Bond>& data) override;
};

void BondHistoricalStreamingConnector::Publish(PriceStream<Bond>& data) {
	auto bond = data.GetProduct();
	auto bidOrder = data.GetBidOrder();
	auto askOrder = data.GetOfferOrder();

	ofstream out;
	out.open("streaming.txt", ios::app);
	out << bond.GetProductId() << ","
		<< bidOrder.GetPrice() << "," << bidOrder.GetVisibleQuantity()
		<< "," << bidOrder.GetHiddenQuantity()
		<< "," << askOrder.GetPrice() << "," << askOrder.GetVisibleQuantity()
		<< "," << askOrder.GetHiddenQuantity()
		<< endl;

	return;
}


class BondHistoricalInquiriesConnector : public Connector<Inquiry<Bond> > {
public:
	// ctor
	BondHistoricalInquiriesConnector() {};

	// do nothing, publish only
	virtual void Subscribe() override {}

	// print position
	virtual void Publish(Inquiry<Bond>& data) override;
};

void BondHistoricalInquiriesConnector::Publish(Inquiry<Bond>& data) {
	auto inquiry_id = data.GetInquiryId();
	auto id = data.GetProduct().GetProductId();
	auto price = data.GetPrice();
	auto quantity = data.GetQuantity();

	string side = (data.GetSide() == BUY ? "BUY" : "SELL");
	auto state = data.GetState();
	string state_;
	switch (state) {
	case RECEIVED:state_ = "RECEIVED"; break;
	case QUOTED:state_ = "QUOTED"; break;
	case DONE:state_ = "DONE"; break;
	case REJECTED:state_ = "REJECTED"; break;
	case CUSTOMER_REJECTED:state_ = "CUSTOMER_REJECTED"; break;
	}

	ofstream out;
	out.open("allinquiries.txt", ios::app);
	out << inquiry_id << "," << id << "," << price << "," << quantity << "," << side << "," << state_
		<< endl;

	return;
}

#endif