#ifndef MyAnalysis_MyAnalysisTree_H
#define MyAnalysis_MyAnalysisTree_H

// package include(s):
#include <xAODAnaHelpers/HelpTreeBase.h>

// EDM include(s):
#include <xAODEventInfo/EventInfo.h>
#include <xAODBase/IParticleContainer.h>

// Infrastructure include(s):
#include <xAODRootAccess/TEvent.h>
#include <xAODRootAccess/TStore.h>

// ROOT include(s):
#include <TTree.h>
#include <TFile.h>

class MyAnalysisTree : public HelpTreeBase
{

  private:

    float m_mjjee;

  public:

    MyAnalysisTree( TTree* tree, TFile* file, xAOD::TEvent* event, xAOD::TStore* store, const float units = 1e3, bool debug = false, bool DC14 = false );
    ~MyAnalysisTree();

    // overload ...User methods of HelpTreeBase to add custom stuff to the tree
    void AddEventUser(const std::string detailStrUser = "");
    void ClearEventUser();
    void FillEventUser( const xAOD::EventInfo* );

};
#endif
