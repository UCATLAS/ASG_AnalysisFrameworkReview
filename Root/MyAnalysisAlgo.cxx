// EL include(s):
#include <EventLoop/Job.h>
#include <EventLoop/Worker.h>
#include <EventLoop/OutputStream.h>

// EDM include(s):
#include <xAODEventInfo/EventInfo.h>
#include <xAODEgamma/ElectronContainer.h>
#include <xAODJet/JetContainer.h>

// package include(s):
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>
#include <MyAnalysis/MyAnalysisAlgo.h>

// ROOT include(s):
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TLorentzVector.h>

// c++ include(s)
#include <stdexcept>


// this is needed to distribute the algorithm to the workers
ClassImp(MyAnalysisAlgo)


MyAnalysisAlgo :: MyAnalysisAlgo ()
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  Info("MyAnalysisAlgo()", "Calling constructor");

  m_inContainerName_Electrons = "";
  m_inContainerName_Jets      = "";

}

EL::StatusCode MyAnalysisAlgo :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  job.useXAOD();
  xAOD::Init( "MyAnalysisAlgo" ).ignore(); // call before opening first file

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode MyAnalysisAlgo :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  Info("histInitialize()", "Calling histInitialize");

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode MyAnalysisAlgo :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode MyAnalysisAlgo :: changeInput (bool /*firstFile*/)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode MyAnalysisAlgo :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  // count number of events
  m_eventCounter = 0;

  Info("initialize()", "MyAnalysisAlgo Interface succesfully initialized!" );

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode MyAnalysisAlgo :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if ( m_debug ) { Info("execute()", "Executing MyAnalysisAlgo..."); }

  if ( (m_eventCounter % 1000) == 0 ) {
    Info("execute()", "processed %i events", m_eventCounter);
  }

  ++m_eventCounter;

  // retrieve event info
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("MyAnalysisAlgo::execute()", HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store, m_verbose), "");

  // retrieve input containers
  const xAOD::ElectronContainer* inElectrons(nullptr);
  RETURN_CHECK("MyAnalysisAlgo::execute()", HelperFunctions::retrieve(inElectrons, m_inContainerName_Electrons, m_event, m_store,  m_verbose), "");
  const xAOD::JetContainer*      inJets(nullptr);
  RETURN_CHECK("MyAnalysisAlgo::execute()", HelperFunctions::retrieve(inJets, m_inContainerName_Jets, m_event, m_store,  m_verbose), "");

  // make sure containers are sorted by pT
  const xAOD::ElectronContainer inElectronsSorted = HelperFunctions::sort_container_pt( inElectrons );
  const xAOD::JetContainer inJetsSorted = HelperFunctions::sort_container_pt( inJets );

  // compute invariant mass of 2 leading jets + 2 leading electrons and store it as decoration

  static SG::AuxElement::Decorator< float >  mjjeeDecor("mjjee");
  float mjjee(-999.0);

  if ( inJetsSorted.size() > 1 && inElectronsSorted.size() > 1 ) {

    TLorentzVector j0 = inJetsSorted.at(0)->p4();
    TLorentzVector j1 = inJetsSorted.at(1)->p4();

    TLorentzVector e0 = inElectronsSorted.at(0)->p4();
    TLorentzVector e1 = inElectronsSorted.at(1)->p4();

    TLorentzVector comb = j0 + j1 + e0 + e1;

    mjjee = comb.M();
  }

  mjjeeDecor(*eventInfo) = mjjee;

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode MyAnalysisAlgo :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode MyAnalysisAlgo :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.

  Info("finalize()", "Deleting tool instances...");

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode MyAnalysisAlgo :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.

  return EL::StatusCode::SUCCESS;

}


