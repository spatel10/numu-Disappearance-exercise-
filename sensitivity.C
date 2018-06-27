
void Flux();
void CrossSection();
int IdealEventRate(float num_ar_icarus, float pot_icarus);
int IncNDEventRate(float num_ar_icarus, float num_ar_nd, float pot_icarus, float pot_nd);
void IdealChiSq(int counter);

int sensitivity()
{
	Flux(); //Takes the flux.root file and puts the contents in a text file (histdata.txt)
	CrossSection(); //Takes the cross_section.root file and puts the contents in a text file (cross_sec.txt)

	float num_ar_icarus = (476e+6)*(6e+23)/(39.948); // (vol in g)*(avogrado's #)/(molar mass of Ar)
	float num_ar_nd = (112e+6)*(6e+23)/(39.948);

	float pot_icarus = (6e+14); //The flux file has units (1/1e-6 POT)
	float pot_nd = (6e+14);

	int counter;

//** USE ONE OF BELOW ACC TO NEED
	counter = IdealEventRate(num_ar_icarus, pot_icarus); //(PART 1) To Calculate Eventrate at ICARUS with perfect knowledge of flux at 470m 
//	counter = IncNDEventRate(num_ar_icarus, num_ar_nd, pot_icarus, pot_nd); //(PART 2B) Including the near detector at 100m to constraint 30% uncertainty in flux (PART 2A)

//** CALCULATING CHISQUARE
	IdealChiSq(counter); //(LINE 132) MAKE CHANGES TO THE FUNCTION FOR PART 1 and 2A ACC. 

	return 0;
}


///*** FUNCTIONS **********************************************************************
void Flux()
{
	TFile *f1 = TFile::Open("flux.root");
	ofstream flux("histdata.txt");

	for(int i=1; i<=numu_CV_AV_TPC->GetNbinsX(); i++)
	{
		flux << i << "\t" << numu_CV_AV_TPC->GetBinCenter(i) << "\t" << numu_CV_AV_TPC->GetBinContent(i) << endl;
	}

	flux.close();
}

//****************************************************************************************
void CrossSection()
{
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
}

//********************************************************************************************
int IdealEventRate(float num_ar_icarus, float pot_icarus)
{
	ifstream input1("histdata.txt"), input2("cross_sec.txt");
	ofstream output1("eventrate_1.txt");

	float f_point, f_ene, f_count;
	float c_point, c_ene, c_value;
	float e_point, e_ene, e_count;
	int counter = 0;
	while((input1 >> f_point >> f_ene >> f_count) || (input2 >> c_point >> c_ene >> c_value))
	{
		e_point = (f_point + c_point)/2;
		e_ene = (f_ene + c_ene)/2;
		e_count = (f_count)*(c_value)*(num_ar_icarus)*(1e-42)*(pow(470,2)/pow(600,2))*(pot_icarus); 
		output1 << e_ene << "\t" << e_count << endl;
		counter += 1;
	}
/*	graph = new TGraphErrors("eventrate_1.txt", "%lg%lg");
	graph->SetMarkerStyle(7);
	graph->SetTitle("Event Rate at ICARUS; Energy (GeV); Counts(# of events)");
	graph->GetYaxis()->SetTitleOffset(1.50);
	graph->SetLineColor(kRed);
	graph->Draw("APL");
*/
	return counter;
}

//********************************************************************************************
int IncNDEventRate(float num_ar_icarus, float num_ar_nd, float pot_icarus, float pot_nd)
{
	ifstream input1("histdata.txt"), input2("cross_sec.txt");
	ofstream output1("eventrate_2b.txt");

	float f_point, f_ene, f_count;
	float c_point, c_ene, c_value;
	float e_point, e_ene, e_count, e_count_nd, error_frac;
	int counter = 0;

	while((input1 >> f_point >> f_ene >> f_count) || (input2 >> c_point >> c_ene >> c_value))
	{
		e_point = (f_point + c_point)/2;
		e_ene = (f_ene + c_ene)/2;
		e_count_nd = (f_count)*(c_value)*(num_ar_nd)*(1e-42)*(pow(470,2)/pow(100,2))*(pot_nd);
		if(e_count_nd != 0)
		{
		error_frac = (sqrt(e_count_nd))/e_count_nd ;
		e_count = (f_count)*(c_value)*(num_ar_icarus)*(1e-42)*(pow(470,2)/pow(600,2))*(pot_icarus); 
		output1 << e_ene << "\t" << e_count << "\t" << error_frac << endl; 
		}
	}
/*	graph = new TGraphErrors("eventrate_2b.txt", "%lg%lg");
	graph->SetMarkerStyle(7);
	graph->SetTitle("Event Rate at ICARUS; Energy (GeV); Counts(# of events)");
	graph->GetYaxis()->SetTitleOffset(1.50);
	graph->SetLineColor(kRed);
	graph->Draw("APL");
*/
	return counter;
}

//*********************************************************************************************
void IdealChiSq(int counter) //changes need to made on line 134 and ~162 
{
	ofstream output("chisquare_2a.txt"); //USE chisquare_1.txt for (part 1) w/o flux uncertainty; chisquare_2a.txt for (part 2a) 30% flux uncertainty
	ifstream event("eventrate_1.txt");

	std::vector<float> ene(counter, 0.0);
	std::vector<float> count(counter, 0.0);
	std::vector<float> prob(counter, 0.0);
	std::vector<float> chi(counter, 0.0);

	for(int i=0; i<counter; i++)
	{
		event >> ene[i] >> count[i];
	}
	event.close();

	for(double a=-3; a<0.01; a+=0.01)
	{
		float angle;
		angle= pow(10, a);	
		for(double m=-2; m<2.01; m+=0.01)
		{
			float mass;
			float chisq = 0.0;
			mass= pow(10, m);
			for(int e=0; e<counter; e++)
			{
				if(ene[e] != 0)
				{
					prob[e] = angle*pow(sin((1.27*mass*0.6)/(ene[e])),2);
//** FOR PART 1 (W/O UNCERTAINTY)
//					chi[e] = count[e]*pow((prob[e]),2);
//					chisq = chisq + chi[e];
//** FOR PART 2A (W/ 30% UNCERTAINTY)
					if(count[e] != 0)
					{
					chi[e] = (pow(count[e],2)*pow(prob[e],2))/(count[e]+pow(0.3*count[e],2));
					}
					chisq = chisq + chi[e];

				}
			}
			output << angle << "\t" << mass << "\t" << chisq << "\t" << endl;
		}
	}
	output.close();	
}
//**********************************************************************************************