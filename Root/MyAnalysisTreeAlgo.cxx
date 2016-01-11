#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <EventLoop/OutputStream.h>

#include <xAODEventInfo/EventInfo.h>

#include <MyAnalysis/MyAnalysisTree.h>
#include <MyAnalysis/MyAnalysisTreeAlgo.h>
#include <xAODAnaHelpers/TreeAlgo.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>


// this is needed to distribute the algorithm to the workers
ClassImp(MyAnalysisTreeAlgo)


EL::StatusCode MyAnalysisTreeAlgo :: treeInitialize ()
{

  Info("treeInitialize()", "%s", m_name.c_str() );

  TTree * outTree = new TTree(m_name.c_str(),m_name.c_str());
  if ( !outTree ) {
    Error("treeInitialize()","Failed to instantiate output tree!");
    return EL::StatusCode::FAILURE;
  }

  // get the file we created already
  TFile* treeFile = wk()->getOutputFile ("tree");
  m_helpTree = new MyAnalysisTree( outTree, treeFile, m_event, m_store, 1e0, m_debug, m_DC14 ); // 1e0 = MeV, default 1e3 = GeV

  // tell the tree to go into the file
  outTree->SetDirectory( treeFile );

  // choose if want to add tree to same directory as ouput histograms
  if ( m_outHistDir ) {
    wk()->addOutput( outTree );
  }

  m_helpTree->AddEvent(m_evtDetailStr);
  if ( !m_elContainerName.empty() )     {   m_helpTree->AddElectrons  (m_elDetailStr);      }
  if ( !m_jetContainerName.empty() )    {   m_helpTree->AddJets       (m_jetDetailStr);     }

  Info("treeInitialize()", "Successfully initialized output tree");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MyAnalysisTreeAlgo :: execute ()
{

  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("MyAnalysisTreeAlgo::execute()", HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store, m_debug) , "");
  const xAOD::VertexContainer* vertices(nullptr);
  RETURN_CHECK("TreeAlgo::execute()", HelperFunctions::retrieve(vertices, "PrimaryVertices", m_event, m_store, m_verbose) ,"");
  // get the primaryVertex
  const xAOD::Vertex* primaryVertex = HelperFunctions::getPrimaryVertex( vertices );

  m_helpTree->FillEvent( eventInfo );

  const xAOD::ElectronContainer* inElectrons(nullptr);
  const xAOD::JetContainer* inJets(nullptr);

  if ( !m_elContainerName.empty() ) {
    RETURN_CHECK("HTopMultilepTreeAlgo::execute()", HelperFunctions::retrieve(inElectrons, m_elContainerName, m_event, m_store, m_debug) , "");
    // sort inElectrons, and pass the reference to FillElectrons()
    const xAOD::ElectronContainer inElectronsSorted = HelperFunctions::sort_container_pt( inElectrons );
    m_helpTree->FillElectrons( &inElectronsSorted, primaryVertex );
  }
  if ( !m_jetContainerName.empty() ) {
    RETURN_CHECK("HTopMultilepTreeAlgo::execute()", HelperFunctions::retrieve(inJets, m_jetContainerName, m_event, m_store, m_debug) , "");
    // sort inJets, and pass the reference to FillJets()
    const xAOD::JetContainer inJetsSorted = HelperFunctions::sort_container_pt( inJets );
    m_helpTree->FillJets( &inJetsSorted, HelperFunctions::getPrimaryVertexLocation(vertices) );
  }

  // fill the tree
  m_helpTree->Fill();

  return EL::StatusCode::SUCCESS;

}
