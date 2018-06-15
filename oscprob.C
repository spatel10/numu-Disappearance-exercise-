
int oscprob()
{

ofstream output("columns.txt");
ifstream input("eventrate.txt");

std::vector<float> ene(135, 0.0);
std::vector<float> count(135, 0.0);
std::vector<float> error(135, 0.0);

for(int x=0; x<135; x++)
{
	input >> ene[x] >> count[x] >> error[x];
}

input.close();
for(int y=0; y<135; y++)
{
	cout << y << "\t" << ene[y] << "\t" << count[y] << "\t" << error[y] << endl;
}

for(double i=-2; i<0.01; i+=0.01)
{
	float angle;
	angle= pow(10, i);
	
	for(double j=-2; j<2.01; j+=0.01)
	{
		float mass;
		mass= pow(10, j);
		output << angle << "\t" << mass << endl;
	}
}
return 0;
}