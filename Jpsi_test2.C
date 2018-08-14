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
		for(double o = 3.090 ; o < 3.100 ; o = o + 0.001) //loop mean1
		{
     	   /*for(double p = 0.01 ; p < 0.09 ; p = p + 0.01 )  //loop sigma2
  		   {
      		 for(double q = 0.1 ; q < 1.3 ; q = q + 0.1 )  //loop sigma2
  		     {*/
//Roofit Variables
RooRealVar mean1("mean1","mean1",k,2.9,3.2);
RooRealVar sigma1("sigma1","sigma1",l,0.01,0.09);
RooRealVar alpha("alpha","tail shift",m,0.2,3.0);
RooRealVar npow("npow","power order",n,0.1,2.5);

//with only Crystal Ball 
//Mean: 3.093 ,Sigma: 0.03 ,Alpha: 2.1 ,Power: 0.4 ,chi^2 = 96.7902

//Crystall Ball Fit (S I G N A L)
RooCBShape cball1("cball1","cball1",mass,mean1,sigma1,alpha,npow);


//Gaussian Fit (B A C K G R O U N D)
RooRealVar mean2("mean2","mean2",o,2.9,3.2);
RooRealVar sigma2("sigma2","sigma2",p,0.01,1);
RooGaussian gauss1("gauss1","gauss1",mass,mean2,sigma2);


//Gaussian Fit (B A C K G R O U N D)
//RooRealVar mean3("mean3","mean3",3.097,2.9,3.2);
//RooRealVar sigma3("sigma2","sigma2",0.04,0.02,1);
//RooGaussian gauss2("gauss2","gauss2",mass,mean3,sigma3);


// A d d  s i g n a l   a n d   s i g n a l
// ------------------------------------------------
// Sum the composite signal and signal 
//RooRealVar bkgfrac1("bkgfrac1","background",0.9,0.,1.) ;
//RooAddPdf  model1("model1","gauss1 + Gauss1",RooArgList(gauss1,gauss2),bkgfrac1);


// A d d  s i g n a l   a n d   b a c k g r o u n d
// ------------------------------------------------
// Sum the composite signal and background 
RooRealVar bkgfrac2("bkgfrac2","fraction of background",q,0.1,1.3) ;
RooAddPdf  model2("model2","CBall1 + Gauss",RooArgList(cball1,gauss1),bkgfrac2);



//Creating a frame
RooPlot* frame = mass.frame(Title("Crystal Ball"));

//Plot of Mass without Cuts
rooNoCutMass.plotOn(frame,Name("theData"));

//Plot of Crystal Ball fit
model2.plotOn(frame,Name("model2"));

model2.paramOn(frame,Layout(0.65,0.99,0.99),Format("NE"),Label(Form("#chi^{2} = 96.7902")) );



//modifying frame bin
//rooNoCutMass.plotOn(frame,Binning(5)) ;
//cball1.plotOn(frame,Binning(5)) ;

double chi2 = frame->chiSquare();
//-------------------------------------------------
//C H I -  S Q U A R E
//--------------------------------------------------
//cout << "chi^2 = " << chi2 << endl;


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

textfile.close();
/*
//L O O P    T E S T
for( int i = 0; i < 3; i++) 
{
   // cout << "Result: " << i << endl;
  for( int j = 0; j < 3; j++)
  {
    cout << "Result: " << i <<","<< j << endl;
  }
}
*/
TCanvas* Jpsi_teste = new TCanvas("Jpsi_teste","Jpsi_teste",1200,600);

frame->Draw();

Jpsi_teste->SaveAs("/eos/user/r/ragomesd/analysis_2018/Jpsi_teste.png");



}
