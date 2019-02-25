// CODE RIGHT NOW IS GEARED TO QUESTION 2A

void FluxNCrossSection();
int EventRate(float num_ar_icarus, float num_ar_nd, float pot_icarus, float pot_nd);
void ChiSquare(int counter);
void RootPlots();

int sensitivity()
{
	FluxNCrossSection(); //Takes the flux.root file and puts the contents in a text file (flux_data.txt)

	float num_ar_icarus = (476e+6)*(6e+23)/(39.948); // (vol in g)*(avogrado's #)/(molar mass of Ar)
	float num_ar_nd = (112e+6)*(6e+23)/(39.948); // --------------------||-----------------------

	float pot_icarus = (6e+14); //The flux file has units (1/1e-6 POT)
	float pot_nd = (6e+14); // ---------------||---------------------

	int counter;

//** GETS THE EVENT RATE AT ICARUS USING THE FLUX AND CROSS-SECTION AND OTHER INFORMATION (IF APPLICABLE)
//** MAKE CHANGES TO THE FOLLOWING FUNCTION ACCORDING TO QUESTION (AS DESCRIBED THERE)
	counter = EventRate(num_ar_icarus, num_ar_nd, pot_icarus, pot_nd); 

//** CALCULATING SENSITIVITY CONTOURS
//** MAKE CHANGES TO THE FOLLOWING FUNCTION ACCORDING TO QUESTION (AS DESCRIBED THERE)
	ChiSquare(counter); 

//** PLOTS EVENTRATE and SENSITIVITY
//** MAKE CHANGES TO THE FOLLOWING FUNCTION ACCORDING TO QUESTION (AS DESCRIBED THERE)
	RootPlots();

	return 0;
}

//-------------------------------------------------FUNCTIONS------------------------------------------------------------------
//** ******************************************************************************************************
void FluxNCrossSection()
{
	TFile *f1 = TFile::Open("flux.root");
	ofstream flux("flux_data.txt");

	for(int i=1; i<=numu_CV_AV_TPC->GetNbinsX(); i++)
	{
		flux << i << "\t" << numu_CV_AV_TPC->GetBinCenter(i) << "\t" << numu_CV_AV_TPC->GetBinContent(i) << endl;
	}

	flux.close();

	TFile *f = TFile::Open("cross_section.root");
	ofstream cross("cross_section_data.txt");

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
}

//********************************************************************************************
int EventRate(float num_ar_icarus, float num_ar_nd, float pot_icarus, float pot_nd) //CHOOSE APPROPRIATE O/P FILE; APPROPRIATE O/P CALC CHANGES 
{
	ifstream input1("flux_data.txt"), input2("cross_section_data.txt");
	ofstream output1("eventrate_2a.txt");

	float f_point, f_ene, f_count;
	float c_point, c_ene, c_value;
	float e_point, e_ene, e_count, e_nd_count, error_frac;
	int counter = 0;
	while((input1 >> f_point >> f_ene >> f_count) || (input2 >> c_point >> c_ene >> c_value))
	{
		e_point = (f_point + c_point)/2;
		e_ene = (f_ene + c_ene)/2;

//** FOR PART 1 AND 2A (NO NEAR DETECTOR)
		e_count = (f_count)*(c_value)*(num_ar_icarus)*(1e-42)*(pow(470,2)/pow(600,2))*(pot_icarus); 
		output1 << e_ene << "\t" << e_count << "\t" << counter << endl;
		counter += 1;

//** FOR PART 2B (WITH NEAR DETECTOR CONSTRAINT ON STAT UNCERTAINTY)
/*		e_nd_count = (f_count)*(c_value)*(num_ar_nd)*(1e-42)*(pow(470,2)/pow(100,2))*(pot_nd);
		if(e_nd_count != 0)
		{
		e_count = (f_count)*(c_value)*(num_ar_icarus)*(1e-42)*(pow(470,2)/pow(600,2))*(pot_icarus); 
		output1 << e_ene << "\t" << e_count << "\t" << e_nd_count << endl; 
		counter += 1;
		}		
*/
	}
	cout << counter << endl;
	return counter;
}

