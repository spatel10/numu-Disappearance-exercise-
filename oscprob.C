//Input the file with the eventrate data.
//Input file contains the count and error for each energy bin center from the original flux

//****THE UNITS STILL HAVE NOT BEEN TAKEN INTO ACCOUNT
//****CHANGES IN EVENT RATE CODE WILL HAVE TO BE REFLECTED IN THIS

int oscprob()
{

ofstream output("chisquare.txt");
ifstream input("eventrate.txt");

std::vector<float> ene(135, 0.0);
std::vector<float> count(135, 0.0);
std::vector<float> error(135, 0.0);
std::vector<float> prob(135, 0.0);
//std::vector<float> n_osc(135, 0.0);
std::vector<float> chi(135, 0.0);

for(int x=0; x<135; x++)
{
	input >> ene[x] >> count[x] >> error[x];
}

input.close();

//calculate the chi-square for each delta-m^2 and sin^2(2theta)
//the formula for chi-square used is: sum over each energy bin((N_null-N_osc)^2/(N_null_Error)^2)
//output file contains sin^2(2theta); delta-m^2; chi-square; where the first two parameters are equally spaced in log scale

for(double i=-2; i<0.01; i+=0.01)
{
	float angle;
	float chisq = 0.0;
	angle= pow(10, i);
	
	for(double j=-2; j<2.01; j+=0.01)
	{
		float mass;
		mass= pow(10, j);
		for(int e=0; e<135; e++)
		{
			prob[e] = 1 - angle* pow(sin((1.27*mass*0.6)/ene[e]),2);
//			n_osc[e] = prob[e] * count[e];
			chi[e] = (count[e]*(1-prob[e]))/(pow(error[e],2));
			chisq = chisq + chi[e];
		}
		output << angle << "\t" << mass << "\t" << chisq << endl;
	}
}
return 0;
}