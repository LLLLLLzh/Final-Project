#pragma once

#ifndef BONDALGOSTREAMINGSERVICELISTENER_HPP
#define BONDALGOSTREAMINGSERVICELISTENER_HPP

#include "soa.hpp"
#include "BondAlgoStreamingService.hpp"

using namespace std;

class BondAlgoStreamingServiceListener : public ServiceListener<Price<Bond> > {
private:
	BondAlgoStreamingService* bondalgoatreamingservice;
public:
	// ctor
	BondAlgoStreamingServiceListener(BondAlgoStreamingService* _bondalgoatreamingservice);
	
	// pass new price to BondAlgoStreamingService
	virtual void ProcessAdd(Price<Bond>& price) override;

	// no use
	virtual void ProcessRemove(Price<Bond>&) override;

	// no use
	virtual void ProcessUpdate(Price<Bond>&) override;
};

BondAlgoStreamingServiceListener::BondAlgoStreamingServiceListener(BondAlgoStreamingService* _bondalgoatreamingservice) {
	bondalgoatreamingservice = _bondalgoatreamingservice;
}

void BondAlgoStreamingServiceListener::ProcessAdd(Price<Bond>& price) {
	bondalgoatreamingservice->AddPrice(price);
	return;
}

void BondAlgoStreamingServiceListener::ProcessRemove(Price<Bond>& data) {
	return;
}

void BondAlgoStreamingServiceListener::ProcessUpdate(Price<Bond>& data) {
	return;
}




#endif