/*************************************************************************
*************************************************************************/

#ifndef FREEENERGY_H
#define FREEENERGY_H

#include "Material.h"

//forward declaration
class FreeEnergy;

template<>
InputParameters validParams<FreeEnergy>();

class FreeEnergy : public Material
{
public:
  FreeEnergy(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  Real _length_scale;
  Real _energy_scale;

  const MaterialProperty<Real> & _kappa_c;      //CH gradient energy coefficient (isotropic)
  const MaterialProperty<Real> & _kappa_n;      //AC gradient energy coefficient (isotropic)

  const MaterialProperty<Real> & _W;            //well height
  const MaterialProperty<Real> & _molar_vol;    //molar volume

  const MaterialProperty<Real> & _alpha_energy;
  const MaterialProperty<Real> & _delta_energy;

  //COUPLED VARIABLES
  const VariableValue & _c;   //coupled concentration
  const VariableValue & _n;   //coupled order parameter
  const VariableGradient & _grad_c;
  const VariableGradient & _grad_n;

  MaterialProperty<Real> & _free_energy_density;  //free energy functional energy density

  MaterialProperty<Real> & _alpha_energy_density; //free energy density of only the alpha phase
  MaterialProperty<Real> & _delta_energy_density; //free energy density of only the delta phase
};

#endif //FREEENERGY_H
