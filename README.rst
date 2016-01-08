ASG Analysis Framework Review
=============================

1. Show how your framework/toolkit would be used with it's minimal (i.e. default) configuration to:
        - select events with at least 2 electrons and 2 jets
        - select the 2 leading electrons (pt ordered)
        - select the 2 leading jets (pt ordered)
        - calculate invariant mass of these 4 objects and add this to an output ntuple or histogram or xAOD
        - store the corresponding electron trigger efficiency scale factor for the electron pair

2. In the above example, how would you allow a user to write code to customize the selection of the 'signal' jets (e.g. select specific ranges of eta and phi)?

3. In the above example, how would you change the 'baseline' (pre-overlap removal) electron selection?

4. How would you activate or otherwise handle systematics? (Please remember we plan a separate week to discuss systematics, so just show how you would turn on systematics, if it's a relatively easily thing to show)

Dependencies
------------

This depends on the analysis framework ``xAODAnaHelpers``::

  git clone git@github.com:UCATLAS/xAODAnaHelpers
  python xAODAnaHelpers/scripts/checkoutASGtags.py 2.3.38


Installing
----------

To install, simply run::

  rc clean
  rc find_packages
  rc compile

and we are good to go.

Running
-------

You can run the code on an example file::

  xAH_run.py --files /share/t3data3/kratsg/xAODs/HFSF/pflowTest.root \
             --config ASG_AnalysisFrameworkReview/data/config.py direct