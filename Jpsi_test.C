using namespace RooFit ;

double Jpsi_test()
{


//Opening a root file and getting a TH1D from it
TFile* file = new TFile("Jpsi.root","read"); 
TH1D *massJpsiHist = (TH1D *)file->Get("h_DimuonsOppositeChargeEtaJpsi_M2_Double");


RooRealVar mass("mass","mass",2.7,3.5);

RooDataHist rooNoCutMass("rooNoCutMass","rooNoCutMass",mass,Import(*massJpsiHist));

ofstream textfile;
textfile.open("chi2.txt");

for(double k = 3.090 ; k < 3.100 ; k = k + 0.001 ) //loop mean1
{
  for(double l = 0.01 ; l < 0.09 ; l = l + 0.01 )  //loop sigma1
  {
    for(double m = 0.1 ; m < 3.0 ; m = m + 0.1) //loop alpha1
    {
      for(double n = 0.1 ; n < 2.5 ; n = n + 0.1) //loop power
      {
	
	//Roofit Variables
	RooRealVar mean1("mean1","mean1",k,2.9,3.2);
	RooRealVar sigma1("sigma1","sigma1",l,0.01,0.09);
	RooRealVar alpha("alpha","tail shift",m,0.2,3.0);
	RooRealVar npow("npow","power order",n,0.1,2.5);

	//with only Crystal Ball 
	//Mean: 3.093 ,Sigma: 0.03 ,Alpha: 2.1 ,Power: 0.4 ,chi^2 = 96.7902

	//Crystall Ball Fit (S I G N A L)
	RooCBShape cball1("cball1","cball1",mass,mean1,sigma1,alpha,npow);


	//Creating a frame
	RooPlot* frame = mass.frame(Title("Crystal Ball"));

	//Plot of Mass without Cuts
	rooNoCutMass.plotOn(frame,Name("theData"));

	//Plot of Crystal Ball fit
	cball1.plotOn(frame,Name("model2"));

	//Plot informations
	cball1.paramOn(frame,Layout(0.65,0.99,0.99),Format("NE"),Label(Form("#chi^{2} = 96.7902")) );

	//Chi^2 of the frame
	double chi2 = frame->chiSquare();
	//-------------------------------------------------
	//C H I -  S Q U A R E
	//--------------------------------------------------
	//cout << "chi^2 = " << chi2 << endl;


	if (chi2 < 100)//For write on the file the chi's^2 that are lower than 100 and its paramters
	{
	cout << "========================================"<< endl;
	cout << "Mean: "<< k << " ,Sigma: "<< l << " ,Alpha: "<< m << " ,Power: "<< n <<" ,chi^2 = " << frame->chiSquare() << endl;
	cout << "========================================"<< endl;

 	//write on file
	textfile << "Mean: "<< k << " ,Sigma: "<< l << " ,Alpha: "<< m << " ,Power: "<< n <<" ,chi^2 = " << chi2 << endl;
  	}//end loop chi2<150

				
      }//end loop power1
    }//end loop alpha1
  }//end loop sigma1
}//end loop mean

//Close file
textfile.close();

//Creating a Canvas
TCanvas* Jpsi_teste = new TCanvas("Jpsi_teste","Jpsi_teste",1200,600);
frame->Draw();

//Save Canvas as PNG
//Jpsi_teste->SaveAs("/eos/user/r/ragomesd/analysis_2018/Jpsi_teste.png");



}//end program
