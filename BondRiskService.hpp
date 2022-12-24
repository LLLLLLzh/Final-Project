#pragma once

#ifndef BONDRISKSERVICE_HPP
#define BONDRISKSERVICE_HPP

using namespace std;

#include "BondPositionService.hpp"
#include "riskservice.hpp"


class BondRiskService : public RiskService<Bond> {
private:
	map<string, PV01<Bond> > pv01list;
	vector<ServiceListener<PV01<Bond> >*> listenerlist;
public:
	// ctor
	BondRiskService();

	
	// get PV01 with bondid
	virtual PV01<Bond>& GetData(string bondid) override;

	// no use
	virtual void OnMessage(PV01<Bond>& pv01) override { return; }

	// add a listener
	// BondHistoricalDataService
	virtual void AddListener(ServiceListener<PV01<Bond> >* listener) override;

	// get listeners
	virtual const vector<ServiceListener<PV01<Bond> >*>& GetListeners() const override;

	// add positions to the pv01list
	virtual void AddPosition(Position<Bond>& position);

	// get total risk
	virtual const double GetBucketedRisk(const BucketedSector<Bond>& sector) override;

};


BondRiskService::BondRiskService() {
	pv01list = map<string, PV01<Bond> >();
}

PV01<Bond>& BondRiskService::GetData(string id) {
	return pv01list.at(id);
}

void BondRiskService::AddListener(ServiceListener<PV01<Bond> >* listener) {
	listenerlist.push_back(listener);
	return;
}

const vector<ServiceListener<PV01<Bond> >*>& BondRiskService::GetListeners() const {
	return listenerlist;
}


void BondRiskService::AddPosition(Position<Bond>& position) {
	auto bond = position.GetProduct();
	auto bondid = bond.GetProductId();
	auto it = pv01list.find(bondid);

	long quantity = position.GetAggregatePosition();

	if (it != pv01list.end()) {
		(it->second).AddQuantity(quantity);
	}
	else {
		pv01list.insert(pair<string, PV01<Bond> >(bondid, PV01<Bond>(bond, 0.001, quantity)));
	}
	it = pv01list.find(bondid);

	for (auto& listener : listenerlist) {
		listener->ProcessAdd(it->second);
	}

	return;
}


const double BondRiskService::GetBucketedRisk(const BucketedSector<Bond>& sector) {
	double res = 0;
	for (auto& product : sector.GetProducts()) {
		res += pv01list.at(product.GetProductId()).GetPV01();
	}
	return res;
}


#endif