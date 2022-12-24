#pragma once

#include "BondHistoricalDataService.hpp"
#include "soa.hpp"

using namespace std;

class BondHistoricalPositionListener : public ServiceListener<Position<Bond> > {
private:
	BondHistoricalPositionService* service;

public:
	// ctor
	BondHistoricalPositionListener(BondHistoricalPositionService* s) :service(s) {}

	virtual void ProcessAdd(Position<Bond>& data) override {
		service->PersistData(data.GetProduct().GetProductId(), data);
		return;
	}
	virtual void ProcessRemove(Position<Bond>& data) override {}
	virtual void ProcessUpdate(Position<Bond>& data) override {}

};

class BondHistoricalRiskListener : public ServiceListener<PV01<Bond> > {
private:
	BondHistoricalRiskService* service;

public:
	// ctor
	BondHistoricalRiskListener(BondHistoricalRiskService* s) :service(s) {}

	virtual void ProcessAdd(PV01<Bond>& data) override {
		service->PersistData(data.GetProduct().GetProductId(), data);
		return;
	}
	virtual void ProcessRemove(PV01<Bond>& data) override {}
	virtual void ProcessUpdate(PV01<Bond>& data) override {}

};


class BondHistoricalExecutionListener : public ServiceListener<ExecutionOrder<Bond> > {
private:
	BondHistoricalExecutionService* service;

public:
	// ctor
	BondHistoricalExecutionListener(BondHistoricalExecutionService* s) :service(s) {}

	virtual void ProcessAdd(ExecutionOrder<Bond>& data) override {
		service->PersistData(data.GetOrderId(), data);
		return;
	}

	virtual void ProcessRemove(ExecutionOrder<Bond>& data) override {}
	virtual void ProcessUpdate(ExecutionOrder<Bond>& data) override {}

};


class BondHistoricalStreamingListener : public ServiceListener<PriceStream<Bond>  > {
private:
	BondHistoricalStreamingService* service;

public:
	// ctor
	BondHistoricalStreamingListener(BondHistoricalStreamingService* p) :service(p) {}

	virtual void ProcessAdd(PriceStream<Bond>& data) override {
		service->PersistData(data.GetProduct().GetProductId(), data);
		return;
	}
	virtual void ProcessRemove(PriceStream<Bond>& data) override {}
	virtual void ProcessUpdate(PriceStream<Bond>& data) override {}

};

class BondHistoricalInquriesListener : public ServiceListener<Inquiry<Bond> > {
private:
	BondHistoricalInquiriesService* service;

public:
	// ctor
	BondHistoricalInquriesListener(BondHistoricalInquiriesService* s) :service(s) {}

	virtual void ProcessAdd(Inquiry<Bond>& data) override {
		service->PersistData(data.GetInquiryId(), data);
		return;
	}
	virtual void ProcessRemove(Inquiry<Bond>& data) override {}
	virtual void ProcessUpdate(Inquiry<Bond>& data) override {}

};
