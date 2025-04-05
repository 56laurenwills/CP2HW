#include <vector>
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TH1F.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "TSystem.h"

//Originally authored by the ROOT Team: https://root.cern/doc/master/hvector_8C.html


void write(int N=1e6, int basketsize=4e3, int vecsize=100) // that is the default autoflush value 
{
 
   TFile *f = TFile::Open(Form("hvector_%d_%d_%d.root",N,vecsize), "RECREATE");
   float percentComplete = 0.0f; 
   if (!f) { return; }
 
   std::vector<float> vpx;
   std::vector<float> vpy;
   std::vector<int> vpz;
   std::vector<float> vpt;   
   std::vector<int> vint;
 
   // Create a TTree
   TTree *t = new TTree("tvec","Tree with vectors");
   t->Branch("vpx",&vpx);
   t->Branch("vpy",&vpy);
   t->Branch("vpz",&vpz);   
   t->Branch("vpt",&vpt);
   t->Branch("vint",&vint); 
   t->SetBasketSize("*", basketsize);  
 
   gRandom->SetSeed();
   for (Int_t i = 0; i < N; i++) {

      // Monitor Progress
      //if(i%10000==0) {
	  // percentComplete =((float)i/N)*100.f;
      	   //std::cerr << "\rLOOP [write()]: " << percentComplete << "\% Complete" <<  std::flush;
      //}


      Int_t npx = (Int_t)(gRandom->Rndm(1)*vecsize);
 
      vpx.clear();
      vpy.clear();
      vpz.clear();
      vpt.clear();
      vint.clear();            
 
      for (Int_t j = 0; j < npx; ++j) {
 
         Float_t px,py,pt;
         Int_t pz,integer;
         gRandom->Rannor(px,py);
         pt = sqrt(px*px + py*py);
         pz = (int)gRandom->Rndm(1);
         
         integer = (int)(gRandom->Rndm(1)*npx+npx);
 
         vpx.emplace_back(px);
         vpy.emplace_back(py);
         vpz.emplace_back(pz);
         
         /// this gets pushed back twice on purpose
         vpt.emplace_back(pt);
         vpt.emplace_back(pt);         
         vint.emplace_back(integer);         
 
      }
      t->Fill();
   }
   
   t->Print();
   f->Write();
   
   delete f;
}
 
 
 
int main()
{
   int N = 1e6; // match this with N in readROOTFile.cpp
   int vecsize=100;
   std::vector<int> basketsize = {4000, 8000, 16000, 32000, 64000, 128000, 256000, 512000, 1024000};

   for (auto bsize : basketsize) {
      std::cerr << "basket size = " << bsize;
//   for (auto flush : flushes) {
//      << " and vec size = " << vecsize << std::endl;
      TBenchmark* gBenchmark= new TBenchmark();
      const char* form = Form("hvector_N%d_Sz%d", N, vecsize);
      gBenchmark->Start(form);
   // Timed write()
      write(N, bsize, vecsize);
      gBenchmark->Stop(form);
      gBenchmark->Show(form);
      delete gBenchmark;
   }
   return 0;
}
