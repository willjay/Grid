/*************************************************************************************

Grid physics library, www.github.com/paboyle/Grid 

Source file: Hadrons/Modules/MContraction/StagA2AMesonFieldCC.hpp

Copyright (C) 2015-2019

Author: Antonin Portelli <antonin.portelli@me.com>
Author: Peter Boyle <paboyle@ph.ed.ac.uk>
Author: paboyle <paboyle@ph.ed.ac.uk>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

See the full license in the file "LICENSE" in the top level distribution directory
*************************************************************************************/
/*  END LEGAL */
#ifndef Hadrons_MContraction_StagA2AMesonFieldCC_hpp_
#define Hadrons_MContraction_StagA2AMesonFieldCC_hpp_

#include <Hadrons/Global.hpp>
#include <Hadrons/Module.hpp>
#include <Hadrons/ModuleFactory.hpp>
#include <Hadrons/A2AMatrix.hpp>

BEGIN_HADRONS_NAMESPACE

/******************************************************************************
 *                     All-to-all meson field creation                        *
 ******************************************************************************/
BEGIN_MODULE_NAMESPACE(MContraction)

class StagA2AMesonFieldCCPar: Serializable
{
public:
    GRID_SERIALIZABLE_CLASS_MEMBERS(StagA2AMesonFieldCCPar,
                                    int, cacheBlock,
                                    int, block,
                                    std::string, gauge,
                                    std::string, left,
                                    std::string, right,
                                    std::string, output,
                                    std::string, gammas,
                                    std::vector<std::string>, mom);
};

class StagA2AMesonFieldCCMetadata: Serializable
{
public:
    GRID_SERIALIZABLE_CLASS_MEMBERS(StagA2AMesonFieldCCMetadata,
                                    std::vector<RealF>, momentum,
                                    Gamma::Algebra, gamma);
};

template <typename T, typename FImpl>
class StagMesonFieldCCKernel: public A2AKernel<T, typename FImpl::FermionField>
{
public:
    typedef typename FImpl::FermionField FermionField;
public:
    StagMesonFieldCCKernel(const LatticeGaugeField &U,
                           const std::vector<Gamma::Algebra> &gamma,
                           const std::vector<LatticeComplex> &mom,
                           GridBase *grid)
    : U_(U), gamma_(gamma), mom_(mom), grid_(grid)
    {
        vol_ = 1.;
        for (auto &d: grid_->GlobalDimensions())
        {
            vol_ *= d;
        }
    }

    virtual ~StagMesonFieldCCKernel(void) = default;
    
    void operator()(A2AMatrixSet<T> &m,
                            const FermionField *left,
                            const FermionField *right,
                            const unsigned int orthogDim,
                            double &t)
    {
        A2Autils<FImpl>::StagMesonFieldCC(m, U_, left, right, gamma_, mom_, orthogDim, &t);
    }
    
    virtual double flops(const unsigned int blockSizei, const unsigned int blockSizej)
    {
        // needs to be updated for staggered
        return vol_*(2*8.0+6.0+8.0*mom_.size())*blockSizei*blockSizej*gamma_.size();
    }

    virtual double bytes(const unsigned int blockSizei, const unsigned int blockSizej)
    {
        // 3.0 ? for colors
        return vol_*(3.0*sizeof(T))*blockSizei*blockSizej
               +  vol_*(2.0*sizeof(T)*mom_.size())*blockSizei*blockSizej*gamma_.size();
    }
private:
    const LatticeGaugeField &U_;
    const std::vector<Gamma::Algebra> &gamma_;
    const std::vector<LatticeComplex> &mom_;
    GridBase                          *grid_;
    double                            vol_;
};

