////////////////////////////////////////////////////////////////////////
// Class:       Supera
// Module Type: analyzer
// File:        Supera_module.cc
//
// Generated at Wed Dec 30 15:55:44 2015 by Taritree Wongjirad using artmod
// from cetpkgsupport v1_10_01.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "caffe.pb.h"

class Supera;

class Supera : public art::EDAnalyzer {
public:
  explicit Supera(fhicl::ParameterSet const & p);
  // The destructor generated by the compiler is fine for classes
  // without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  Supera(Supera const &) = delete;
  Supera(Supera &&) = delete;
  Supera & operator = (Supera const &) = delete;
  Supera & operator = (Supera &&) = delete;

  // Required functions.
  void analyze(art::Event const & e) override;


private:

  // Declare member data here.

};


Supera::Supera(fhicl::ParameterSet const & p)
  :
  EDAnalyzer(p)  // ,
 // More initializers here.
{}

void Supera::analyze(art::Event const & e)
{
  // Implementation of required member function here.
}

DEFINE_ART_MODULE(Supera)
