#pragma once

#ifndef BONDEXECUTIONSERVICELISTENER_HPP
#define BONDEXECUTIONSERVICELISTENER_HPP

using namespace std;

#include "BondExecutionService.hpp"
#include "soa.hpp"

class BondExecutionServiceListener : public ServiceListener<AlgoExecution<Bond>> {
private:
	BondExecutionService* bondexecutionservice;

public:
	// ctor
	BondExecutionServiceListener(BondExecutionService* _bondexecutionservice);

	// and BondExecutionService::ExecuteOrder
	virtual void ProcessAdd(AlgoExecution<Bond>& data) override;

	// no use in this case
	virtual void ProcessRemove(AlgoExecution<Bond>& data) override;

	// no use in this case
	virtual void ProcessUpdate(AlgoExecution<Bond>& data) override;

};

BondExecutionServiceListener::BondExecutionServiceListener(BondExecutionService* _bondexecutionservice) {
	bondexecutionservice = _bondexecutionservice;
}

void BondExecutionServiceListener::ProcessAdd(AlgoExecution<Bond>& data) {
	auto order = data.GetExecutionOrder();
	bondexecutionservice->AddAlgoExecution(data);
	bondexecutionservice->ExecuteOrder(order, BROKERTEC);
	return;
}

void BondExecutionServiceListener::ProcessRemove(AlgoExecution<Bond>& data) {
}


void BondExecutionServiceListener::ProcessUpdate(AlgoExecution<Bond>& data) {
}

#endif