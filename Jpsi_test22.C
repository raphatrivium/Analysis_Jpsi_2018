using namespace RooFit ;

double Jpsi_test22()
{

//Opening a root file and getting a TH1D from it
TFile* file = new TFile("Jpsi.root","read"); 
TH1D *massJpsiHist = (TH1D *)file->Get("h_DimuonsOppositeChargeEtaJpsi_M2_Double");

RooRealVar mass("mass","mass",2.7,3.5);
RooDataHist rooNoCutMass("rooNoCutMass","rooNoCutMass",mass,Import(*massJpsiHist));

//open a file
ofstream textfile;
textfile.open("chi2_2.txt");


/*for(double k = 3.090 ; k < 3.100 ; k = k + 0.001 ) //loop mean1
{
  for(double l = 0.01 ; l < 0.09 ; l = l + 0.01 )  //loop sigma1
  {
    for(double m = 0.1 ; m < 3.0 ; m = m + 0.1) //loop alpha1
    {
      for(double n = 0.1 ; n < 2.5 ; n = n + 0.1) //loop power
      {
	for(double o = 0.5 ; o < 5 ; o = o + 0.5)//lambda
		{
     	   for(double p = 50 ; p < 2000 ; p = p + 50 )  //frac_sig
  		   {
      		 for(double q = 50 ; q < 1100 ; q = q + 50 )  //loop frac_bkg
  		     {*/
//Roofit Variables
RooRealVar mean1("mean1","mean1",3.093,2.9,3.2);
RooRealVar sigma1("sigma1","sigma1",0.03,0.01,0.09);
RooRealVar alpha("alpha","tail shift",2.1,0.2,3.0);
RooRealVar npow("npow","power order",0.4,0.1,2.5);
RooRealVar lambda("lambda", "slope", 2.1,0.1, 4.);

//Crystall Ball Fit (S I G N A L)
RooCBShape cball1("cball1","cball1",mass,mean1,sigma1,alpha,npow);

//ROOFIT EXPONENTIAL (B A C K G R O U N D)
RooExponential expo("expo", "exponential PDF", mass, lambda);

//with only Crystal Ball 
//Mean: 3.093 ,Sigma: 0.03 ,Alpha: 2.1 ,Power: 0.4 ,chi^2 = 96.7902

//------------------------------------------------
// A d d  s i g n a l   a n d   b a c k g r o u n d
// ------------------------------------------------
// Sum the composite signal and background 
//RooRealVar sig_frac("sig_frac" ,"fraction of signal",0.5,0.1,1.) ;
//RooRealVar bkg_frac("bkg_frac","fraction of background",0.92,0.1,1.3) ;
RooRealVar bkg_frac("bkg_frac","fraction of background",300,0,1000);
RooRealVar sig_frac("sig_frac","fraction of signal",1611,0,10000);
RooAddPdf  sigbkg("sigbkg","CBall1 + Expo",RooArgList(cball1,expo),RooArgList(sig_frac,bkg_frac));

// Fitting
sigbkg.fitTo(rooNoCutMass, Extended());
//sigbkg.fitTo(rooNoCutMass);

//Creating a frame
RooPlot* frame = mass.frame(Title("Signal + Background"));

//Plots
rooNoCutMass.plotOn(frame,Name("Data"));
cball1.plotOn(frame,LineStyle(kDashed),LineColor(kGreen));
//expo.plotOn(frame,LineStyle(kDashed),LineColor(kRed));
sigbkg.plotOn(frame,Name("SPLUSBK"),LineStyle(kDashed),LineColor(kBlue));
sigbkg.plotOn(frame,Components("exp*"),LineStyle(kDashed),LineColor(kRed));

// Sig + Bck
RooArgSet * pars = sigbkg.getParameters(rooNoCutMass);
int nfloatpars = pars->selectByAttrib("Constant",kFALSE)->getSize(); 
double mychsq = frame->chiSquare("SPLUSBK","Data", nfloatpars); 
double myndof = massJpsiHist->GetNbinsX() - nfloatpars;
double alt_chi2 = frame->chiSquare(7);

cout << "nfloatpars: "<< nfloatpars << endl;
cout << "mychsq: " << mychsq << endl;
cout << "myndof: " << myndof << endl;
cout << "frame->chiSquare(7)= " << alt_chi2 << endl;
//sig_bkg.Print("t");

sigbkg.paramOn(frame,Layout(0.65,0.99,0.99),Format("NE"),Label(Form("#chi^{2}/ndf = %2.0f/%2.0f", myndof*mychsq, myndof)) );
//cball1.plotOn(frame,("Signal"), LineStyle(kDashed),LineColor(kGreen));
//sig_bkg.plotOn(frame,Components("expo*"),LineStyle(kDashed),LineColor(kRed));

//----------------------------------------------------------
//modifying frame bin
//----------------------------------------------------------
//rooNoCutMass.plotOn(frame,Binning(5)) ;
//cball1.plotOn(frame,Binning(5)) ;

//-------------------------------------------------
//C H I -  S Q U A R E
//--------------------------------------------------
//double chi2 = frame->chiSquare();
//cout << "chi^2 = " << chi2 << endl;

/*
			if (mychsq < 30.)
			{
//			cout << "========================================"<< endl;
//			cout << "Mean: "<< k << " ,Sigma: "<< l << " ,Alpha: "<< m << " ,Power: "<< n <<" ,chi^2 = " << frame->chiSquare() << endl;
			//textfile << "Mean: " << k << " ,mychsq: " << mychsq << endl;
//			cout << "========================================"<< endl;

   			textfile << "Mean: "<< k << " ,Sigma: "<< l << " ,Alpha: "<< m << " ,Power: "<< n << " ,Lambda: "<< o << " ,sig_frac: " << p << " ,bkg_frac: "<< q <<" ,chi^2 = " << chi2 << endl;
   
			}//end loop chi2<150

	   }//bkg_frac
	 }//sig_frac
	}//lambda
      }//end loop power1
    }//end loop alpha1
  }//end loop sigma1
}//end loop mean*/


//close file
textfile.close();

//Making a Canvas
TCanvas* Jpsi_teste_canvas = new TCanvas("Jpsi_teste_canvas","Jpsi_teste",1200,600);
frame->Draw();

//Save canvas as PNG
Jpsi_teste_canvas->SaveAs("/eos/user/r/ragomesd/analysis_2018/Jpsi_teste2.png");


}//end program
