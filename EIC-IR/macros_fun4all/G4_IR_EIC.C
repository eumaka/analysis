
void IRInit() {

}

double IRSetup(PHG4Reco* g4Reco,
               const int absorberactive = 1,
               int verbosity = 0) {

  /* Increase world size to fit extended IR magnets */
  g4Reco->SetWorldSizeZ(25000);

  /* Define outer radius for all extneded IR magnets */
  double ir_magnet_outer_radius = 50;

  /* Read IR configuration file- this needs to go somewhere else using parameters and a .root file to store them*/
  // string irfile = "../input_ir/magnet_quad_test.dat";
  // string irfile = "../input_ir/magnet_dipole_test.dat";
  // string irfile = "../input_ir/linac-ring-proton-magnets-Version3.01-21apr2016.dat";
  string irfile = "../input_ir/proton-magnets-250GeV-opt2.dat";
  // string irfile = "../input_ir/updated-magnets-2017.dat";

  ifstream irstream(irfile.c_str());

  while(!irstream.eof()){
    string str;
    getline(irstream, str);
    if(str[0] == '#') continue; //for comments
    stringstream ss(str);

    string name;
    double center_z, center_x, center_y, aperture_radius, length, angle, B, gradient;

    ss >> name >> center_z >> center_x >> center_y >> aperture_radius >> length >> angle >> B >> gradient;

    if ( name == "" ) continue; //for empty lines

    /* convert units from m to cm */
    center_x *= 100;
    center_y *= 100;
    center_z *= 100;
    aperture_radius *= 100;
    length *= 100;

    /* convert angle from millirad to degrees */
    angle = (angle / 1000.) * (180./TMath::Pi());

    /* Place IR component */
    cout << "New IR component: " << name << " at x = " << center_x << ", y = " << center_y << ", z = " << center_z << ", angle = " << angle << endl;

    string volname = "IRMAGNET_";
    volname.append(name);

    PHG4BeamlineMagnetSubsystem *ir_magnet_i = new PHG4BeamlineMagnetSubsystem(volname, 0);
    ir_magnet_i->set_int_param("lengthviarapidity",0);
    ir_magnet_i->set_double_param("length",length);
    ir_magnet_i->set_double_param("radius",aperture_radius);
    ir_magnet_i->set_double_param("thickness", ir_magnet_outer_radius - aperture_radius);
    ir_magnet_i->set_double_param("place_x",center_x);
    ir_magnet_i->set_double_param("place_y",center_y);
    ir_magnet_i->set_double_param("place_z",center_z);
    ir_magnet_i->set_double_param("rot_y",angle);
    ir_magnet_i->set_string_param("material","G4_Galactic"); // magnet yoke material is 'vacuum'
    // ir_magnet_i->set_string_param("material","G4_Fe"); // magnet yoke material is 'iron'

    if ( B != 0 && gradient == 0.0 )
      {
        ir_magnet_i->set_string_param("magtype","dipole");
        ir_magnet_i->set_double_param("field_y",B);
      }
    else if ( B == 0 && gradient != 0.0 )
      {
        ir_magnet_i->set_string_param("magtype","quadrupole");
        ir_magnet_i->set_double_param("fieldgradient",gradient);
      }
    else if ( B == 0 && gradient == 0.0 )
      {
        ir_magnet_i->set_string_param("magtype","dipole");
        ir_magnet_i->set_double_param("fieldgradient",0);
        ir_magnet_i->set_double_param("field_y",0);
      }
    else
      {
        cout << "Error in G4_IR_EIC: Could not identify magnet type. Abort." << endl;
        return 1;
      }

    ir_magnet_i->SetActive(false);
    ir_magnet_i->OverlapCheck(overlapcheck);
    g4Reco->registerSubsystem(ir_magnet_i);
  }

  return 0;
}
