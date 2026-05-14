#include <fun4all/Fun4AllUtils.h>
#include <qahtml/QADrawClient.h>

#include <calo/CaloDraw.h>
#include <calo/CaloGoodRunChecker.h>

#include <sPhenixStyle.C>

#include <fstream>
#include <vector>
#include <string>

R__LOAD_LIBRARY( libfun4all.so )
R__LOAD_LIBRARY( libqadrawcalo.so )


struct CaloCheckerValues_t {
  int runnumber = -1;
  int n_events= 0;
  bool cemc_isgood = false;
  bool cemc_fails_events = false;
  bool cemc_fails_badtowers = false;
  bool cemc_fails_timing = false;
  bool cemc_fails_vertex = false; 
  int cemc_hot_towers = 999999;
  int cemc_cold_towers = 999999;
  int cemc_dead_towers = 999999;
  float cemc_time_mean = 999.9;
  float cemc_time_sigma = 999.9;
  bool ihcal_isgood = false;
  bool ihcal_fails_events = false;
  bool ihcal_fails_badtowers = false;
  bool ihcal_fails_timing = false;
  bool ihcal_fails_vertex = false;
  int ihcal_hot_towers = 999999;
  int ihcal_cold_towers = 999999;
  int ihcal_dead_towers = 999999;
  float ihcal_time_mean = 999.9;
  float ihcal_time_sigma = 999.9;
  bool ohcal_isgood = false;
  bool ohcal_fails_events = false;  
  bool ohcal_fails_badtowers = false;
  bool ohcal_fails_timing = false;
  bool ohcal_fails_vertex = false;
  int ohcal_hot_towers = 999999;
  int ohcal_cold_towers = 999999;
  int ohcal_dead_towers = 999999;
  float ohcal_time_mean = 999.9;
  float ohcal_time_sigma = 999.9;
  float vtxz_mean = 999.9;
  float vtxz_sigma = 999.9;

  int ref_cemc_hot_towers = 999999;
  int ref_cemc_cold_towers = 999999;
  int ref_cemc_dead_towers = 999999;
  int ref_ihcal_hot_towers = 999999;
  int ref_ihcal_cold_towers = 999999;
  int ref_ihcal_dead_towers = 999999;
  int ref_ohcal_hot_towers = 999999;
  int ref_ohcal_cold_towers = 999999;
  int ref_ohcal_dead_towers = 999999;
};

