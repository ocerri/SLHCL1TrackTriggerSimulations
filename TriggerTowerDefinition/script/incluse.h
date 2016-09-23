#include <cstdio>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>  // necessario per scrivere su file
#include <string>

#include "TH1D.h"
#include "TFile.h"
#include "TF1.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TLegend.h"

#include "TStopwatch.h"


#include "cebefo_style.h"

#ifndef pigreco
  #define pigreco 3.141592653589793
#endif

Int_t segno(Int_t a)
{
	if (a>=0) return 1;
	else return -1;
}


using namespace std;
TStopwatch* watch_progress_bar;// = new TStopwatch();
Int_t st_time_progress_bar = 0;

void show_progress_bar(int entry, int numberOfEntries){
	Int_t step = numberOfEntries>200 ?  numberOfEntries/ 200 : 2;
	if (entry==0 || entry%step==0 || entry==numberOfEntries-1){
      if (entry==0){
				watch_progress_bar = new TStopwatch();
				watch_progress_bar->Start();
    		cout << endl << Form("[--------------------]   0%% ") << flush;
				st_time_progress_bar = time(nullptr);
      }
      else if((entry+1)/(float)numberOfEntries == 1){
				watch_progress_bar->Stop();
    		cout << '\r' << Form("[####################]  100%%                          ") << endl;
				cout <<"Elapsed time: " << watch_progress_bar->RealTime() << " sec (real time)"<< endl << endl;
				delete watch_progress_bar;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.95){
    		cout << '\r' << Form("[###################-]  95%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.90){
    		cout << '\r' << Form("[##################--]  90%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.85){
    		cout << '\r' << Form("[#################---]  85%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.80){
    		cout << '\r' << Form("[################----]  80%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.75){
    		cout << '\r' << Form("[###############-----]  75%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.70){
    		cout << '\r' << Form("[##############------]  70%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.65){
    		cout << '\r' << Form("[#############-------]  65%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.60){
    		cout << '\r' << Form("[############--------]  60%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.55){
    		cout << '\r' << Form("[###########---------]  55%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.50){
    		cout << '\r' << Form("[##########----------]  50%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.45){
    		cout << '\r' << Form("[#########-----------]  45%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.40){
    		cout << '\r' << Form("[########------------]  40%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.35){
    		cout << '\r' << Form("[#######-------------]  35%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.30){
    		cout << '\r' << Form("[######--------------]  30%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.25){
    		cout << '\r' << Form("[#####---------------]  25%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.20){
    		cout << '\r' << Form("[####----------------]  20%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.15){
    		cout << '\r' << Form("[###-----------------]  15%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.10){
    		cout << '\r' << Form("[##------------------]  10%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries >= 0.05){
    		cout << '\r' << Form("[#-------------------]   5%% ") << flush;
      }
      else if((entry+1)/(float)numberOfEntries < 0.05){
    		cout << '\r' << Form("[--------------------]   0%% ") << flush;
      }
    }
    cout << flush;

	if (entry>0 && entry%step==0)
	{
		Int_t now = time(nullptr);
		Double_t timeleft = (numberOfEntries - (Double_t)entry)*((Double_t)now - st_time_progress_bar)/(Double_t)entry;
		if(timeleft<181)
		{
			cout << Form(" -  remaning:%5.0f s   ",timeleft) << flush;
		}
		else if(timeleft<10801)
		{
			timeleft /= 60.;
			cout << Form(" -  remaning:%5.1f min ",timeleft) << flush;
		}
		else
		{
			timeleft /= 3600.;
			cout << Form(" -  remaning:%5.1f h   ",timeleft) << flush;
		}
	}
}

Double_t hist_FWMH(TH1F * h1){
	Int_t bin1 = h1->FindFirstBinAbove(h1->GetMaximum()/2);
  Int_t bin2 = h1->FindLastBinAbove(h1->GetMaximum()/2);
  return h1->GetBinCenter(bin2) - h1->GetBinCenter(bin1);
}

UInt_t index_min_elem(vector<UInt_t> vec)
{
    std::vector<UInt_t>::iterator mins = std::min_element(vec.begin(), vec.end()); //returns all mins
    UInt_t min = mins[0];
    for(unsigned int i=0; i < vec.size(); i++)
    {
        //Note: could use fabs( (min - vec[i]) < 0.01) if worried about floating-point precision
        if(vec[i] == min)
            return i;
    }
    return -1;
}
