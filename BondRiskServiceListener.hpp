#pragma once

#ifndef BONDRISKSERVICELISTENER_HPP
#define BONDRISKSERVICELISTENER_HPP

using namespace std;

#include "BondRiskService.hpp"
#include "soa.hpp"

class BondRiskServiceListener : public ServiceListener<Position<Bond> > {
private:
	BondRiskService* bondriskservice;
public:
	//ctor
	BondRiskServiceListener(BondRiskService* _bondriskservice);

	// call bondRiskService AddPosition method
	virtual void ProcessAdd(Position<Bond>& data) override;

	// no use
	virtual void ProcessRemove(Position<Bond>& data) override {};

	// no use
	virtual void ProcessUpdate(Position<Bond>& data) override {};
};

BondRiskServiceListener::BondRiskServiceListener(BondRiskService* _bondriskservice) {
	bondriskservice = _bondriskservice;
}

void BondRiskServiceListener::ProcessAdd(Position<Bond>& data) {
	bondriskservice->AddPosition(data);
	return;
}



#endif