//*********************************************************************************************
void ChiSquare(int counter) //CHANGES - APPROPRIATE O/P AND I/P FILES JUST BELOW; APPROPRIATE CHI CALC IN OSC SECTION
{
	ifstream event("eventrate_2a.txt"); // USE FILE NAME THAT O/P OF EVENTRATE FUNCTION
	ofstream ninety("two_a_90.txt");
	ofstream three("two_a_3sigma.txt");
	ofstream five("two_a_5sigma.txt");

	std::vector<float> ene(counter, 0.0);
	std::vector<float> count(counter, 0.0);
	std::vector<float> nd_count(counter, 0.0);
	std::vector<float> prob(counter, 0.0);
	std::vector<float> chi(counter, 0.0);

	for(int i=0; i<counter; i++)
	{
		event >> ene[i] >> count[i] >> nd_count[i]; //error_frac only applies to part 2B with ND constraint
	}
	event.close();
	float x1, x2, x3, y1, y2, y3;

	for(double m=-2; m<2.01; m+=0.01)
	{
		float mass;
		float  min1=1e+15, min2=1e+15, min3=1e+15;
		mass= pow(10, m);
		for(double a=-3; a<0.0; a+=0.01)
		{
			float angle;
			angle= pow(10, a);
			float chisq = 0.0;
			for(int e=0; e<counter; e++)
			{
				if(ene[e] != 0)
				{
					prob[e] = angle*pow(sin((1.27*mass*0.6)/(ene[e])),2);
//** FOR PART 1 (W/O UNCERTAINTY)
//					chi[e] = count[e]*pow((prob[e]),2);

//** FOR PART 2A (W/ 30% UNCERTAINTY)
					if(count[e] != 0)
					{
					chi[e] = (pow(count[e],2)*pow(prob[e],2))/(count[e]+pow(0.3*count[e],2));
					}


//** FOR PART 2B (W/ NEAR DETECTOR)
/*					if(count[e] != 0)
					{
					chi[e] = (count[e])*pow(prob[e],2)/(1 + count[e]/nd_count[e]);
					}	
*/
					chisq = chisq + chi[e];
				} //END Energy IF
			} //END Energy For

			x1=fabs(chisq - 1.64);
			x2=fabs(chisq - 7.74);
			x3=fabs(chisq - 23.4);

			if(x1<min1)	{min1=x1; y1=angle;}
			if(x2<min2) {min2=x2; y2=angle;}
			if(x3<min3) {min3=x3; y3=angle;}
		} //END Angle For
		ninety << y1 << "\t" << mass << endl;
		three << y2 << "\t" << mass << endl;
		five << y3 << "\t" << mass << endl;
	} //END MASS for
event.close(); ninety.close(); three.close(); five.close();
}

//**********************************************************************************************
void RootPlots() //CHANGES - SAME I/P FILES IN TGRAPH AS O/P OF EVENTRATE FUNCTION; CHANGES TO NAMES OF I/P FILES OF THE CONTOURS
{

//** EVENTRATE AT ICARUS (# OF EVENTS vs. ENERGY)
/*	TCanvas *c1 = new TCanvas();
	graph = new TGraphErrors("eventrate_2b.txt", "%lg%lg");
	graph->SetMarkerStyle(7);
	graph->SetTitle("Event Rate at ICARUS; Energy (GeV); Counts(# of events)");
	graph->GetYaxis()->SetTitleOffset(1.50);
	graph->SetLineColor(kRed);
	graph->Draw("APL");
*/

//** 90%, 3SIGMA, 5SIGMA SENSITIVITY CONTOURS (DELTA-M^2 vs. SIN^2(2THETA))
	TCanvas *c2 = new TCanvas();	
	g = new TGraph("two_a_90.txt");
	g->SetTitle("#nu_{#mu} Disappearance (ICARUS) (30% Flux Uncertainty)");
	g->Draw("AL");	
	g->SetLineColor(kBlue);
	g->SetMinimum(0.01);
	g->SetMaximum(100);
	g->GetXaxis()->SetTitle("sin^{2}(2#theta)");
	g->GetYaxis()->SetTitle("#Deltam^{2}");
	g->GetXaxis()->SetTitleOffset(1.25);
	g->GetYaxis()->SetTitleOffset(1.25);
	g->GetXaxis()->SetLimits(0.001,1);
	g->GetXaxis()->CenterTitle();
	g->GetYaxis()->CenterTitle();

	g1 = new TGraph("two_a_3sigma.txt");
	g1->SetTitle("3#sigma");
	g1->SetLineColor(kRed);
	g1->Draw("L SAME");
	
	g2 = new TGraph("two_a_5sigma.txt");
	g2->SetTitle("5#sigma");
	g2->Draw("L SAME");
	g2->SetLineColor(kGreen);
	
	legend = new TLegend(0.75, 0.75, 0.9, 0.9);
	legend->AddEntry(g, "90% C.L.","l");
	legend->AddEntry(g1, "3#sigma","l");
	legend->AddEntry(g2, "5#sigma", "l");
	legend->Draw();

	TLatex text;
	text.SetTextSize(0.03);
//2B	
//	text.DrawLatex(0.2, 7, "#splitline{POT:- @ ND= 6x10^{20}}{		 @ ICARUS= 6x10^{20}}");
//1 and 2A
	text.DrawLatex(0.1, 7, "POT @ ICARUS= 6x10^{20}");

	c2.SetLogx();
	c2.SetLogy();
	c2.Update();
}