template <typename FImpl>
class TStagA2AMesonFieldCC : public Module<StagA2AMesonFieldCCPar>
{
public:
    FERM_TYPE_ALIASES(FImpl,);
    typedef A2AMatrixBlockComputation<Complex, 
                                      FermionField, 
                                      StagA2AMesonFieldCCMetadata,
                                      HADRONS_A2AM_IO_TYPE> Computation;
    typedef StagMesonFieldCCKernel<Complex, FImpl> Kernel;
public:
    // constructor
    TStagA2AMesonFieldCC(const std::string name);
    // destructor
    virtual ~TStagA2AMesonFieldCC(void){};
    // dependency relation
    virtual std::vector<std::string> getInput(void);
    virtual std::vector<std::string> getOutput(void);
    // setup
    virtual void setup(void);
    // execution
    virtual void execute(void);
private:
    bool                               hasPhase_{false};
    std::string                        momphName_;
    std::vector<Gamma::Algebra>        gamma_;
    std::vector<std::vector<Real>>     mom_;
    //LatticeGaugeField                  U_;
};

MODULE_REGISTER(StagA2AMesonFieldCC, ARG(TStagA2AMesonFieldCC<STAGIMPL>), MContraction);

/******************************************************************************
*                  TStagA2AMesonFieldCC implementation                             *
******************************************************************************/
// constructor /////////////////////////////////////////////////////////////////
template <typename FImpl>
TStagA2AMesonFieldCC<FImpl>::TStagA2AMesonFieldCC(const std::string name)
: Module<StagA2AMesonFieldCCPar>(name)
, momphName_(name + "_momph")
{
}

// dependencies/products ///////////////////////////////////////////////////////
template <typename FImpl>
std::vector<std::string> TStagA2AMesonFieldCC<FImpl>::getInput(void)
{
    std::vector<std::string> in = {par().gauge, par().left, par().right};

    return in;
}

template <typename FImpl>
std::vector<std::string> TStagA2AMesonFieldCC<FImpl>::getOutput(void)
{
    std::vector<std::string> out = {};

    return out;
}

// setup ///////////////////////////////////////////////////////////////////////
template <typename FImpl>
void TStagA2AMesonFieldCC<FImpl>::setup(void)
{
    gamma_.clear();
    mom_.clear();
    if (par().gammas == "all")
    {
        gamma_ = {
            Gamma::Algebra::Gamma5,
            Gamma::Algebra::Identity,    
            Gamma::Algebra::GammaX,
            Gamma::Algebra::GammaY,
            Gamma::Algebra::GammaZ,
            Gamma::Algebra::GammaT,
            Gamma::Algebra::GammaXGamma5,
            Gamma::Algebra::GammaYGamma5,
            Gamma::Algebra::GammaZGamma5,
            Gamma::Algebra::GammaTGamma5,
            Gamma::Algebra::SigmaXY,
            Gamma::Algebra::SigmaXZ,
            Gamma::Algebra::SigmaXT,
            Gamma::Algebra::SigmaYZ,
            Gamma::Algebra::SigmaYT,
            Gamma::Algebra::SigmaZT
        };
    }
    else
    {
        gamma_ = strToVec<Gamma::Algebra>(par().gammas);
    }
    for (auto &pstr: par().mom)
    {
        auto p = strToVec<Real>(pstr);

        if (p.size() != env().getNd() - 1)
        {
            HADRONS_ERROR(Size, "Momentum has " + std::to_string(p.size())
                                + " components instead of " 
                                + std::to_string(env().getNd() - 1));
        }
        mom_.push_back(p);
    }
    envCache(std::vector<ComplexField>, momphName_, 1, 
             par().mom.size(), envGetGrid(ComplexField));
    envTmpLat(ComplexField, "coor");
    envTmp(Computation, "computation", 1, envGetGrid(FermionField), 
           env().getNd() - 1, mom_.size(), gamma_.size(), par().block, 
           par().cacheBlock, this);
}

