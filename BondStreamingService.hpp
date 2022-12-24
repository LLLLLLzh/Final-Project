#pragma once

#ifndef BONDSTREAMINGSERVICE_HPP
#define BONDSTREAMINGSERVICE_HPP

#include "streamingservice.hpp"
#include "BondAlgoStreamingService.hpp"

using namespace std;

class BondStreamingServiceConnector;

class BondStreamingService : public StreamingService<Bond> {
private:
	map<string, PriceStream<Bond> > streamslist;
	vector<ServiceListener<PriceStream<Bond> >*> listenerlist;
	BondStreamingServiceConnector* bondstreamingserviceconnector;
public:
	// ctor
	BondStreamingService(BondStreamingServiceConnector* _bondstreamingserviceconnector);

	// given id, return PriceStream from streamMap
	virtual PriceStream<Bond>& GetData(string bondid) override;

	// no use
	virtual void OnMessage(PriceStream<Bond>&) override;

	// add a listener. in this case, should be BondHistoricalDataServiceListener
	virtual void AddListener(ServiceListener<PriceStream<Bond> >* listener) override;

	// get listeners. in this case, should be BondHistoricalDataServiceListener
	virtual const vector<ServiceListener<PriceStream<Bond> >*>& GetListeners() const override;

	// call connector's Publish method to print stream
	virtual void PublishPrice(PriceStream<Bond>& priceStream) override;

	// called by BondStreamingServiceListener, pass AlgoStream from BondAlgoStreamingService
	void AddAlgo(const AlgoStream<Bond>& algo);
};

BondStreamingService::BondStreamingService(BondStreamingServiceConnector* _bondstreamingserviceconnector) {
	bondstreamingserviceconnector = _bondstreamingserviceconnector;
	streamslist = map<string, PriceStream<Bond> >();
}

PriceStream<Bond>& BondStreamingService::GetData(string id) {
	return streamslist.at(id);
}

void BondStreamingService::OnMessage(PriceStream<Bond>& stream) {
	return;
}

void BondStreamingService::AddListener(ServiceListener<PriceStream<Bond> >* listener) {
	listenerlist.push_back(listener);
	return;
}

const vector<ServiceListener<PriceStream<Bond> >*>& BondStreamingService::GetListeners() const {
	return listenerlist;
}

// void BondStreamingService::PublishPrice(PriceStream<Bond>&) defined in BondStreamingServiceConnector.hpp

void BondStreamingService::AddAlgo(const AlgoStream<Bond>& algo) {
	
	PriceStream<Bond> stream = algo.GetPriceStream();
	string bondid = stream.GetProduct().GetProductId();
	if (streamslist.find(bondid) != streamslist.end())
		streamslist.erase(bondid);
	streamslist.insert(pair<string, PriceStream<Bond> >(bondid, stream));

	for (auto& listener : listenerlist) { listener->ProcessAdd(stream);	}

	return;
}
#endif 