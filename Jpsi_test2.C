using namespace RooFit ;

double Jpsi_test2()
{


//Opening a root file and getting a TH1D from it
TFile* file = new TFile("Jpsi.root","read"); 
TH1D *massJpsiHist = (TH1D *)file->Get("h_DimuonsOppositeChargeEtaJpsi_M2_Double");


RooRealVar mass("mass","mass",2.7,3.5);

RooDataHist rooNoCutMass("rooNoCutMass","rooNoCutMass",mass,Import(*massJpsiHist));

ofstream textfile;
textfile.open("chi2_2.txt");

/*
for(double k = 3.090 ; k < 3.100 ; k = k + 0.001 ) //loop mean1
{
  for(double l = 0.01 ; l < 0.09 ; l = l + 0.01 )  //loop sigma1
  {
    for(double m = 0.1 ; m < 3.0 ; m = m + 0.1) //loop alpha1
    {
      for(double n = 0.1 ; n < 2.5 ; n = n + 0.1) //loop power
      {
	for(double o = 3.090 ; o < 3.100 ; o = o + 0.001) //loop mean1
		{
     	   /*for(double p = 0.01 ; p < 0.09 ; p = p + 0.01 )  //loop sigma2
  		   {
      		 for(double q = 0.1 ; q < 1.3 ; q = q + 0.1 )  //loop sigma2
  		     {*/
//Roofit Variables
RooRealVar mean1("mean1","mean1",3.093,2.9,3.2);
RooRealVar sigma1("sigma1","sigma1",0.027,0.01,0.09);
RooRealVar alpha("alpha","tail shift",2.15,0.2,3.0);
RooRealVar npow("npow","power order",0.40,0.1,2.5);

RooRealVar lambda("lambda", "slope", 2.6, 0.1, 4.);

//ROOFIT EXPONENTIAL
RooExponential expo("expo", "exponential PDF", mass, lambda);

//with only Crystal Ball 
//Mean: 3.093 ,Sigma: 0.03 ,Alpha: 2.1 ,Power: 0.4 ,chi^2 = 96.7902

//Crystall Ball Fit (S I G N A L)
RooCBShape cball1("cball1","cball1",mass,mean1,sigma1,alpha,npow);

// A d d  s i g n a l   a n d   b a c k g r o u n d
// ------------------------------------------------
// Sum the composite signal and background 
RooRealVar bkgfrac2("bkgfrac2","fraction of background",0.92,0.1,1.3) ;
RooAddPdf  model2("model2","CBall1 + Expo",RooArgList(cball1,expo),bkgfrac2);

//Creating a frame
RooPlot* frame = mass.frame(Title("Signal + Background"));

//Plot of Mass without Cuts
rooNoCutMass.plotOn(frame,Name("theData"));

//Plot of Crystal Ball fit
model2.plotOn(frame,Name("ThePdf"));

RooArgSet * pars = model2.getParameters(rooNoCutMass);
int nfloatpars = pars->selectByAttrib("Constant",kFALSE)->getSize(); 
double mychsq = frame->chiSquare("thePdf","theData", nfloatpars); 
double myndof = massJpsiHist->GetNbinsX() - nfloatpars;

model2.paramOn(frame,Layout(0.65,0.99,0.99),Format("NE"),Label(Form("#chi^{2}/ndf = %2.0f/%2.0f", myndof*mychsq, myndof))
                    );
cball1.plotOn(frame,("Signal"), LineStyle(kDashed),LineColor(kGreen));
model2.plotOn(frame,Components("expo*"),LineStyle(kDashed),LineColor(kRed));


//modifying frame bin
//rooNoCutMass.plotOn(frame,Binning(5)) ;
//cball1.plotOn(frame,Binning(5)) ;

double chi2 = frame->chiSquare();
//-------------------------------------------------
//C H I -  S Q U A R E
//--------------------------------------------------
cout << "chi^2 = " << chi2 << endl;

/*
			if (chi2 < 100)
			{
			cout << "========================================"<< endl;
			cout << "Mean: "<< k << " ,Sigma: "<< l << " ,Alpha: "<< m << " ,Power: "<< n <<" ,chi^2 = " << frame->chiSquare() << endl;
			cout << "========================================"<< endl;

   			textfile << "Mean: "<< k << " ,Sigma: "<< l << " ,Alpha: "<< m << " ,Power: "<< n <<" ,chi^2 = " << chi2 << endl;
   
			}//end loop chi2<150

			}
		  }
		}
      }//end loop power1
    }//end loop alpha1
  }//end loop sigma1
}//end loop mean
*/
textfile.close();

TCanvas* Jpsi_teste_canvas = new TCanvas("Jpsi_teste_canvas","Jpsi_teste",1200,600);

frame->Draw();

Jpsi_teste_canvas->SaveAs("/eos/user/r/ragomesd/analysis_2018/Jpsi_teste2.png");


}
