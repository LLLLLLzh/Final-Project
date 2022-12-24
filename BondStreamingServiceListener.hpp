#pragma once

#ifndef BONDSTREAMINGSERVICELISTENER_HPP
#define BONDSTREAMINGSERVICELISTENER_HPP

#include "BondStreamingService.hpp"
#include "products.hpp"
#include "soa.hpp"

using namespace std;

class BondStreamingServiceListener : public ServiceListener<AlgoStream<Bond>> {
private:
	BondStreamingService* bondstreamingservice;

public:
	// ctor
	BondStreamingServiceListener(BondStreamingService* _bondStreamingService);

	
	// pass AlgoStream to BondStreamingService
	virtual void ProcessAdd(AlgoStream<Bond>& algoStream) override;

	// no use
	virtual void ProcessRemove(AlgoStream<Bond>&) override;

	// no use
	virtual void ProcessUpdate(AlgoStream<Bond>&) override;
};

BondStreamingServiceListener::BondStreamingServiceListener(BondStreamingService* _bondstreamingservice) {
	bondstreamingservice = _bondstreamingservice;
}

void BondStreamingServiceListener::ProcessAdd(AlgoStream<Bond>& algostream) {
	bondstreamingservice->AddAlgo(algostream);
	auto priceStream = algostream.GetPriceStream();
	bondstreamingservice->PublishPrice(priceStream);
	return;
}

void BondStreamingServiceListener::ProcessRemove(AlgoStream<Bond>& data) {
	return;
}

void BondStreamingServiceListener::ProcessUpdate(AlgoStream<Bond>& data) {
	return;
}
#endif