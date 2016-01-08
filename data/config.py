from xAH_config import xAH_config
c = xAH_config()

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

# add in calibrations for electrons and jets here

# end add in calibrations for electrons and jets here

c.setalg("ElectronSelector", {"m_name": "BaselineElectrons",
                              "m_inContainerName": "Electrons",
                              "m_decorateSelectedObjects": False,
                              "m_createSelectedContainer": True,
                              "m_outContainerName": "ElectronsBaseline",
                              "m_pass_min": 2})

c.setalg("JetSelector", {"m_name": "BaselineJets",
                              "m_inContainerName": "AntiKt4EMTopoJets",
                              "m_decorateSelectedObjects": False,
                              "m_createSelectedContainer": True,
                              "m_outContainerName": "AntiKt4EMTopoJetsBaseline",
                              "m_pass_min": 2,
                              "m_cleanJets": False})

