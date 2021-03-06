// This file is part of GQCG-gqcp.
// 
// Copyright (C) 2017-2019  the GQCG developers
// 
// GQCG-gqcp is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// GQCG-gqcp is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with GQCG-gqcp.  If not, see <http://www.gnu.org/licenses/>.
// 
#pragma once


#include "Basis/Integrals/Interfaces/LibcintInterfacer.hpp"
#include "Basis/Integrals/Interfaces/LibcintOneElectronIntegralBuffer.hpp"
#include "Basis/Integrals/Interfaces/LibcintOneElectronIntegralEngine.hpp"
#include "Basis/Integrals/Interfaces/LibcintTwoElectronIntegralBuffer.hpp"
#include "Basis/Integrals/Interfaces/LibcintTwoElectronIntegralEngine.hpp"
#include "Basis/Integrals/Interfaces/LibintInterfacer.hpp"
#include "Basis/Integrals/Interfaces/LibintOneElectronIntegralBuffer.hpp"
#include "Basis/Integrals/Interfaces/LibintOneElectronIntegralEngine.hpp"
#include "Basis/Integrals/Interfaces/LibintTwoElectronIntegralBuffer.hpp"
#include "Basis/Integrals/Interfaces/LibintTwoElectronIntegralEngine.hpp"

#include "Basis/Integrals/BaseOneElectronIntegralBuffer.hpp"
#include "Basis/Integrals/BaseOneElectronIntegralEngine.hpp"
#include "Basis/Integrals/BaseTwoElectronIntegralBuffer.hpp"
#include "Basis/Integrals/BaseTwoElectronIntegralEngine.hpp"
#include "Basis/Integrals/IntegralCalculator.hpp"
#include "Basis/Integrals/IntegralEngine.hpp"

#include "Basis/ScalarBasis/CartesianExponents.hpp"
#include "Basis/ScalarBasis/CartesianGTO.hpp"
#include "Basis/ScalarBasis/GTOBasisSet.hpp"
#include "Basis/ScalarBasis/GTOShell.hpp"
#include "Basis/ScalarBasis/ShellSet.hpp"
#include "Basis/ScalarBasis/ScalarBasis.hpp"

#include "Basis/SpinorBasis/GSpinorBasis.hpp"
#include "Basis/SpinorBasis/JacobiRotationParameters.hpp"
#include "Basis/SpinorBasis/OrbitalRotationGenerators.hpp"
#include "Basis/SpinorBasis/RSpinorBasis.hpp"
#include "Basis/SpinorBasis/SimpleSpinorBasis.hpp"
#include "Basis/SpinorBasis/SpinComponent.hpp"
#include "Basis/SpinorBasis/USpinorBasis.hpp"

#include "Basis/transform.hpp"
#include "Basis/TransformationMatrix.hpp"

#include "Mathematical/Algorithm/Algorithm.hpp"
#include "Mathematical/Algorithm/ConvergenceCriterion.hpp"
#include "Mathematical/Algorithm/IterativeAlgorithm.hpp"
#include "Mathematical/Algorithm/Step.hpp"
#include "Mathematical/Algorithm/StepCollection.hpp"

#include "Mathematical/Optimization/Accelerator/ConstantDamper.hpp"
#include "Mathematical/Optimization/Accelerator/DIIS.hpp"

#include "Mathematical/Optimization/Eigenproblem/Davidson/CorrectionVectorCalculation.hpp"
#include "Mathematical/Optimization/Eigenproblem/Davidson/DavidsonSolver.hpp"
#include "Mathematical/Optimization/Eigenproblem/Davidson/GuessVectorUpdate.hpp"
#include "Mathematical/Optimization/Eigenproblem/Davidson/MatrixVectorProductCalculation.hpp"
#include "Mathematical/Optimization/Eigenproblem/Davidson/ResidualVectorCalculation.hpp"
#include "Mathematical/Optimization/Eigenproblem/Davidson/ResidualVectorConvergence.hpp"
#include "Mathematical/Optimization/Eigenproblem/Davidson/SubspaceMatrixCalculation.hpp"
#include "Mathematical/Optimization/Eigenproblem/Davidson/SubspaceMatrixDiagonalization.hpp"
#include "Mathematical/Optimization/Eigenproblem/Davidson/SubspaceUpdate.hpp"

