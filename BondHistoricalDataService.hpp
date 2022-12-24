#pragma once

#ifndef BONDHISTORICALDATASERVICE_HPP
#define BONDHISTORICALDATASERVICE_HPP

#include <iostream>

#include "BondPositionService.hpp"
#include "BondRiskService.hpp"
#include "GUIServiceConnector.hpp"
#include "BondStreamingService.hpp"
#include "BondExecutionService.hpp"
#include "BondInquiryService.hpp"

#include "historicaldataservice.hpp"
#include "BondHistoricalDataServiceConnector.hpp"

using namespace std;



class BondHistoricalPositionService : public HistoricalDataService<Position<Bond> > {
private:
	BondHistoricalPositionConnector* connector;
	map<string, Position<Bond> > datalist;
public:
	// ctor
	BondHistoricalPositionService(BondHistoricalPositionConnector* _connector):connector(_connector) {};
	
	// get data given bondid
	virtual Position<Bond>& GetData(string bondid) override;

	// no use
	virtual void OnMessage(Position<Bond>& bond) override {}

	// no use
	virtual void AddListener(ServiceListener<Position<Bond>  >* listener) override {}

	// no use
	virtual const vector<ServiceListener<Position<Bond>  >*>& GetListeners() const override {
		return {};}

	// called by listeners, call correspoding connector to publish
	virtual void PersistData(string persistKey, Position<Bond>& data) override;
};

void BondHistoricalPositionService::PersistData(string persistKey, Position<Bond>& data) {
	string id = data.GetProduct().GetProductId();
	if (datalist.find(id) != datalist.end())
		datalist.erase(id);
	datalist.insert(pair<string, Position<Bond> >(id, data));
	connector->Publish(data);
	return;
}

Position<Bond>& BondHistoricalPositionService::GetData(string bondid) {
	return datalist.at(bondid); 
}


class BondHistoricalRiskService : public HistoricalDataService<PV01<Bond> > {
private:
	BondHistoricalRiskConnector* connector;
	map<string, PV01<Bond> > datalist;
public:
	// ctor
	BondHistoricalRiskService(BondHistoricalRiskConnector* _conncector) :connector(_conncector) {}

	// get data with bondid
	virtual PV01<Bond>& GetData(string bondid) override { return datalist[bondid]; }

	// no use
	virtual void OnMessage(PV01<Bond>& bond) override {}

	// no use
	virtual void AddListener(ServiceListener<PV01<Bond>>* listener) override {}

	// no use
	virtual const vector<ServiceListener<PV01<Bond>  >*>& GetListeners() const override {
		return {};
	}

	// called by listeners, call correspoding connector to publish
	virtual void PersistData(string persistKey, PV01<Bond>& data) override;
};

void BondHistoricalRiskService::PersistData(string persistKey, PV01<Bond>& data) {
	string id = data.GetProduct().GetProductId();
	if (datalist.find(id) != datalist.end())
		datalist.erase(id);
	datalist.insert(pair<string, PV01<Bond> >(id, data));
	connector->Publish(data);
	return;
}


class BondHistoricalExecutionService : public HistoricalDataService<ExecutionOrder<Bond> > {
private:
	BondHistoricalExecutionConnector* connector;
	map<string, ExecutionOrder<Bond> > datalist;
public:
	// ctor
	BondHistoricalExecutionService(BondHistoricalExecutionConnector* _conncector) :connector(_conncector) {}

	// override pure virtual functions in base class
	// get data given id
	virtual ExecutionOrder<Bond>& GetData(string bondid) override { return datalist[bondid]; }

	// no use
	virtual void OnMessage(ExecutionOrder<Bond>& bond) override {}

	// no use
	virtual void AddListener(ServiceListener<ExecutionOrder<Bond>  >* listener) override {}

	// no use
	virtual const vector<ServiceListener<ExecutionOrder<Bond>  >*>& GetListeners() const override {return {};}

	// called by listeners, call correspoding connector to publish
	virtual void PersistData(string persistKey, ExecutionOrder<Bond>& data) override;
};

void BondHistoricalExecutionService::PersistData(string persistKey, ExecutionOrder<Bond>& data) {
	auto id = data.GetOrderId();
	if (datalist.find(id) != datalist.end())
		datalist.erase(id);
	datalist.insert(pair<string, ExecutionOrder<Bond> >(id, data));
	connector->Publish(data);
	return;
}


class BondHistoricalStreamingService : public HistoricalDataService<PriceStream<Bond> > {
private:
	BondHistoricalStreamingConnector* connector;
	map<string, PriceStream<Bond> > datalist;
public:
	// ctor
	BondHistoricalStreamingService(BondHistoricalStreamingConnector* _conncector) :connector(_conncector) {}

	// get data given bondid
	virtual PriceStream<Bond>& GetData(string bondid) override { return datalist[bondid]; }

	// no use
	virtual void OnMessage(PriceStream<Bond>& bond) override {}

	// no use
	virtual void AddListener(ServiceListener<PriceStream<Bond>  >* listener) override {}

	// no use
	virtual const vector<ServiceListener<PriceStream<Bond>  >*>& GetListeners() const override {
		return {};
	}

	// called by listeners, call correspoding connector to publish
	virtual void PersistData(string persistKey, PriceStream<Bond>& data) override;
};

void BondHistoricalStreamingService::PersistData(string persistKey, PriceStream<Bond>& data) {
	auto id = data.GetProduct().GetProductId();
	if (datalist.find(id) != datalist.end())
		datalist.erase(id);
	datalist.insert(pair<string, PriceStream<Bond> >(id, data));
	connector->Publish(data);
	return;
}


class BondHistoricalInquiriesService : public HistoricalDataService<Inquiry<Bond>> {
private:
	BondHistoricalInquiriesConnector* connector;
	map<string, Inquiry<Bond> > datalist;
public:
	// ctor
	BondHistoricalInquiriesService(BondHistoricalInquiriesConnector* _connector) :connector(_connector) {};

	// get data with bondid
	virtual Inquiry<Bond>& GetData(string bondid) override { return datalist[bondid]; }

	// no use
	virtual void OnMessage(Inquiry<Bond>& bond) override {}

	// no use
	virtual void AddListener(ServiceListener<Inquiry<Bond>>* listener) override {}

	// no use
	virtual const vector<ServiceListener<Inquiry<Bond> >*>& GetListeners() const override {return {};}

	// called by listeners, call correspoding connector to publish
	virtual void PersistData(string persistKey, Inquiry<Bond>& data) override;
};

void BondHistoricalInquiriesService::PersistData(string persistKey, Inquiry<Bond>& data) {
	string id = data.GetInquiryId();
	if (datalist.find(id) != datalist.end())
		datalist.erase(id);
	datalist.insert(pair<string, Inquiry<Bond> >(id, data));

	connector->Publish(data);
	return;
};


#endif