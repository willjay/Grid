#include <Hadrons/Modules/MContraction/WeakEye3pt.hpp>
#include <Hadrons/Modules/MContraction/Baryon.hpp>
#include <Hadrons/Modules/MContraction/A2AAslashField.hpp>
#include <Hadrons/Modules/MContraction/A2ALoop.hpp>
#include <Hadrons/Modules/MContraction/A2AMesonField.hpp>
#include <Hadrons/Modules/MContraction/StagA2AMesonFieldCC.hpp>
#include <Hadrons/Modules/MContraction/Meson.hpp>
#include <Hadrons/Modules/MContraction/MesonCC.hpp>
#include <Hadrons/Modules/MContraction/MesonCCLoop.hpp>
#include <Hadrons/Modules/MContraction/DiscLoop.hpp>
#include <Hadrons/Modules/MContraction/Gamma3pt.hpp>
#include <Hadrons/Modules/MContraction/WeakMesonDecayKl2.hpp>
#include <Hadrons/Modules/MContraction/StagA2AMesonField.hpp>
#include <Hadrons/Modules/MContraction/WeakNonEye3pt.hpp>
#include <Hadrons/Modules/MFermion/FreeProp.hpp>
#include <Hadrons/Modules/MFermion/GaugeProp.hpp>
#include <Hadrons/Modules/MFermion/EMLepton.hpp>
#include <Hadrons/Modules/MFermion/GaugeSpectProp.hpp>
#include <Hadrons/Modules/MSource/SeqGamma.hpp>
#include <Hadrons/Modules/MSource/Convolution.hpp>
#include <Hadrons/Modules/MSource/Point.hpp>
#include <Hadrons/Modules/MSource/Wall.hpp>
#include <Hadrons/Modules/MSource/SeqAslash.hpp>
#include <Hadrons/Modules/MSource/Z2.hpp>
#include <Hadrons/Modules/MSource/SeqConserved.hpp>
#include <Hadrons/Modules/MSource/Momentum.hpp>
#include <Hadrons/Modules/MSource/Gauss.hpp>
#include <Hadrons/Modules/MSink/Smear.hpp>
#include <Hadrons/Modules/MSink/Point.hpp>
#include <Hadrons/Modules/MSolver/MixedPrecisionRBPrecCG.hpp>
#include <Hadrons/Modules/MSolver/LocalCoherenceLanczos.hpp>
#include <Hadrons/Modules/MSolver/A2AAslashVectors.hpp>
#include <Hadrons/Modules/MSolver/Guesser.hpp>
#include <Hadrons/Modules/MSolver/RBPrecCG.hpp>
#include <Hadrons/Modules/MSolver/A2AVectors.hpp>
#include <Hadrons/Modules/MGauge/UnitEm.hpp>
#include <Hadrons/Modules/MGauge/StoutSmearing.hpp>
#include <Hadrons/Modules/MGauge/Unit.hpp>
#include <Hadrons/Modules/MGauge/Random.hpp>
#include <Hadrons/Modules/MGauge/GaugeFix.hpp>
#include <Hadrons/Modules/MGauge/FundtoHirep.hpp>
#include <Hadrons/Modules/MGauge/StochEm.hpp>
#include <Hadrons/Modules/MGauge/ConstEField.hpp>
#include <Hadrons/Modules/MGauge/Electrify.hpp>
#include <Hadrons/Modules/MNoise/TimeDilutedSpinColorDiagonal.hpp>
#include <Hadrons/Modules/MNoise/TimeDilutedColorDiagonal.hpp>
#include <Hadrons/Modules/MNoise/FullVolumeColorDiagonal.hpp>
#include <Hadrons/Modules/MNoise/FullVolumeSpinColorDiagonal.hpp>
#include <Hadrons/Modules/MUtilities/PrecisionCast.hpp>
#include <Hadrons/Modules/MUtilities/RandomVectors.hpp>
#include <Hadrons/Modules/MScalar/FreeProp.hpp>
#include <Hadrons/Modules/MScalar/Scalar.hpp>
#include <Hadrons/Modules/MScalar/ChargedProp.hpp>
#include <Hadrons/Modules/MNPR/Bilinear.hpp>
#include <Hadrons/Modules/MNPR/Amputate.hpp>
#include <Hadrons/Modules/MNPR/FourQuark.hpp>
#include <Hadrons/Modules/MAction/DWF.hpp>
#include <Hadrons/Modules/MAction/Staggered.hpp>
#include <Hadrons/Modules/MAction/MobiusDWF.hpp>
#include <Hadrons/Modules/MAction/Wilson.hpp>
#include <Hadrons/Modules/MAction/WilsonClover.hpp>
#include <Hadrons/Modules/MAction/ZMobiusDWF.hpp>
#include <Hadrons/Modules/MAction/ImprovedStaggered.hpp>
#include <Hadrons/Modules/MAction/ScaledDWF.hpp>
#include <Hadrons/Modules/MScalarSUN/StochFreeField.hpp>
#include <Hadrons/Modules/MScalarSUN/TwoPointNPR.hpp>
#include <Hadrons/Modules/MScalarSUN/Div.hpp>
#include <Hadrons/Modules/MScalarSUN/TrMag.hpp>
#include <Hadrons/Modules/MScalarSUN/EMT.hpp>
#include <Hadrons/Modules/MScalarSUN/TwoPoint.hpp>
#include <Hadrons/Modules/MScalarSUN/TrPhi.hpp>
#include <Hadrons/Modules/MScalarSUN/Utils.hpp>
#include <Hadrons/Modules/MScalarSUN/TransProj.hpp>
#include <Hadrons/Modules/MScalarSUN/Grad.hpp>
#include <Hadrons/Modules/MScalarSUN/TrKinetic.hpp>
#include <Hadrons/Modules/MIO/LoadEigenPack.hpp>
#include <Hadrons/Modules/MIO/LoadNersc.hpp>
#include <Hadrons/Modules/MIO/LoadA2AVectors.hpp>
#include <Hadrons/Modules/MIO/LoadCosmHol.hpp>
#include <Hadrons/Modules/MIO/LoadIldg.hpp>
#include <Hadrons/Modules/MIO/LoadCoarseEigenPack.hpp>
#include <Hadrons/Modules/MIO/LoadBinary.hpp>