#include "Mathematical/Optimization/Eigenproblem/DenseDiagonalization.hpp"
#include "Mathematical/Optimization/Eigenproblem/Eigenpair.hpp"
#include "Mathematical/Optimization/Eigenproblem/EigenproblemEnvironment.hpp"
#include "Mathematical/Optimization/Eigenproblem/EigenproblemSolver.hpp"

#include "Mathematical/Optimization/Minimization/BaseHessianModifier.hpp"
#include "Mathematical/Optimization/Minimization/IterativeIdentitiesHessianModifier.hpp"
#include "Mathematical/Optimization/Minimization/UnalteringHessianModifier.hpp"

#include "Mathematical/Optimization/NonLinearEquation/NewtonStepUpdate.hpp"
#include "Mathematical/Optimization/NonLinearEquation/NonLinearEquationEnvironment.hpp"
#include "Mathematical/Optimization/NonLinearEquation/NonLinearEquationSolver.hpp"

#include "Mathematical/Optimization/ConsecutiveIteratesNormConvergence.hpp"
#include "Mathematical/Optimization/OptimizationEnvironment.hpp"

#include "Mathematical/Representation/BlockMatrix.hpp"
#include "Mathematical/Representation/BlockRankFourTensor.hpp"
#include "Mathematical/Representation/Matrix.hpp"
#include "Mathematical/Representation/QCMatrix.hpp"
#include "Mathematical/Representation/QCRankFourTensor.hpp"
#include "Mathematical/Representation/SquareMatrix.hpp"
#include "Mathematical/Representation/SquareRankFourTensor.hpp"
#include "Mathematical/Representation/Tensor.hpp"

#include "Mathematical/CartesianDirection.hpp"
#include "Mathematical/LinearCombination.hpp"
#include "Mathematical/ScalarFunction.hpp"

#include "Molecule/elements.hpp"
#include "Molecule/Molecule.hpp"
#include "Molecule/NuclearFramework.hpp"
#include "Molecule/Nucleus.hpp"

#include "QCModel/CI/LinearExpansion.hpp"

#include "ONVBasis/BaseFrozenCoreONVBasis.hpp"
#include "ONVBasis/BaseONVBasis.hpp"
#include "ONVBasis/EvaluationIterator.hpp"
#include "ONVBasis/ONVBasisType.hpp"
#include "ONVBasis/ONVManipulator.hpp"
#include "ONVBasis/SeniorityZeroONVBasis.hpp"
#include "ONVBasis/SpinResolvedFrozenONVBasis.hpp"
#include "ONVBasis/SpinResolvedONV.hpp"
#include "ONVBasis/SpinResolvedONVBasis.hpp"
#include "ONVBasis/SpinResolvedSelectedONVBasis.hpp"
#include "ONVBasis/SpinUnresolvedFrozenONVBasis.hpp"
#include "ONVBasis/SpinUnresolvedONV.hpp"
#include "ONVBasis/SpinUnresolvedONVBasis.hpp"

#include "Operator/FirstQuantized/BaseFQOneElectronOperator.hpp"
#include "Operator/FirstQuantized/BaseFQTwoElectronOperator.hpp"
#include "Operator/FirstQuantized/BaseMultipoleOperator.hpp"
#include "Operator/FirstQuantized/BaseNuclearOperator.hpp"
#include "Operator/FirstQuantized/CoulombRepulsionOperator.hpp"
#include "Operator/FirstQuantized/ElectronicDipoleOperator.hpp"
#include "Operator/FirstQuantized/KineticOperator.hpp"
#include "Operator/FirstQuantized/NuclearAttractionOperator.hpp"
#include "Operator/FirstQuantized/NuclearDipoleOperator.hpp"
#include "Operator/FirstQuantized/Operator.hpp"
#include "Operator/FirstQuantized/OverlapOperator.hpp"

#include "Operator/SecondQuantized/ModelHamiltonian/HoppingMatrix.hpp"
#include "Operator/SecondQuantized/ModelHamiltonian/HubbardHamiltonian.hpp"

#include "Operator/SecondQuantized/SQHamiltonian.hpp"
#include "Operator/SecondQuantized/SQOneElectronOperator.hpp"
#include "Operator/SecondQuantized/SQTwoElectronOperator.hpp"
#include "Operator/SecondQuantized/USQHamiltonian.hpp"

#include "Processing/Properties/BaseElectricalResponseSolver.hpp"
#include "Processing/Properties/expectation_values.hpp"
#include "Processing/Properties/properties.hpp"
#include "Processing/Properties/RHFElectricalResponseSolver.hpp"