CaloCheckerValues_t check_calo(const std::string & rootfile )
{

  CaloCheckerValues_t values;

  auto runseg = Fun4AllUtils::GetRunSegment( rootfile );
  int run_number = runseg.first;
  int segment = runseg.second;

  values.runnumber = run_number;

  std::cout << " Checking calo file: " << rootfile << std::endl;

  QADrawClient *cl = QADrawClient::instance();
  CaloDraw *ex = new CaloDraw();
  cl->registerDrawer(ex);
  cl->ReadHistogramsFromFile(rootfile);

  // Tower masking & good/bad run determination
  CaloGoodRunChecker* ch = new CaloGoodRunChecker();
  ex->SetCaloChecker(ch);
  ch->SetHistfile(rootfile);
  std::string mapsfile = ch->MakeHotColdDeadMaps();

  //Reference Hist
  ch->SetRefHistfile("/sphenix/data/data02/sphnxpro/QAhtml/aggregated/run3auau/physics/ana492_2025p003_v005/caloy2calib/run_00067500_00067600/HIST_CALOQA_run3auau_ana492_2025p003_v005-00067526-9999.root");
  std::string ref_mapsfile = ch->MakeHotColdDeadMapsRef();

  ch->CemcCheckGoodRun();
  TCanvas* cemc_summ = ch->CemcMakeSummary();
  ex->SetCemcSummary(cemc_summ);
  values.n_events = ch->n_events;
  values.cemc_isgood = ch->cemc_isgood;
  values.cemc_fails_events = ch->cemc_fails_events;
  values.cemc_fails_badtowers = ch->cemc_fails_badtowers;
  values.cemc_fails_timing = ch->cemc_fails_timing;
  values.cemc_fails_vertex = ch->cemc_fails_vertex;
  values.cemc_hot_towers = ch->cemc_hot_towers;
  values.cemc_cold_towers = ch->cemc_cold_towers;
  values.cemc_dead_towers = ch->cemc_dead_towers;
  values.cemc_time_mean = ch->cemc_time_mean;
  values.cemc_time_sigma = ch->cemc_time_sigma;

  ch->ihcalCheckGoodRun();
  TCanvas* ihcal_summ = ch->ihcalMakeSummary();
  ex->SetihcalSummary(ihcal_summ);
  values.ihcal_isgood = ch->ihcal_isgood;
  values.ihcal_fails_events = ch->ihcal_fails_events;
  values.ihcal_fails_badtowers = ch->ihcal_fails_badtowers;
  values.ihcal_fails_timing = ch->ihcal_fails_timing;
  values.ihcal_fails_vertex = ch->ihcal_fails_vertex;
  values.ihcal_hot_towers = ch->ihcal_hot_towers;
  values.ihcal_cold_towers = ch->ihcal_cold_towers;
  values.ihcal_dead_towers = ch->ihcal_dead_towers;
  values.ihcal_time_mean = ch->ihcal_time_mean;
  values.ihcal_time_sigma = ch->ihcal_time_sigma;


  // oHCal
  ch->ohcalCheckGoodRun();
  TCanvas* ohcal_summ = ch->ohcalMakeSummary();
  ex->SetohcalSummary(ohcal_summ);
  values.ohcal_isgood = ch->ohcal_isgood;
  values.ohcal_fails_events = ch->ohcal_fails_events;
  values.ohcal_fails_badtowers = ch->ohcal_fails_badtowers;
  values.ohcal_fails_timing = ch->ohcal_fails_timing;
  values.ohcal_fails_vertex = ch->ohcal_fails_vertex;
  values.ohcal_hot_towers = ch->ohcal_hot_towers;
  values.ohcal_cold_towers = ch->ohcal_cold_towers;
  values.ohcal_dead_towers = ch->ohcal_dead_towers;
  values.ohcal_time_mean = ch->ohcal_time_mean;
  values.ohcal_time_sigma = ch->ohcal_time_sigma;
  values.vtxz_mean = ch->vtxz_mean;
  values.vtxz_sigma = ch->vtxz_sigma;


  cl->ReadHistogramsFromFile(mapsfile.c_str());
  cl->MakeHtml();
  ch->DeleteHotColdDeadMaps();

  std::cout << "Writing emcal_auto to run triage DB... ";
  ch->CaloWriteDB("emcal");
  std::cout << "Done!!" << std::endl;
  std::cout << "Writing ihcal_auto to run triage DB... ";
  ch->CaloWriteDB("ihcal");
  std::cout << "Done!" << std::endl;
  std::cout << "Writing ohcal_auto to run triage DB... ";
  ch->CaloWriteDB("ohcal");
  std::cout << "Done!" << std::endl;

  // delete cl;
  // delete ex;
  delete ch;

  return values;
}


