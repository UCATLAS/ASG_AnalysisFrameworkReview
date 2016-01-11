#include <MyAnalysis/MyAnalysisTree.h>

MyAnalysisTree :: MyAnalysisTree( TTree* tree, TFile* file, xAOD::TEvent* event, xAOD::TStore* store, const float units, bool debug, bool DC14 ) :
  HelpTreeBase(tree, file, event, store, units, debug, DC14 )
{
  Info("MyAnalysisTree", "Creating output TTree");
}

MyAnalysisTree :: ~MyAnalysisTree() {}

void MyAnalysisTree::AddEventUser(const std::string detailStrUser)
{

  if ( m_debug ) { Info("AddEventUser()", "Adding branches w/ detail: %s", detailStrUser.c_str()); }

  m_tree->Branch("mjjee", &m_mjjee, "mjjee/F");

}

void MyAnalysisTree::ClearEventUser()
{
  m_mjjee = -999.0;
}

void MyAnalysisTree::FillEventUser( const xAOD::EventInfo* eventInfo )
{
  m_mjjee = ( eventInfo->isAvailable< float >( "mjjee" ) ) ? eventInfo->auxdecor< float >( "mjjee" ) : -999.0;
}

