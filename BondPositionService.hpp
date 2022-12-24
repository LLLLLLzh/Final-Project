#pragma once

#ifndef BONDPOSITIONSERVICE_HPP
#define BONDPOSITIONSERVICE_HPP

#include "BondTradeBookingService.hpp"
#include "positionservice.hpp"

using namespace std;

class BondPositionService : public PositionService<Bond> {
private:
	map<string, Position<Bond> >positionslist;
	vector<ServiceListener<Position<Bond> >*> listenerlist;
public:
	// ctor
	BondPositionService();

	// get position with bondid
	virtual Position<Bond>& GetData(string bondid) override;

	// no connector
	virtual void OnMessage(Position<Bond>& position) override;

	// add a listener
	// BondRickServiceListener and BondHistoricalDataServiceListener
	virtual void AddListener(ServiceListener<Position<Bond> >* listener) override;

	// get all of its listeners
	virtual const vector<ServiceListener<Position<Bond> >*>& GetListeners() const override;

	// add trade to list and notify listeners
	// Called by PostionServiceListener
	virtual void AddTrade(const Trade<Bond>& trade) override;
};

BondPositionService::BondPositionService() {
	positionslist = map<string, Position<Bond> >();
}

Position<Bond>& BondPositionService::GetData(string bondid) {
	return positionslist.at(bondid);
}

void BondPositionService::OnMessage(Position<Bond>& position) {
	return;
}

void BondPositionService::AddListener(ServiceListener<Position<Bond> >* listener) {
	listenerlist.push_back(listener);
	return;
}

const vector<ServiceListener<Position<Bond> >*>& BondPositionService::GetListeners() const {
	return listenerlist;
}

void BondPositionService::AddTrade(const Trade<Bond>& trade) {
	long quantity = trade.GetQuantity();
	if (trade.GetSide() == SELL)quantity = -quantity;
	auto bond = trade.GetProduct();
	auto bondid = bond.GetProductId();

	auto it = positionslist.find(bondid);
	if (it != positionslist.end()) {
		(it->second).AddPosition(trade.GetBook(), quantity);
	}
	else {
		positionslist.insert(pair<string, Position<Bond> >(bondid, Position<Bond>(bond)));
		AddTrade(trade);
		return;
	}

	for (auto& listener : listenerlist) {
		listener->ProcessAdd(it->second);
	}
	return;
}

#endif