// execution ///////////////////////////////////////////////////////////////////
template <typename FImpl>
void TStagA2AMesonFieldCC<FImpl>::execute(void)
{
    auto &left  = envGet(std::vector<FermionField>, par().left);
    auto &right = envGet(std::vector<FermionField>, par().right);
    auto &temp = envGet(std::vector<FermionField>, par().right);
    auto &U = envGet(LatticeGaugeField, par().gauge);
    
    int nt         = env().getDim().back();
    int N_i        = left.size();
    int N_j        = right.size();
    int ngamma     = gamma_.size();
    assert(ngamma==1);// do one at a time
    int nmom       = mom_.size();
    int block      = par().block;
    int cacheBlock = par().cacheBlock;

    LOG(Message) << "Computing all-to-all meson fields" << std::endl;
    LOG(Message) << "Left: '" << par().left << "' Right: '" << par().right << "'" << std::endl;
    LOG(Message) << "Momenta:" << std::endl;
    for (auto &p: mom_)
    {
        LOG(Message) << "  " << p << std::endl;
    }
    LOG(Message) << "Spin bilinears:" << std::endl;
    for (auto &g: gamma_)
    {
        LOG(Message) << "  " << g << std::endl;
    }
    LOG(Message) << "Meson field size: " << nt << "*" << N_i << "*" << N_j 
                 << " (filesize " << sizeString(nt*N_i*N_j*sizeof(HADRONS_A2AM_IO_TYPE)) 
                 << "/momentum/bilinear)" << std::endl;

    auto &ph = envGet(std::vector<ComplexField>, momphName_);

    if (!hasPhase_)
    {
        startTimer("Momentum phases");
        for (unsigned int j = 0; j < nmom; ++j)
        {
            Complex           i(0.0,1.0);
            std::vector<Real> p;

            envGetTmp(ComplexField, coor);
            //ph[j] = zero;
            ph[j] = Zero();
            for(unsigned int mu = 0; mu < mom_[j].size(); mu++)
            {
                LatticeCoordinate(coor, mu);
                ph[j] = ph[j] + (mom_[j][mu]/env().getDim(mu))*coor;
            }
            ph[j] = exp((Real)(2*M_PI)*i*ph[j]);
        }
        hasPhase_ = true;
        stopTimer("Momentum phases");
    }

    auto ionameFn = [this](const unsigned int m, const unsigned int g)
    {
        std::stringstream ss;

        ss << gamma_[g] << "_";
        for (unsigned int mu = 0; mu < mom_[m].size(); ++mu)
        {
            ss << mom_[m][mu] << ((mu == mom_[m].size() - 1) ? "" : "_");
        }

        return ss.str();
    };

    auto filenameFn = [this, &ionameFn](const unsigned int m, const unsigned int g)
    {
        return par().output + "." + std::to_string(vm().getTrajectory()) 
               + "/" + ionameFn(m, g) + ".h5";
    };

    auto metadataFn = [this](const unsigned int m, const unsigned int g)
    {
        StagA2AMesonFieldCCMetadata md;

        for (auto pmu: mom_[m])
        {
            md.momentum.push_back(pmu);
        }
        md.gamma = gamma_[g];
        
        return md;
    };

    // U_mu(x) right(x+mu)
    std::vector<LatticeColourMatrix> Umu(4,U.Grid());
    for(int mu=0;mu<Nd;mu++){
        Umu[mu] = PeekIndex<LorentzIndex>(U,mu);
    }
    //int num_vec = right.size();
    //std::vector<FermionField> temp(num_vec, right[0].Grid());
    // spatial gamma's only
    
    int mu;
    if(gamma_[0]==GammaX)mu=0;
    else if(gamma_[0]==GammaY)mu=1;
    else if(gammas_[0]==GammaZ)mu=2;
    else assert(0);
    
    for(int j=0;j<num_vec;j++)
        temp[j] = Umu[mu]*Cshift(right[j], mu, 1);
    
    Kernel      kernel(U, gamma_, ph, envGetGrid(FermionField));

    envGetTmp(Computation, computation);
    computation.execute(left, temp, kernel, ionameFn, filenameFn, metadataFn);

}



END_MODULE_NAMESPACE

END_HADRONS_NAMESPACE

#endif // Hadrons_MContraction_StagA2AMesonFieldCC_hpp_