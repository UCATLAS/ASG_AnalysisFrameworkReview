#ifndef MyAnalysis_MyAnalysisTreeAlgo_H
#define MyAnalysis_MyAnalysisTreeAlgo_H

// EL include(s)
#include <EventLoop/Algorithm.h>

// package include(s):
#include <xAODAnaHelpers/TreeAlgo.h>
#include <MyAnalysis/MyAnalysisTree.h>

class MyAnalysisTreeAlgo : public TreeAlgo
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:

  // this is a standard constructor
  MyAnalysisTreeAlgo () : TreeAlgo() {};

  // these are the functions inherited from Algorithm

  // overload only the ones that somehow differ from the original methods in xAH::TreeAlgo (e.g., the ones manipulating m_helpTree)
  virtual EL::StatusCode treeInitialize ();
  virtual EL::StatusCode execute ();

  // this is needed to distribute the algorithm to the workers
  ClassDef(MyAnalysisTreeAlgo, 1);
};

#endif
