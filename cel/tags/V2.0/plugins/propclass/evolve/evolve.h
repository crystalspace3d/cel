/*
    Crystal Space Entity Layer
    Copyright (C) 2007 by Jorrit Tyberghein

    Genetic Algorithm Property Class
    Copyright (C) 2007 by Mat Sutcliffe

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CEL_PF_EVOLVEFACT__
#define __CEL_PF_EVOLVEFACT__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/refcount.h"
#include "iutil/comp.h"

#include "propclass/evolve.h"
#include "propclass/neuralnet.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"

struct iObjectRegistry;

CEL_DECLARE_FACTORY(Evolve)

/**
 * An interface representing a single genome in the population of celPcEvolve.
 *
 * Its methods are all specific to the type of genome in use (i.e. the type of
 * property class being evolved).
 *
 * This makes it easier to add support for other types of property class in
 * future, since celPcEvolve itself contains no code which is specific to the
 * type of property class. All such specific code is delegated to the
 * iCelGenome implementation.
 *
 * Currently there is only one implementation, and it supports evolving
 * iPcNeuralNet.
 *
 * An implementation's constructor must take a single parameter, a pointer to
 * the subject iCelPropertyClass. It must also define a static method called
 * Supports. See celNeuralNetGenome for an example. To add an implementation
 * to celPcEvolve, see the code for celPcEvolve::Generate().
 */
struct iCelGenome : public virtual iBase
{
  SCF_INTERFACE(iCelGenome, 0, 0, 1);

  /// Copies the genome from the subject propclass into this genome object.
  virtual void Retrieve() = 0;

  /// Copies the genome from this genome object into the subject propclass.
  virtual void Store() = 0;

  /// Sets the fitness value associated with this genome.
  virtual void SetFitness(float fitness) = 0;

  /// Gets the fitness value associated with this genome.
  virtual float GetFitness() const = 0;

  /// Gets a pointer to the internal representation of the genome data.
  virtual void* GetInternalData() = 0;

  /// Fills the genome with random data.
  virtual void Randomize() = 0;

  /// Returns the number of genes in the genome.
  virtual size_t GetSize() const = 0;

  /// Creates a new genome using the crossover operation.
  virtual csPtr<iCelGenome> Crossover(iCelGenome *other, size_t point) const = 0;

  /// Mutates the genome.
  virtual void Mutate(float probability) = 0;
};

/**
 * The genetic algorithm implementation, incorporating its CEL property class.
 */
