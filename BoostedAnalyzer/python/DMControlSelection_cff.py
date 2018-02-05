import FWCore.ParameterSet.Config as cms


# SingleMuon= cms.PSet(
#     max_mt=cms.double(160),
# )
# DMControlSelection = cms.PSet(
#     channel=cms.string("channel"),
# )

# DMControlSelection_DoubleMuon = cms.PSet(
# channel=cms.string("DoubleMuon"),
# )
# DMSignalSelection = cms.PSet(
# channel=cms.string("signal"),
# )

DMControlSelection = cms.PSet(
    channel=cms.string("channel"),

    SingleMuon=cms.PSet(
        max_mt=cms.double(150),
    ),
    SingleElectron=cms.PSet(
        min_met=cms.double(50),
    ),   
    DoubleMuon=cms.PSet(
    	min_pt=cms.double(20),
    	min_mass_mumu=cms.double(60),
    	max_mass_mumu=cms.double(120),

    	),

)
