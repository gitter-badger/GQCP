target_sources(gqcp
    PRIVATE
        DOCIRDMBuilder.cpp
        FCIRDMBuilder.cpp
        # FrozenCoreDOCIRDMBuilder.cpp
        FrozenCoreFCIRDMBuilder.cpp
        FrozenCoreRDMBuilder.cpp
        RDMCalculator.cpp
        SelectedRDMBuilder.cpp
        SpinUnresolvedFCIRDMBuilder.cpp
        SpinUnresolvedRDMCalculator.cpp
)
