#pragma once
#pragma once

#ifndef BONDSTREAMINGSERVICECONNECTOR_HPP
#define BONDSTREAMINGSERVICECONNECTOR_HPP

#include <string>
#include <iostream>

#include "soa.hpp"
#include "BondStreamingService.hpp"
#include "streamingservice.hpp"
#include "products.hpp"

using namespace std;

class BondStreamingServiceConnector : public Connector<PriceStream<Bond> > {
public:
	// ctor
	BondStreamingServiceConnector();

	// no use
	virtual void Subscribe() override;

	// print pricestream 
	virtual void Publish(PriceStream<Bond>& data) override;
};

BondStreamingServiceConnector::BondStreamingServiceConnector() {
}

void BondStreamingServiceConnector::Publish(PriceStream<Bond>& data) {
	Bond bond = data.GetProduct();
	PriceStreamOrder bidorder = data.GetBidOrder();
	PriceStreamOrder offerorder = data.GetOfferOrder();

	cout << bond.GetProductId() << "\n"
		<< "	BidOrder: " << " Price: " << bidorder.GetPrice() << ", VisibleQuantity: " << bidorder.GetVisibleQuantity()
		<< ", HiddenQuantity: " << bidorder.GetHiddenQuantity() << "\n"
		<< "    offerOrder: " << " Price: " << offerorder.GetPrice() << ", VisibleQuantity: " << offerorder.GetVisibleQuantity()
		<< ", HiddenQuantity: " << offerorder.GetHiddenQuantity() << "\n"
		<< endl;

	return;
}

void BondStreamingServiceConnector::Subscribe() {
}

void BondStreamingService::PublishPrice(PriceStream<Bond>& pricestream) {
	bondstreamingserviceconnector->Publish(pricestream);
	return;
}

#endif