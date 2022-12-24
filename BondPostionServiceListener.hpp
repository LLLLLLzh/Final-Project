#pragma once
#ifndef BONDPOSITIONSERVICELISTENER_HPP
#define BONDPOSITIONSERVICELISTENER_HPP

#include "BondPositionService.hpp"
#include "soa.hpp"

using namespace std;

class BondPositionServiceListener : public ServiceListener<Trade<Bond> > {
private:
    BondPositionService* bondpositionservice;

public:
    // ctor
    BondPositionServiceListener(BondPositionService* _bondpositionservice);

    // call bondPositionService AddTrade method
    virtual void ProcessAdd(Trade<Bond>& data);

    // no use 
    virtual void ProcessRemove(Trade<Bond>& data) override;

    // no use
    virtual void ProcessUpdate(Trade<Bond>& data) override;
};

BondPositionServiceListener::BondPositionServiceListener(BondPositionService* _bondpositionservice) {
    bondpositionservice = _bondpositionservice;
}

void BondPositionServiceListener::ProcessAdd(Trade<Bond>& data) {
    bondpositionservice->AddTrade(data);
    return;
}

void BondPositionServiceListener::ProcessRemove(Trade<Bond>& data) {
}

void BondPositionServiceListener::ProcessUpdate(Trade<Bond>& data) {
}

#endif