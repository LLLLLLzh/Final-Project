#pragma once
#ifndef BONDALGOSTREAMINGSERVICE_HPP
#define BONDALGOSTREAMINGSERVICE_HPP

#include <map>
#include <vector>
#include <string>


#include "algostreamingservice.hpp"
#include "products.hpp"
#include "soa.hpp"

using namespace std;

class BondAlgoStreamingService : public AlgoStreamingService<Bond> {
private:
	map<string, AlgoStream<Bond> > algostreamlist;
	vector<ServiceListener<AlgoStream<Bond>>* > listenerlist;
public:
	// ctor
	BondAlgoStreamingService();

	// get AlgoStream given bondid 
	virtual AlgoStream<Bond>& GetData(string bondid) override;

	// no connector
	virtual void OnMessage(AlgoStream<Bond>&) override;

	// add a listener
	// BondStreamingServiceListener
	virtual void AddListener(ServiceListener<AlgoStream<Bond>>* listener) override;

	// get listeners. In this case, listener is BondStreamingServiceListener
	virtual const vector<ServiceListener<AlgoStream<Bond>>*>& GetListeners() const override;

	// add price to update/initialize a new AlgoStream in algomap
	// called by BondAlgoStreamingServiceListener to pass a new price from BondPricingService
	void AddPrice(const Price<Bond>& price);

};

BondAlgoStreamingService::BondAlgoStreamingService() {
	algostreamlist = map<string, AlgoStream<Bond>>();
}

AlgoStream<Bond>& BondAlgoStreamingService::GetData(string id) {
	return algostreamlist.at(id);
}

void BondAlgoStreamingService::OnMessage(AlgoStream<Bond>& algo) {
}

void BondAlgoStreamingService::AddListener(ServiceListener<AlgoStream<Bond>>* listener) {
	listenerlist.push_back(listener);
}

const vector<ServiceListener<AlgoStream<Bond>>*>& BondAlgoStreamingService::GetListeners() const {
	return listenerlist;
}

void BondAlgoStreamingService::AddPrice(const Price<Bond>& price) {
	string id = price.GetProduct().GetProductId();

	// if already exist, update price by RunAlgo
	if (algostreamlist.find(id) != algostreamlist.end()) {
		AlgoStream<Bond> as = algostreamlist.find(id)->second;
		as.AlgoResult(price);
	}
	// add an empty AlgoStream as value. now as id already in map,
	// call AddPrice again to update price by RunAlgo
	else {
		PriceStreamOrder p1(0, 0, 0, BID);
		PriceStreamOrder p2(0, 0, 0, OFFER);
		PriceStream<Bond> ps(price.GetProduct(), p1, p2);
		algostreamlist.insert(pair<string, PriceStream<Bond> >(id, ps));
		AddPrice(price);
		return;
	}

	// notify the listeners
	for (auto& listener : listenerlist) {
		listener->ProcessAdd(algostreamlist.find(id)->second);
	}

	return;
}
#endif