{
	c1 = new TCanvas("c1", "Plots");
	TGraph2D *g = new TGraph2D("chisquare.txt","%lg%lg%lg");
	g->Draw("P");
	g->SetTitle("ChiSquare Distribution; sin^2(2theta); delta-m^2; chi-square");
	c1.SetLogx();
	c1.SetLogy();
	c1.Update();
}