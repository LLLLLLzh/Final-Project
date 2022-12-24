#pragma once
#ifndef ALGOSTREAMINGSERVICE_HPP
#define ALGOSTREAMINGSERVICE_HPP

#include <iostream>

#include "streamingservice.hpp"
#include "pricingservice.hpp"
#include "products.hpp"

using namespace std;

template<typename T>
class AlgoStream {
private:
	PriceStream<T> pricestream;
public:
	// ctor
	AlgoStream(const PriceStream<T>& _pricestream);

	// use the algorithm to update priceStream according to bond price
	void AlgoResult(Price<T> price);

	// return pricestream
	PriceStream<T> GetPriceStream() const;
};

template<typename T>
AlgoStream<T>::AlgoStream(const PriceStream<T>& _pricestream) :pricestream(_pricestream) {
}

template<typename T>
void AlgoStream<T>::AlgoResult(Price<T> price) {
	T product = price.GetProduct();

	// not this PriceStream to update
	if (product.GetProductId() != pricestream.GetProduct().GetProductId()) { 
		return; 
	}

	float mid = price.GetMid();
	float spread = price.GetBidOfferSpread();
	float bid = mid - spread / 2.0;
	float offer = mid + spread / 2.0;

	long visibleQ = 0, hiddenQ = 0;
	// if spread is tight
	if (spread < 2.5 / 256.0) {
		visibleQ = 1000000;
		hiddenQ = 2 * visibleQ;
		
	}
	PriceStreamOrder order_bid(bid, 1000000, 2000000, BID);
	PriceStreamOrder order_ask(offer, 1000000, 2000000, OFFER);
	pricestream = PriceStream<T>(product, order_bid, order_ask);

	return;
}

template<typename T>
PriceStream<T> AlgoStream<T>::GetPriceStream() const {
	return pricestream;
}

template<typename T>
class AlgoStreamingService : public Service<string, AlgoStream<T> >
{
};

#endif 