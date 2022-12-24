#pragma once
#ifndef BONDALGOEXECUTIONSERVICE_HPP
#define BONDALGOEXECUTIONSERVICE_HPP

using namespace std;

#include "algoexecutionservice.hpp"


class BondAlgoExecutionService : public AlgoExecutionService<Bond> {
private:
	map<string, AlgoExecution<Bond>> algoslist;
	vector<ServiceListener<AlgoExecution<Bond> >*> listenerlist;

public:
	// ctor
	BondAlgoExecutionService();

	// get algoExecution by id 
	AlgoExecution<Bond>& GetData(string id);

	// called by conector, in this case, no connector
	void OnMessage(AlgoExecution<Bond>&);

	// add a listener. in this case, BondExecutionServiceListener
	void AddListener(ServiceListener<AlgoExecution<Bond>>* listener);

	// get listeners. in this case, BondExecutionServiceListener
	const vector<ServiceListener<AlgoExecution<Bond>>*>& GetListeners() const;

	// add a order book and call RunAlgo in corresponding AlgoExecution
	// called by BondAlgoExecutionServiceListener
	void AddOrderBook(OrderBook<Bond>& orderBook);
};

BondAlgoExecutionService::BondAlgoExecutionService() {
	algoslist = map<string, AlgoExecution<Bond>>();
}

AlgoExecution<Bond>& BondAlgoExecutionService::GetData(string id) {
	return algoslist[id];
}

void BondAlgoExecutionService::OnMessage(AlgoExecution<Bond>& a) {
	return;
}

void BondAlgoExecutionService::AddListener(ServiceListener<AlgoExecution<Bond>>* listener) {
	listenerlist.push_back(listener);
	return;
}

const vector<ServiceListener<AlgoExecution<Bond>>*>& BondAlgoExecutionService::GetListeners() const {
	return listenerlist;
}

void BondAlgoExecutionService::AddOrderBook(OrderBook<Bond>& orderBook) {
	string bondid = orderBook.GetProduct().GetProductId();
	auto it = algoslist.find(bondid);
	// if already exist, update price by RunAlgo
	if (it != algoslist.end()) {
		(it->second).AlgoResult(orderBook);
	}

	// add an empty AlgoStream as value. now as id already in map,
	else {
		auto executionorder = ExecutionOrder<Bond>(orderBook.GetProduct(), BID, "0", FOK, 0, 0, 0, "0", true);
		algoslist.insert(pair<string, AlgoExecution<Bond> >(bondid, AlgoExecution<Bond>(executionorder)));
		AddOrderBook(orderBook);
		return;
	}

	// notify the listeners
	for (auto& listener : listenerlist) {
		listener->ProcessAdd(it->second);
	}

	return;
}
#endif 