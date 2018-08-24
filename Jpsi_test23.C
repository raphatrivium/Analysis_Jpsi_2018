using namespace RooFit ;

double Jpsi_test23()
{

//Opening a root file and getting a TH1D wich contains the data
TFile* file = new TFile("Jpsi.root","read"); 
TH1D *massJpsiHist = (TH1D *)file->Get("h_DimuonsOppositeChargeEtaJpsi_M2_Double");

RooRealVar mass("mass","mass",2.9,3.3);
RooDataHist rooNoCutMass("rooNoCutMass","rooNoCutMass",mass,Import(*massJpsiHist));

//open a file
ofstream textfile;
textfile.open("chi2_23.txt");

//---------------------------------------------
//P A R A M E T E R S    C O M B I N A T I O N
//----------------------------------------------

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


//--------------------------------------------------
//Roofit Variables
//--------------------------------------------------
//RooRealVar mean1("mean1","mean1",3.093,2.970,3.200);
//RooRealVar sigma1("sigma1","sigma1",0.03,0.02,0.09);
//RooRealVar alpha("alpha","tail shift",2.1,0.2,3.0);
//RooRealVar npow("npow","power order",0.4,0.0,2.3);
//RooRealVar lambda("lambda", "slope", 30.0,0.1, 50.8);

RooRealVar mean1( "mean1", "mean1", 3.093, 2.960, 3.150);
RooRealVar sigma1( "sigma1", "sigma1", 0.020, 0.01, 0.10);
RooRealVar alpha( "alpha", "tail shift", 2.1, 0.3, 2.3);
RooRealVar npow( "npow", "power order", 0.4, 0.1, 12.5);
RooRealVar lambda( "lambda", "slope", 2.0, 0.1, 6.8);


//Crystall Ball Fit (S I G N A L)
RooCBShape cball1("cball1","cball1",mass,mean1,sigma1,alpha,npow);

//Gaussian (S I G N A L)
RooGaussian gauss("gauss","gauss",mass,mean1,sigma1);

//ROOFIT EXPONENTIAL (B A C K G R O U N D)
RooExponential expo("expo", "exponential PDF", mass, lambda);

//with only Crystal Ball 
//Mean: 3.093 ,Sigma: 0.03 ,Alpha: 2.1 ,Power: 0.4 ,chi^2 = 96.7902

//------------------------------------------------
//A D D  S I G N A L  +  S I G N A L
//------------------------------------------------
//RooRealVar sigCB_frac("sigCB_frac","fraction of crystal ball", 8100, 790 ,9300);
//RooRealVar sigGauss_frac("sigGauss_frac","fraction of Gaussian", 2650 , 300 ,3000);
RooRealVar sigCB_frac("sigCB_frac","fraction of crystal ball", 1000, 590 ,3400);
RooRealVar sigGauss_frac("sigGauss_frac","fraction of Gaussian", 640 , 200 ,650);
RooAddPdf sig("sig","Signal CB + Guss", RooArgList(cball1,gauss), RooArgList(sigCB_frac,sigGauss_frac));

//------------------------------------------------
// A d d  s i g n a l   a n d   b a c k g r o u n d
// ------------------------------------------------
// Sum the composite signal and background 
//RooRealVar bkg_frac("bkg_frac","fraction of background",300,100,350);
//RooRealVar sig_frac("sig_frac","fraction of signal",16011,1000,19000);
RooRealVar bkg_frac("bkg_frac","fraction of background",300,250,290);
RooRealVar sig_frac("sig_frac","fraction of signal",1620,1100,3100);
RooAddPdf  sigbkg("sigbkg","CBall1 + Expo",RooArgList(sig,expo),RooArgList(sig_frac,bkg_frac));

//F I T T I N G
sigbkg.fitTo(rooNoCutMass,Range(2.9,3.3), Extended());
//sigbkg.fitTo(rooNoCutMass);

//Creating a frame
RooPlot* frame = mass.frame(Title("Signal + Background"));

//Plots
rooNoCutMass.plotOn(frame,Name("Data"));
//cball1.plotOn(frame,LineStyle(kDashed),LineColor(kGreen));
//expo.plotOn(frame,LineStyle(kDashed),LineColor(kBlack));
sigbkg.plotOn(frame,Name("SPLUSBK"),LineStyle(kDashed),LineColor(kBlue));
sigbkg.plotOn(frame,Components("exp*"),LineStyle(kDashed),LineColor(kRed));

// Sig + Bck
RooArgSet * pars = sigbkg.getParameters(rooNoCutMass);
int nfloatpars = pars->selectByAttrib("Constant",kFALSE)->getSize(); 
double mychsq = frame->chiSquare("SPLUSBK","Data", nfloatpars); 
double myndof = massJpsiHist->GetNbinsX() - nfloatpars;
double alt_chi2 = frame->chiSquare(7);
 
cout << "--------------------------------------" << endl;
cout << "nfloatpars: "<< nfloatpars << endl;
cout << "mychsq: " << mychsq << endl;
cout << "myndof: " << myndof << endl;
cout << "frame->chiSquare(7)= " << alt_chi2 << endl;
cout << "--------------------------------------" << endl;
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

/*
// TLegend *legend_sr=FastLegend(x1+0.02,y2-0.3,x1+0.35,y2-0.02,0.045);
 TLegend *legend_sr=FastLegend(x1+0.02,y2-0.3,x1+0.35,y2-0.02,0.045);
  legend_sr->AddEntry(frame->findObject("xdata"),"Data","LEP");
  legend_sr->AddEntry(frame->findObject("xbackground"),"Background fit","L");
  legend_sr->AddEntry(frame->findObject("xsm"),"SM H","L");
  legend_sr->AddEntry(frame->findObject("xbsm"),"Best-fit BSM H","L");
  legend_sr->AddEntry(frame->findObject("xtotal"),"Total","L");
  frame->Draw();
  legend_sr->Draw("SAME");*/


TLegend *leg1 = new TLegend(0.65,0.13,0.86,0.37);
leg1->SetFillColor(kWhite);
leg1->AddEntry(frame->findObject("Data"),"Data","P");
leg1->AddEntry(frame->findObject("SPLUSBK"),"Sig + Bkg","L");
//leg1->AddEntry(frame->findObject(Components("exp*")),"Background","LP");
//leg1->AddEntry(sigbkg,"sigbkg","L");
//leg1->AddEntry(“data”,“Data”, “P”);
//leg1->AddEntry(sigbkg,“Signal + background”,“LP”);
//leg1->AddEntry(“background”,“Background only”, “LP”);
//leg1->AddEntry(“signal only”,“Signal only”, “LP”);
leg1->Draw();

//Save canvas as PNG
Jpsi_teste_canvas->SaveAs("/eos/user/r/ragomesd/analysis_2018/Jpsi_teste23.png");


}//end program
