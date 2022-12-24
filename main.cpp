
#include "algoexecutionservice.hpp"
#include "algostreamingservice.hpp"

#include "BondAlgoExecutionService.hpp"
#include "BondAlgoExecutionServiceListener.hpp"
#include "BondAlgoStreamingService.hpp"
#include "BondAlgoStreamingServiceListener.hpp"

#include "BondExecutionService.hpp"
#include "BondExecutionServiceListener.hpp"
#include "BondExecutionServiceConnector.hpp"

#include "BondHistoricalDataService.hpp"
#include "BondHistoricalDataServiceConnector.hpp"
#include "BondHistoricalDataServiceListener.hpp"

#include "BondInquiryService.hpp"
#include "BondInquiryServiceConnector.hpp"

#include "BondMarketDataService.hpp"

#include "BondPositionService.hpp"
#include "BondPostionServiceListener.hpp"

#include "BondPricingService.hpp"

#include "BondRiskService.hpp"
#include "BondRiskServiceListener.hpp"

#include "BondService.hpp"

#include "BondStreamingService.hpp"
#include "BondStreamingServiceConnector.hpp"
#include "BondStreamingServiceListener.hpp"

#include "BondTradeBookingService.hpp"
#include "BondTradeBookingServiceConnector.hpp"
#include "BondTradeBookingServiceListener.hpp"

#include "GUIService.hpp"
#include "GUIServiceConnector.hpp"
#include "GUIServiceListener.hpp"

#include "products.hpp"

using namespace std;

