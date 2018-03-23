#!/bin/bash
# run this script with one or more arguments that are the directories that hold the code files for
# the objects you want to update.

dirs=$@

appname='HyraxApp'

objs='
ACTransformElasticDF
ACCoupledCalphad
CHCoupledCalphadSplit
CHPrecipMatrixElasticity
AuxDeltaGStar
AuxBulkEnergyCalphad
AuxGradientEnergy
AuxElasticEnergy
AuxCalphadElasticity
AuxElasticInteractionEnergy
AuxAMRNucleationProbability
AuxVolumetricNucleationRate
AuxDFchemDC
AuxDFelDC
AuxGrandPotential
StressBC
SimpleSplitCHFluxBC
CalphadEnergyMaterial
CalphadAB1CD1Material
CalphadAB1CD2Material
ZrHCalphadDiffusivity
TwoPhaseLinearElasticMaterial
PolySpecifiedSmoothCircleIC
EllipsoidIC
SmoothBoxIC
DiamondIC
DepletionRegionIC
MeshSolutionModify
NucleiInformation
NucleationLocationUserObject
NucleusIntroductionSolutionModifier
OneNucleusUserObject
NucleationMarker
'

actions='
'

for obj in $objs; do
    find $dirs | grep "/${obj}\.C$" | xargs perl -0777 -i'' -pe 's/(template ?<>\nInputParameters\n? ?validParams<(\w*)>\(\))/registerMooseObject\("'"$appname"'", \2\);\n\n\1/igs'
done

while read action task; do
    find $dirs | grep "/${action}\.C$" | xargs perl -0777 -i'' -pe 's/(template ?<>\nInputParameters\n? ?validParams<(\w*)>\(\))/registerMooseAction\("'"$appname"'", \2, "'"$task"'"\);\n\n\1/igs'
done <<< "$actions"
