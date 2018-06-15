
#include <iostream>
#include <vector>
#include <fstream>

//***STILL HAVE NOT CHANGED TO APPROPRIATE UNITS IN THE FINAL YAXIS OF THE EVENT RATE GRAPH

void eventrate()
{

//Reading in the flux.root file (flux histogram)
//Outputing the contents into a text file with different columns 
// histdata.txt format: (Bin #; Bin Center Energy (GeV); Counts (Flux); Bin Error)

	TFile *f1 = TFile::Open("flux.root");

	ofstream flux("histdata.txt");

	for(int i=1; i<=numu_CV_AV_TPC->GetNbinsX(); i++)
	{
		flux << i << "\t" << numu_CV_AV_TPC->GetBinCenter(i) << "\t" << numu_CV_AV_TPC->GetBinContent(i) << "\t" << numu_CV_AV_TPC->GetBinError(i) << endl;
	}

	flux.close();

//Reading in the cross_section.root file (event cross-section graph)
//outputing the contents into a text fie with different columns 
//cross_sec.txt format: (Point #; X-Axis Value (ENE in GeV); Y-axis Value (cross-section))

	TFile *f = TFile::Open("cross_section.root");
	
	ofstream cross("cross_sec.txt");

	int nPoint = qel_cc_n->GetN();
	count=0;

	std::vector<double> x(nPoint, 0.0);
	std::vector<double> y(nPoint, 0.0);

	for(int i=0; i<nPoint; i++)
	{
		qel_cc_n->GetPoint(i, x[i], y[i]);
	}

	for(int i=0; i<nPoint; i+=5)
	{
		count++;
		cross << count << "\t" << (x[i+1]+x[i+2])/2 << "\t" << (y[i+1]+y[i+2])/2 << endl;
	}
	cross.close();


//Reading in the contents of the flux and cross_section data files in order to multiply the graphs
//Drawing the graph using TGraphErrors in ROOT

	ifstream input1("histdata.txt"), input2("cross_sec.txt");
	ofstream output1("eventrate.txt");

	float h_point, h_ene, h_count, h_error;
	double c_point, c_ene, c_value;
	float f_point, f_ene, f_count, f_error;



	while((input1 >> h_point >> h_ene >> h_count >> h_error) || (input2 >> c_point >> c_ene >> c_value))
	{
		f_point = (h_point + c_point)/2;
		f_ene = (h_ene + c_ene)/2;
		f_count = (h_count)*(c_value);
		f_error = h_error;
		output1 << f_ene << "\t" << f_count << "\t" << f_error << endl; 
	}

	graph = new TGraphErrors("eventrate.txt", "%lg%lg%lg");
	graph->SetMarkerStyle(7);
	graph->SetTitle("Event Rate; Energy (GeV); Counts(# of events)");
	graph->Draw("AP");
} 