int stat_chi()
{
//	float a, m, c;
//	double min_chi = 1e+10;

	ifstream input("chisquare.txt");
	ofstream ninety("90.txt");
	ofstream three("3sigma.txt");
	ofstream five("5sigma.txt");

/*	while(input >> a >> m >> c)
	{
		if(c < min_chi)
			min_chi = c;
	}
	input.close();

	input.open("chisquare.txt");
*/
	std::vector<float> angle(323609, 0.0);
	std::vector<float> mass(323609, 0.0);
	std::vector<float> chi(323609, 0.0);
//	std::vector<float> diff_chi(323609, 0.0);

	for(int i=0; i<323609; i++)
	{
		input >> angle[i] >> mass[i] >> chi[i];
//		diff_chi[i] = chi[i] - min_chi;
	}

	for(int i=0; i<323609; i++)
	{
		if((1.476<chi[i])) && (chi[i]<1.804))
		{
			ninety << angle[i] << "\t" << mass[i] << "\t" << chi[i] << endl; 
		}
		if((7<chi[i]) && (chi[i]<8))
		{
			three << angle[i] << "\t" << mass[i] << "\t" << chi[i] << endl;
		}
		if((22<chi[i]) && (chi[i]<25))
		{
			five << angle[i] << "\t" << mass[i] << "\t" << chi[i] << endl;
		}
	}

	c1 = new TCanvas("c1", "contour");
	
	g = new TGraph("90.txt");
	g->SetLineColor(kRed);
	g->Draw("AP");
	g->SetMinimum(0.01);
	g->SetMaximum(100);

	g1 = new TGraph("3sigma.txt");
	g1->Draw("PL");
	

	g2 = new TGraph("5sigma.txt");
	g2->Draw("PL");
	g2->SetLineColor(kGreen);
	
	
	c1.SetLogx();
	c1.SetLogy();
	c1.Update();


	return 0;
}
