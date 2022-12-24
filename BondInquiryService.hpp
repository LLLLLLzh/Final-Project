#pragma once

#ifndef BONDINQUIRYSERVICE_HPP
#define BONDINQUIRYSERVICE_HPP


#include <string>
#include <map>
#include <vector>
#include <fstream>

#include "inquiryservice.hpp"
#include "products.hpp"

using namespace std;

class BondInquiryService : public InquiryService<Bond> {
private:
	map<string, Inquiry<Bond> > inquirieslist;
	vector<ServiceListener<Inquiry<Bond> >*> listenerlist;

public:
	// ctor
	BondInquiryService();

	// get inquiry given id
	virtual Inquiry<Bond>& GetData(string id) override;

	// called by connector
	virtual void OnMessage(Inquiry<Bond>& inquiry) override;

	// add a listener, in this case, BondHistoricalDataServiceListener
	virtual void AddListener(ServiceListener<Inquiry<Bond> >* listener) override;

	// get listeners, in this case, BondHistoricalDataServiceListener
	virtual const vector<ServiceListener<Inquiry<Bond> >*>& GetListeners() const override;

	// send quote back to client. Here client is a txt file. no use here
	virtual void SendQuote(const string& inquiryId, double price) override;

	// reject quote. Here client is a txt file. no use here
	virtual void RejectInquiry(const string& inquiryId) override;

};

BondInquiryService::BondInquiryService() {
	inquirieslist = map<string, Inquiry<Bond> >();
}

Inquiry<Bond>& BondInquiryService::GetData(string id) {
	return inquirieslist[id];
}

void BondInquiryService::OnMessage(Inquiry<Bond>& inquiry) {
	inquiry.SetState(inquiry.GetPrice(), DONE);
	inquirieslist.insert(pair<string, Inquiry<Bond> >(inquiry.GetInquiryId(), inquiry));
	for (auto& listener : listenerlist) {
		listener->ProcessAdd(inquiry);
	}

	return;
}

void BondInquiryService::AddListener(ServiceListener<Inquiry<Bond> >* listener) {
	listenerlist.push_back(listener);
	return;
}

const vector<ServiceListener<Inquiry<Bond> >*>& BondInquiryService::GetListeners() const {
	return listenerlist;
}

void BondInquiryService::SendQuote(const string& inquiryId, double price) {
}

void BondInquiryService::RejectInquiry(const string& inquiryId) {
}



#endif