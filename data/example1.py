from xAH_config import xAH_config
c = xAH_config()

# input containers
electrons = "Electrons"
jets = "AntiKt4EMTopoJets"

# calibrated versions
electrons_calib = electrons+"Calib"
jets_calib = jets+"Calib"

# selected version
electrons_select = electrons_calib+"Select"
jets_select = jets_calib+"Select"

''' Set up all the algorithms '''
c.setalg("BasicEventSelection", {"m_name": "BasicEventSelection",
                                 "m_applyGRLCut": False,
                                 "m_cleanPowheg": False,
                                 "m_doPUreweighting": False,
                                 "m_applyPrimaryVertexCut": False,
                                 "m_applyEventCleaningCut": False,
                                 "m_applyCoreFlagsCut": False,
                                 "m_applyTriggerCut": False,
                                 "m_useMetaData": False,
                                 "m_checkDuplicatesData": False,
                                 "m_checkDuplicatesMC": False})

''' Calibrate the Electrons and Jets '''
c.setalg("ElectronCalibrator", {"m_name": "ElectronCalibrator",
                                "m_inContainerName": electrons,
                                "m_outContainerName": electrons_calib,
                                "m_sort": True,
                                "m_esModel": "es2015PRE",
                                "m_decorrelationModel": "FULL_ETACORRELATED_v1"})

c.setalg("JetCalibrator", {"m_name": "JetCalibrator",
                           "m_inContainerName": jets,
                           "m_outContainerName": jets_calib,
                           "m_jetAlgo": "AntiKt4EMTopo",
                           "m_sort": True,
                           "m_calibSequence": "JetArea_Residual_Origin_EtaJES_GSC",
                           "m_calibConfigAFII": "JES_MC15Prerecommendation_AFII_June2015.config",
                           "m_calibConfigFullSim": "JES_MC15Prerecommendation_April2015.config",
                           "m_calibConfigData": "JES_MC15Prerecommendation_April2015.config",
                           "m_JESUncertConfig": "$ROOTCOREBIN/data/JetUncertainties/JES_2015/Prerec/PrerecJES2015_AllNuisanceParameters_25ns.config",
                           "m_JESUncertMCType": "MC15",
                           "m_JERUncertConfig": "JetResolution/Prerec2015_xCalib_2012JER_ReducedTo9NP_Plots_v2.root",
                           "m_JERApplyNominal": False,
                           "m_JERFullSys": False,
                           "m_jetCleanCutLevel": "LooseBad",
                           "m_jetCleanUgly": False,
                           "m_cleanParent": False,
                           "m_saveAllCleanDecisions": True,
                           "m_redoJVT": True})

''' Select the Electrons and (clean) Jets passing a baseline selection (require at least two) '''
c.setalg("ElectronSelector", {"m_name": "BaselineElectrons",
                              "m_inContainerName": electrons_calib,
                              "m_decorateSelectedObjects": False,
                              "m_createSelectedContainer": True,
                              "m_outContainerName": electrons_select,
                              "m_debug":False,
                              "m_pass_min": 2})

c.setalg("JetSelector", {"m_name": "BaselineJets",
                              "m_inContainerName": jets_calib,
                              "m_decorateSelectedObjects": False,
                              "m_createSelectedContainer": True,
                              "m_outContainerName": jets_select,
                              "m_pass_min": 2,
                              "m_debug":False,
                              "m_cleanJets": True})

c.setalg("MyAnalysisAlgo",  {"m_name" : "MyAnalysisExample",
                            "m_debug"     : False,
                            "m_inContainerName_Electrons" : electrons_select,
                            "m_inContainerName_Jets"   : jets_select})


''' Start dumping information of what we have so far... '''
c.setalg("MyAnalysisTreeAlgo", {"m_debug": False,
                                "m_name": "MyAnalysisTreeExample",
                                "m_evtDetailStr"       : "electronSF",
                                "m_jetContainerName"      : jets_select,
                                "m_jetDetailStr" : "kinematic",
                                "m_elContainerName": electrons_select,
                                "m_elDetailStr": "kinematic"})
