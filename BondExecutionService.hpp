#pragma once
#ifndef BONDEXECUTIONSERVICE_HPP
#define BONDEXECUTIONSERVICE_HPP

#include "BondAlgoExecutionService.hpp"
#include "executionservice.hpp"

using namespace std;

class BondExecutionServiceConnector;

class BondExecutionService : public ExecutionService<Bond> {
private:
	map<string, ExecutionOrder<Bond> > executionslist;
	vector<ServiceListener<ExecutionOrder<Bond> >*> listenerlist;
	BondExecutionServiceConnector* bondexecutionserviceconnector;

public:
	// ctor
	BondExecutionService(BondExecutionServiceConnector* _bondexecutionserviceconnector);

	// get execution order by id
	virtual ExecutionOrder<Bond>& GetData(string id) override;

	// no use
	virtual void OnMessage(ExecutionOrder<Bond>&) override;

	// add a listener
	// BondHistoricalDataServiceListener
	virtual void AddListener(ServiceListener<ExecutionOrder<Bond> >* listener) override;

	// get listeners
	virtual const vector<ServiceListener<ExecutionOrder<Bond> >*>& GetListeners() const override;

	// pass execution order to connector
	virtual void ExecuteOrder(ExecutionOrder<Bond>& order, Market market) override;

	// add algorithm to algoslist
	void AddAlgoExecution(const AlgoExecution<Bond>& algo);
};

BondExecutionService::BondExecutionService(BondExecutionServiceConnector* _bondexecutionserviceconnector) {
	bondexecutionserviceconnector = _bondexecutionserviceconnector;
}

ExecutionOrder<Bond>& BondExecutionService::GetData(string bondid) {
	return executionslist[bondid];
}

void BondExecutionService::OnMessage(ExecutionOrder<Bond>&) {
	return;
}

void BondExecutionService::AddListener(ServiceListener<ExecutionOrder<Bond> >* listener) {
	listenerlist.push_back(listener);
	return;
}

const vector<ServiceListener<ExecutionOrder<Bond> >*>& BondExecutionService::GetListeners() const {
	return listenerlist;
}


// define void BondExecutionService::ExecuteOrder in BondExecutionServiceConnector.hpp


void BondExecutionService::AddAlgoExecution(const AlgoExecution<Bond>& algo) {
	ExecutionOrder<Bond> executionorder = algo.GetExecutionOrder();
	string id = executionorder.GetProduct().GetProductId();

	if (executionslist.find(id) != executionslist.end())
		executionslist.erase(id);
	executionslist.insert(pair<string, ExecutionOrder<Bond> >(id, executionorder));

	for (auto& listener : listenerlist) {
		listener->ProcessAdd(executionorder);
	}

	return;
}
#endif