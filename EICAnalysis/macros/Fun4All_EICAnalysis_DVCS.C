int Fun4All_EICAnalysis_DVCS(
		     const int nEvents = 10,
		     const char * inputFile = "example_tesst.dat",
		     const char * outputFile = "G4sPHENIXCells.root",
		     const char * embed_input_file = "/sphenix/sim/sim01/production/2016-07-12/sHijing/spacal2d/G4Hits_sPHENIX_sHijing-0-4.4fm.list"
		       )
{
  //===============
  // Input options
  //===============

  // Either:
  // read previously generated g4-hits files, in this case it opens a DST and skips
  // the simulations step completely. The G4Setup macro is only loaded to get information
  // about the number of layers used for the cell reco code
  //
  // In case reading production output, please double check your G4Setup_sPHENIX.C and G4_*.C consistent with those in the production macro folder
  // E.g. /sphenix/sim//sim01/production/2016-07-21/single_particle/spacal2d/
  const bool readhits = false;
  // Or:
  // read files in HepMC format (typically output from event generators like hijing or pythia)
  const bool readhepmc = false; // read HepMC files
  // Or:
  // Use pythia
  const bool runpythia8 = false;
  const bool runpythia6 = false;
  const bool runhepgen = true;
  // else
  // Use particle generator (default simple generator)
  // or gun/ very simple generator
  const bool usegun = false;

  const bool readeictree = false;

  gSystem->Load("libfun4all.so");
  //  gSystem->Load("libg4detectors.so");
  gSystem->Load("libphhepmc.so");
  gSystem->Load("libg4testbench.so");
  gSystem->Load("libg4hough.so");
  //  gSystem->Load("libg4eval.so");
  gSystem->Load("libeicana.so");

  // establish the geometry and reconstruction setup
  //  gROOT->LoadMacro("G4Setup_sPHENIX.C");
  //  G4Init(do_svtx,do_preshower,do_cemc,do_hcalin,do_magnet,do_hcalout,do_pipe);

  //  int absorberactive = 1; // set to 1 to make all absorbers active volumes
  //  const string magfield = "1.5"; // if like float -> solenoidal field in T, if string use as fieldmap name (including path)
  const string magfield = "/phenix/upgrades/decadal/fieldmaps/sPHENIX.2d.root"; // if like float -> solenoidal field in T, if string use as fieldmap name (including path)
  const float magfield_rescale = 1.4/1.5; // scale the map to a 1.4 T field

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  // just if we set some flags somewhere in this macro
  //  recoConsts *rc = recoConsts::instance();
  // By default every random number generator uses
  // PHRandomSeed() which reads /dev/urandom to get its seed
  // if the RANDOMSEED flag is set its value is taken as seed
  // You ca neither set this to a random value using PHRandomSeed()
  // which will make all seeds identical (not sure what the point of
  // this would be:
  //  rc->set_IntFlag("RANDOMSEED",PHRandomSeed());
  // or set it to a fixed value so you can debug your code
  //  rc->set_IntFlag("RANDOMSEED", 12345);

  //-----------------
  // Event generation
  //-----------------

  if (readhits)
    {
      // Get the hits from a file
      // The input manager is declared later
    }
  else if (readhepmc)
    {
      // this module is needed to read the HepMC records into our G4 sims
      // but only if you read HepMC input files
      HepMCNodeReader *hr = new HepMCNodeReader();
      se->registerSubsystem(hr);
    }
  /* Read EICTree files */
  else if (readeictree)
    {
      // this module is needed to read the EICTree style records into our G4 sims
      ReadEICFiles *eicr = new ReadEICFiles();
      eicr->OpenInputFile(inputFile);

      se->registerSubsystem(eicr);

      HepMCNodeReader *hr = new HepMCNodeReader();
      se->registerSubsystem(hr);

    }
  else if (runpythia8)
    {
      gSystem->Load("libPHPythia8.so");
      
      PHPythia8* pythia8 = new PHPythia8();
      // see coresoftware/generators/PHPythia8 for example config
      pythia8->set_config_file("phpythia8.cfg"); 
      se->registerSubsystem(pythia8);

      HepMCNodeReader *hr = new HepMCNodeReader();
      se->registerSubsystem(hr);
    }
  else if (runpythia6)
    {
      gSystem->Load("libPHPythia6.so");

      PHPythia6 *pythia6 = new PHPythia6();
      pythia6->set_config_file("phpythia6.cfg");
      se->registerSubsystem(pythia6);

      HepMCNodeReader *hr = new HepMCNodeReader();
      se->registerSubsystem(hr);
    }
  else if (runhepgen)
    {
      gSystem->Load("libsHEPGen.so");

      sHEPGen *hepgen = new sHEPGen();
      hepgen->set_datacard_file("hepgen_dvcs.data");
      hepgen->set_momentum_electron(20);
      hepgen->set_momentum_hadron(250);
      se->registerSubsystem(hepgen);

      //      HepMCNodeReader *hr = new HepMCNodeReader();
      //      se->registerSubsystem(hr);
    }
  else
    {
      // toss low multiplicity dummy events
      PHG4SimpleEventGenerator *gen = new PHG4SimpleEventGenerator();
      gen->add_particles("e-",1); // mu+,e+,proton,pi+,Upsilon
      // gen->add_particles("e+",5); // mu-,e-,anti_proton,pi-
      if (readhepmc)
	{
	  gen->set_reuse_existing_vertex(true);
	  gen->set_existing_vertex_offset_vector(0.0, 0.0, 0.0);
	}
      else
	{
	  gen->set_vertex_distribution_function(PHG4SimpleEventGenerator::Uniform,
						PHG4SimpleEventGenerator::Uniform,
						PHG4SimpleEventGenerator::Uniform);
	  gen->set_vertex_distribution_mean(0.0, 0.0, 0.0);
	  gen->set_vertex_distribution_width(0.0, 0.0, 5.0);
	}
      gen->set_vertex_size_function(PHG4SimpleEventGenerator::Uniform);
      gen->set_vertex_size_parameters(0.0, 0.0);
      gen->set_eta_range(-0.5, 0.5);
      gen->set_phi_range(-1.0 * TMath::Pi(), 1.0 * TMath::Pi());
      gen->set_pt_range(0.1, 10.0);
      gen->Embed(1);
      gen->Verbosity(0);
      if (! usegun)
	{
	  se->registerSubsystem(gen);
	}
      else
	{
	  PHG4ParticleGun *gun = new PHG4ParticleGun();
	  //  gun->set_name("anti_proton");
	  gun->set_name("geantino");
	  gun->set_vtx(0, 0, 0);
	  gun->set_mom(10, 0, 0.01);
	  // gun->AddParticle("geantino",1.7776,-0.4335,0.);
	  // gun->AddParticle("geantino",1.7709,-0.4598,0.);
	  // gun->AddParticle("geantino",2.5621,0.60964,0.);
	  // gun->AddParticle("geantino",1.8121,0.253,0.);
	  //	  se->registerSubsystem(gun);
	  PHG4ParticleGenerator *pgen = new PHG4ParticleGenerator();
          pgen->set_name("geantino");
	  pgen->set_z_range(0,0);
	  pgen->set_eta_range(0.01,0.01);
	  pgen->set_mom_range(10,10);
	  pgen->set_phi_range(5.3./180.*TMath::Pi(),5.7./180.*TMath::Pi());
	  se->registerSubsystem(pgen);
	  pgen = new PHG4ParticleGenerator();
          pgen->set_name("geantino");
	  pgen->set_z_range(0,0);
	  pgen->set_eta_range(0.01,0.01);
	  pgen->set_mom_range(10,10);
	  pgen->set_phi_range(-0.2./180.*TMath::Pi(),0.2./180.*TMath::Pi());
	  se->registerSubsystem(pgen);
	}
    }


  if (readhits)
    {
      // Hits file
      Fun4AllInputManager *hitsin = new Fun4AllDstInputManager("DSTin");
      hitsin->fileopen(inputFile);
      se->registerInputManager(hitsin);
    }
  if (readhepmc)
    {
      Fun4AllInputManager *in = new Fun4AllHepMCInputManager( "DSTIN");
      se->registerInputManager( in );
      se->fileopen( in->Name().c_str(), inputFile );
    }
  else
    {
      // for single particle generators we just need something which drives
      // the event loop, the Dummy Input Mgr does just that
      //      Fun4AllInputManager *in = new Fun4AllDummyInputManager( "JADE");
      //      se->registerInputManager( in );
    }


  //--------------
  // Analysis modules
  //--------------
  DISKinematics *mcana = new DISKinematics(outputFile);
  se->registerSubsystem( mcana );


  //-----------------
  // Event processing
  //-----------------
  if (nEvents < 0)
    {
      return;
    }
  // if we run the particle generator and use 0 it'll run forever
  if (nEvents == 0 && !readhits && !readhepmc)
    {
      cout << "using 0 for number of events is a bad idea when using particle generators" << endl;
      cout << "it will run forever, so I just return without running anything" << endl;
      return;
    }

  se->run(nEvents);

  //-----
  // Exit
  //-----

  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  gSystem->Exit(0);
}