class celPcEvolve
: public scfImplementationExt1<celPcEvolve, celPcCommon, iPcEvolve>
{
  static PropertyHolder propinfo;
  csRef<celGenericParameterBlock> params;

  csRef<iMessageDispatcher> dispatcher_result;
  csRef<iMessageDispatcher> dispatcher_fitness;

  /// The property class that will be evolved.
  iCelPropertyClass *subject;

  /// Comparison function for sorting the population array by genome fitness.
  static int GenomeCompare(iCelGenome* const &a, iCelGenome* const &b)
  { return csArray<float>::DefaultCompare(b->GetFitness(), a->GetFitness()); }

  /// The size of the population of genomes.
  int32 popSize;

  /// The population of genomes.
  csRefArray<iCelGenome> population;

  /// Probability value to use when selecting genomes for crossover.
  float selectProb;

  /// Probability value to use when mutating genomes.
  float mutateProb;

  /// Index into the population array, marking the next genome to be evaluated.
  size_t nextGenome;

  /**
   * Re-entrant method that evolves the population and then evaluates the
   * fitness of each of the new genomes.
   */
  void Evolve();

  /**
   * Sends the pcevolve_fitness message to the behaviour to evaluate the
   * fitness of a genome.
   */
  void EvaluateFitness(size_t genome);

  /**
   * Selects two genomes for breeding based on their fitness, performs a
   * crossover and returns the result.
   */
  csPtr<iCelGenome> Crossover(size_t point);

  /**
   * The private method called by the "ReturnFitness" action.
   * Stores the returned fitness value (from params) in the genome and calls
   * the re-entrant method Evolve().
   */
  bool FitnessCallback(iCelParameterBlock *params);

  /**
   * The private method called by the "GetFitness" action.
   * Returns (in ret) the fitness value of the genome specified by the param.
   */
  bool GetFitness(iCelParameterBlock *params, celData &ret);

  /**
   * The private method called by the "Select" action. Stores in
   * subject the genome specified by the param.
   */
  bool SelectGenome(iCelParameterBlock *params);

  /**
   * Sorts the population of genomes based on their fitness, fittest first.
   */
  void SortByFitness() { population.Sort(GenomeCompare); }

  /**
   * A convenience template used to cut down on code repetition in Generate().
   * Populates the population array with new instances of class T. The class T
   * must implement iCelGenome.
   */
  template <class T>
  void InitPopulation()
  {
    for (size_t i = 0; i < size_t(popSize); i++)
      population.Push(csRef<iCelGenome> (csPtr<iCelGenome>
	(new T (this, subject))));
  }

public:
  /// Constructor.
  celPcEvolve(iObjectRegistry *objreg);

  /// Destructor.
  virtual ~celPcEvolve();

  /// Report error. Always returns false.
  bool Error(char const *desc, ...) const;
  /// Report warning. Always returns false.
  bool Warning(char const *desc, ...) const;
  /// Report bug. Always returns false.
  bool Bug(char const *desc, ...) const;

  //--------------------------------------------------------- celPcCommon --//
  virtual bool PerformActionIndexed(int index, iCelParameterBlock *params, celData &ret);
  virtual csPtr<iCelDataBuffer> Save() { return pl->CreateDataBuffer(0); }
  virtual bool Load(iCelDataBuffer *buf) { return true; }

  //----------------------------------------------------------- iPcEvolve --//
  virtual void SetPopulationSize(size_t size) { popSize = (int32)size; }
  virtual size_t GetPopulationSize() const { return size_t(popSize); }
  virtual void SetSubject(iCelPropertyClass *pc) { subject = pc; }
  virtual iCelPropertyClass* GetSubject() { return subject; }
  virtual void SetProbabilities(float select, float mutate)
  { CS_ASSERT(select > 0.0 && select <= 1.0); CS_ASSERT(mutate >= 0.0);
    selectProb = select; mutateProb = mutate; }

  virtual float GetFitness(size_t index) const
  { CS_ASSERT_MSG("Genome index out of bounds", index < size_t(popSize));
    return population[index]->GetFitness(); }
  virtual void SelectGenome(size_t index)
  { CS_ASSERT_MSG("Genome index out of bounds", index < size_t(popSize));
    population[index]->Store(); }

  virtual void ResetPopulation() { population.DeleteAll(); }
  virtual void Generate();
  virtual void FitnessCallback(float fitness);
};

/**
 * The iCelGenome implementation for interfacing with iPcNeuralNet.
 */
class celNeuralNetGenome
: public scfImplementation1<celNeuralNetGenome, iCelGenome>
{
  celPcEvolve *parent;
  iPcNeuralNet *pc;
  csRef<iCelNNWeights> data;
  float fitness;

public:
  /**
   * This static method returns true if the property class c is supported by
   * celNeuralNetGenome (i.e. if it implements iPcNeuralNet).
   */
  static bool Supports(iCelPropertyClass *c)
  { return csRef<iPcNeuralNet>(scfQueryInterface<iPcNeuralNet>(c)).IsValid(); }

  celNeuralNetGenome(celPcEvolve *evo, iCelPropertyClass *c)
  : scfImplementationType (this), parent (evo), fitness (0)
  { pc = csRef<iPcNeuralNet> (scfQueryInterface<iPcNeuralNet>(c));
    data.AttachNew(pc->CreateEmptyWeights()); }

  celNeuralNetGenome(celNeuralNetGenome *other) : scfImplementationType (this),
    parent (other->parent), pc (other->pc), fitness (0)
  { data.AttachNew(other->data->Clone()); }

  ~celNeuralNetGenome() {}

  virtual void Retrieve() { pc->GetWeights(data); }
  virtual void Store() { pc->SetWeights(data); }

  virtual void SetFitness(float f) { fitness = f; }
  virtual float GetFitness() const { return fitness; }
  virtual void* GetInternalData() { return data; }

  virtual void Randomize();
  virtual size_t GetSize() const;
  virtual csPtr<iCelGenome> Crossover(iCelGenome *other, size_t point) const;
  virtual void Mutate(float probability);
};

#endif // __CEL_PF_EVOLVEFACT__