int main() {
	vector<string> CUSIPlist = { "91282CFQ9","91282CFP1","91282CFR7","91282CDW8","912828ZZ6","912810PZ5","982810SL3"};
	
	vector<Bond> bondslist;
	BondService* bondservice = new BondService();

	for (int i = 0; i < 6; i++) {
		Bond b = Bond(CUSIPlist[i], CUSIP, "T", float(0.001), date(2030,Jan,01));
		bondslist.push_back(b);
		bondservice->Add(b);
	}
	
	


	// Pricing:
	BondPricingService* bondpricingservice = new BondPricingService();
	BondPricingServiceConnector* bondPricingServiceConnector
		= new BondPricingServiceConnector(bondpricingservice, bondservice);

	BondAlgoStreamingService* bondalgostreamingservice = new BondAlgoStreamingService();
	BondAlgoStreamingServiceListener* bondalgostreamingservicelistener
		= new BondAlgoStreamingServiceListener(bondalgostreamingservice);

	
	bondpricingservice->AddListener(bondalgostreamingservicelistener);

	BondStreamingServiceConnector* bondstreamingserviceconnector = new BondStreamingServiceConnector();
	BondStreamingService* bondStreamingService = new BondStreamingService(bondstreamingserviceconnector);
	BondStreamingServiceListener* bondstreamingservicelistener = new BondStreamingServiceListener(bondStreamingService);

	bondalgostreamingservice->AddListener(bondstreamingservicelistener);


	BondMarketDataService* bondMarketDataService = new BondMarketDataService();
	BondMarketDataServiceConnector* bondMarketDataServiceConnector
		= new BondMarketDataServiceConnector(bondMarketDataService, bondservice);

	BondAlgoExecutionService* bondAlgoExecutionService = new BondAlgoExecutionService();
	BondAlgoExecutionServiceListener* bondAlgoExecutionServiceListener
		= new BondAlgoExecutionServiceListener(bondAlgoExecutionService);

	bondMarketDataService->AddListener(bondAlgoExecutionServiceListener);

	BondExecutionServiceConnector* bondExecutionServiceConnector = new BondExecutionServiceConnector();
	BondExecutionService* bondExecutionService = new BondExecutionService(bondExecutionServiceConnector);
	BondExecutionServiceListener* bondExecutionServiceListener = new BondExecutionServiceListener(bondExecutionService);

	bondAlgoExecutionService->AddListener(bondExecutionServiceListener);


	
	
	BondTradeBookingService* bondTradeBookingService = new BondTradeBookingService();
	BondTradeBookingServiceConnector* bondTradeBookingServiceConnector
		= new BondTradeBookingServiceConnector(bondTradeBookingService, bondservice);

	BondPositionService* bondPositionService = new BondPositionService();
	BondPositionServiceListener* bondPositionServiceListener = new BondPositionServiceListener(bondPositionService);

	bondTradeBookingService->AddListener(bondPositionServiceListener);

	BondRiskService* bondRiskService = new BondRiskService();
	BondRiskServiceListener* bondRiskServiceListener = new BondRiskServiceListener(bondRiskService);

	bondPositionService->AddListener(bondRiskServiceListener);

	BondTradeBookingServiceListener* bondTradeBookingServiceListener
		= new BondTradeBookingServiceListener(bondTradeBookingService);
	bondExecutionService->AddListener(bondTradeBookingServiceListener);


	// **************GUI & Inquiry**********
	GUIServiceConnector* gUIServiceConnector = new GUIServiceConnector();
	GUIService* gUIService = new GUIService(gUIServiceConnector);
	GUIServiceListener* gUIServiceListener = new GUIServiceListener(gUIService);

	bondpricingservice->AddListener(gUIServiceListener);


	BondInquiryService* bondInquiryService = new BondInquiryService();
	BondInquiryServiceConnector* bondInquiryServiceConnector
		= new BondInquiryServiceConnector(bondInquiryService, bondservice);


	BondHistoricalPositionConnector* historicalPositionConnector = new BondHistoricalPositionConnector();
	BondHistoricalPositionService* bondHistoricalPositionService = new BondHistoricalPositionService(historicalPositionConnector);
	BondHistoricalPositionListener* historicalPositionListener = new BondHistoricalPositionListener(bondHistoricalPositionService);
	bondPositionService->AddListener(historicalPositionListener);

	BondHistoricalRiskConnector* historicalRiskConnector = new BondHistoricalRiskConnector();
	BondHistoricalRiskService* bondHistoricalRiskService = new BondHistoricalRiskService(historicalRiskConnector);
	BondHistoricalRiskListener* historicalRiskListener = new BondHistoricalRiskListener(bondHistoricalRiskService);
	bondRiskService->AddListener(historicalRiskListener);

	BondHistoricalExecutionConnector* historicalExecutionConnector = new BondHistoricalExecutionConnector();
	BondHistoricalExecutionService* bondHistoricalExecutionService = new BondHistoricalExecutionService(historicalExecutionConnector);
	BondHistoricalExecutionListener* historicalExecutionListener = new BondHistoricalExecutionListener(bondHistoricalExecutionService);
	bondExecutionService->AddListener(historicalExecutionListener);

	BondHistoricalStreamingConnector* historicalStreamingConnector = new BondHistoricalStreamingConnector();
	BondHistoricalStreamingService* bondHistoricalStreamingService = new BondHistoricalStreamingService(historicalStreamingConnector);
	BondHistoricalStreamingListener* historicalStreamingListener = new BondHistoricalStreamingListener(bondHistoricalStreamingService);
	bondStreamingService->AddListener(historicalStreamingListener);

	BondHistoricalInquiriesConnector* historicalAllInquiriesConnector = new BondHistoricalInquiriesConnector();
	BondHistoricalInquiriesService* bondHistoricalAllInquiriesService = new BondHistoricalInquiriesService(historicalAllInquiriesConnector);
	BondHistoricalInquriesListener* historicalAllInquriesListener = new BondHistoricalInquriesListener(bondHistoricalAllInquiriesService);
	bondInquiryService->AddListener(historicalAllInquriesListener);

	ofstream out;
	out.open("process.txt", ios::app);
	out << '1' << endl;


	// Run the system
	bondPricingServiceConnector->Subscribe();
	bondMarketDataServiceConnector->Subscribe();
	bondTradeBookingServiceConnector->Subscribe();
	bondInquiryServiceConnector->Subscribe();

	
	return 0;
}