#pragma once

#ifndef BONDINQUIRYSERVICECONNECTOR_HPP		
#define BONDINQUIRYSERVICECONNECTOR_HPP



#include "soa.hpp"
#include <iostream>
#include <fstream>
// #include <cstdlib>
// #include <cstring>

#include "BondService.hpp"
#include "inquiryservice.hpp"
#include "BondInquiryService.hpp"

using namespace std;
class BondInquiryService;

class BondInquiryServiceConnector : public Connector<Inquiry<Bond> > {
private:
	BondInquiryService* bondinquiryservice;
	BondService* bondservice;

	// transform a string to price numver
	float PriceStringToNum(string& s);
public:
	// ctor
	BondInquiryServiceConnector(BondInquiryService* _bondinquiryservice, BondService* _bondservice);

	// Subscribe only, no use
	virtual void Publish(Inquiry<Bond>& data) override {}

	// read data from inquiries.txt
	virtual void Subscribe() override;

};


BondInquiryServiceConnector::BondInquiryServiceConnector(BondInquiryService* _bondinquiryservice, BondService* _bondservice) {
	bondinquiryservice = _bondinquiryservice;
	bondservice = _bondservice;
}

float BondInquiryServiceConnector::PriceStringToNum(string& s) {
	int n = s.size();
	if (s[n - 1] == '+') { s[n - 1] = '4'; }
	float p = stoi(s.substr(0, n - 4));
	p += float(stoi(s.substr(n - 3, 2)) / 32.0);
	p += float(stoi(s.substr(n - 1, 1)) / 256.0);
	return p;
}

void BondInquiryServiceConnector::Subscribe() {
	ifstream input;
	input.open("inquiries.txt");
	string row;

	while (getline(input, row)) {
		stringstream line(row);
		string s;

		getline(line, s, ',');
		string inquiry_ID = s;

		getline(line, s, ',');
		Bond bond = bondservice->GetData(s);

		getline(line, s, ',');
		Side side = (s == "BUY" ? BUY : SELL);

		getline(line, s, ',');
		long quantity = stol(s);

		getline(line, s, ',');
		double quote = PriceStringToNum(s);

		getline(line, s, ',');
		InquiryState new_state = RECEIVED;

		Inquiry<Bond> inquiry(inquiry_ID, bond, side, quantity, quote, new_state);
		bondinquiryservice->OnMessage(inquiry);
	}
	return;
}


#endif