#include "Processing/RDM/BaseRDMBuilder.hpp"
#include "Processing/RDM/BaseSpinUnresolvedRDMBuilder.hpp"
#include "Processing/RDM/DOCIRDMBuilder.hpp"
#include "Processing/RDM/FCIRDMBuilder.hpp"
// #include "Processing/RDM/FrozenCoreDOCIRDMBuilder.hpp"
#include "Processing/RDM/FrozenCoreFCIRDMBuilder.hpp"
#include "Processing/RDM/FrozenCoreRDMBuilder.hpp"
#include "Processing/RDM/OneRDM.hpp"
// #include "Processing/RDM/RDMCalculator.hpp"
#include "Processing/RDM/RDMs.hpp"
#include "Processing/RDM/SelectedRDMBuilder.hpp"
#include "Processing/RDM/SpinUnresolvedFCIRDMBuilder.hpp"
#include "Processing/RDM/SpinUnresolvedRDMCalculator.hpp"
#include "Processing/RDM/TwoRDM.hpp"

#include "QCMethod/Applications/AtomicDecompositionParameters.hpp"

#include "QCMethod/CI/HamiltonianBuilder/DOCI.hpp"
#include "QCMethod/CI/HamiltonianBuilder/FCI.hpp"
#include "QCMethod/CI/HamiltonianBuilder/FrozenCoreCI.hpp"
// #include "QCMethod/CI/HamiltonianBuilder/FrozenCoreDOCI.hpp"
#include "QCMethod/CI/HamiltonianBuilder/FrozenCoreFCI.hpp"
#include "QCMethod/CI/HamiltonianBuilder/HamiltonianBuilder.hpp"
#include "QCMethod/CI/HamiltonianBuilder/Hubbard.hpp"
#include "QCMethod/CI/HamiltonianBuilder/SelectedCI.hpp"

// #include "QCMethod/CI/DOCINewtonOrbitalOptimizer.hpp"

#include "QCMethod/Geminals/AP1roGJacobiOrbitalOptimizer.hpp"
#include "QCMethod/Geminals/AP1roGLagrangianNewtonOrbitalOptimizer.hpp"

#include "QCMethod/HF/DiagonalRHFFockMatrixObjective.hpp"
#include "QCMethod/HF/RHF.hpp"
#include "QCMethod/HF/RHFDensityMatrixCalculation.hpp"
#include "QCMethod/HF/RHFDensityMatrixDamper.hpp"
#include "QCMethod/HF/RHFElectronicEnergyCalculation.hpp"
#include "QCMethod/HF/RHFErrorCalculation.hpp"
#include "QCMethod/HF/RHFFockMatrixCalculation.hpp"
#include "QCMethod/HF/RHFFockMatrixDiagonalization.hpp"
#include "QCMethod/HF/RHFFockMatrixDIIS.hpp"
#include "QCMethod/HF/RHFSCFEnvironment.hpp"
#include "QCMethod/HF/RHFSCFSolver.hpp"

#include "QCMethod/OrbitalOptimization/Localization/ERJacobiLocalizer.hpp"
#include "QCMethod/OrbitalOptimization/Localization/ERNewtonLocalizer.hpp"

#include "QCMethod/OrbitalOptimization/BaseOrbitalOptimizer.hpp"
#include "QCMethod/OrbitalOptimization/JacobiOrbitalOptimizer.hpp"
#include "QCMethod/OrbitalOptimization/NewtonOrbitalOptimizer.hpp"
#include "QCMethod/OrbitalOptimization/QCMethodNewtonOrbitalOptimizer.hpp"

#include "QCMethod/RMP2/RMP2.hpp"

#include "QCMethod/QCObjective.hpp"
#include "QCMethod/QCStructure.hpp"

#include "QCModel/CI/LinearExpansion.hpp"

#include "QCModel/Geminals/AP1roG.hpp"
#include "QCModel/Geminals/AP1roGGeminalCoefficients.hpp"
#include "QCModel/Geminals/APIGGeminalCoefficients.hpp"
#include "QCModel/Geminals/GeminalCoefficientsInterface.hpp"

#include "QCModel/HF/RHF.hpp"

#include "Utilities/CRTP.hpp"
#include "Utilities/linalg.hpp"
#include "Utilities/memory.hpp"
#include "Utilities/miscellaneous.hpp"
#include "Utilities/type_traits.hpp"
#include "Utilities/typedefs.hpp"
#include "Utilities/units.hpp"

#include "version.hpp"