void checkall_calo( 
  const std::string & rootfile = "/sphenix/user/tmengel/GRL/run3pp_histcaloqa/rootfiles/HIST_CALOQA_run3pp_new_newcdbtag_v008-00079146-9999.root",
  const std::string & outfile = "/sphenix/user/tmengel/GRL/run3pp_histcaloqa/values/00079146.root"
)
{


  SetsPhenixStyle();
  auto values = check_calo( rootfile );
  // std::string outfile = "/sphenix/user/tmengel/GRL/run3pp_histcaloqa/values/" + std::to_string( values.runnumber ) + ".root";
  auto out = TFile::Open( outfile.c_str(), "RECREATE" );
  out->cd();
  auto t = new TTree( "T","T" );
  t->Branch( "runnumber", &values.runnumber, "runnumber/I" );
  t->Branch( "n_events", &values.n_events, "n_events/I" );
  t->Branch( "cemc_isgood", &values.cemc_isgood, "cemc_isgood/O" );
  t->Branch( "cemc_fails_events", &values.cemc_fails_events, "cemc_fails_events/O" );
  t->Branch( "cemc_fails_badtowers", &values.cemc_fails_badtowers, "cemc_fails_badtowers/O" );
  t->Branch( "cemc_fails_timing", &values.cemc_fails_timing, "cemc_fails_timing/O" );
  t->Branch( "cemc_fails_vertex", &values.cemc_fails_vertex, "cemc_fails_vertex/O" );
  t->Branch( "cemc_hot_towers", &values.cemc_hot_towers, "cemc_hot_towers/I" );
  t->Branch( "cemc_cold_towers", &values.cemc_cold_towers, "cemc_cold_towers/I" );
  t->Branch( "cemc_dead_towers", &values.cemc_dead_towers, "cemc_dead_towers/I" );
  t->Branch( "cemc_time_mean", &values.cemc_time_mean, "cemc_time_mean/F" );
  t->Branch( "cemc_time_sigma", &values.cemc_time_sigma, "cemc_time_sigma/F" );
  t->Branch( "ihcal_isgood", &values.ihcal_isgood, "ihcal_isgood/O" );
  t->Branch( "ihcal_fails_events", &values.ihcal_fails_events, "ihcal_fails_events/O" );
  t->Branch( "ihcal_fails_badtowers", &values.ihcal_fails_badtowers, "ihcal_fails_badtowers/O" );
  t->Branch( "ihcal_fails_timing", &values.ihcal_fails_timing, "ihcal_fails_timing/O" );
  t->Branch( "ihcal_fails_vertex", &values.ihcal_fails_vertex, "ihcal_fails_vertex/O" );
  t->Branch( "ihcal_hot_towers", &values.ihcal_hot_towers, "ihcal_hot_towers/I" );
  t->Branch( "ihcal_cold_towers", &values.ihcal_cold_towers, "ihcal_cold_towers/I" );
  t->Branch( "ihcal_dead_towers", &values.ihcal_dead_towers, "ihcal_dead_towers/I" );
  t->Branch( "ihcal_time_mean", &values.ihcal_time_mean, "ihcal_time_mean/F" );
  t->Branch( "ihcal_time_sigma", &values.ihcal_time_sigma, "ihcal_time_sigma/F" );
  t->Branch( "ohcal_isgood", &values.ohcal_isgood, "ohcal_isgood/O" );
  t->Branch( "ohcal_fails_events", &values.ohcal_fails_events, "ohcal_fails_events/O" );
  t->Branch( "ohcal_fails_badtowers", &values.ohcal_fails_badtowers, "ohcal_fails_badtowers/O" );
  t->Branch( "ohcal_fails_timing", &values.ohcal_fails_timing, "ohcal_fails_timing/O" );
  t->Branch( "ohcal_fails_vertex", &values.ohcal_fails_vertex, "ohcal_fails_vertex/O" );
  t->Branch( "ohcal_hot_towers", &values.ohcal_hot_towers, "ohcal_hot_towers/I" );
  t->Branch( "ohcal_cold_towers", &values.ohcal_cold_towers, "ohcal_cold_towers/I" );
  t->Branch( "ohcal_dead_towers", &values.ohcal_dead_towers, "ohcal_dead_towers/I" );
  t->Branch( "ohcal_time_mean", &values.ohcal_time_mean, "ohcal_time_mean/F" );
  t->Branch( "ohcal_time_sigma", &values.ohcal_time_sigma, "ohcal_time_sigma/F" );
  t->Branch( "vtxz_mean", &values.vtxz_mean, "vtxz_mean/F" );
  t->Branch( "vtxz_sigma", &values.vtxz_sigma, "vtxz_sigma/F" );
  t->Fill();
  out->Write();
  out->Close();
  std::cout << "Wrote calo checker values to " << outfile << std::endl; 
  
  gSystem->Exit(0);
  return ;
}
