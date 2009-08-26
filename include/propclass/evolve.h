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

#ifndef __CEL_PF_EVOLVE__
#define __CEL_PF_EVOLVE__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/refcount.h"
#include "csutil/array.h"

#include "physicallayer/datatype.h"

/**
 * A property class implementing a genetic algorithm.
 *
 * Currently the only thing that can be evolved using this is the
 * celPcNeuralNet, but it would be fairly easy to extend it to support
 * evolving other property classes where applicable.
 *
 * The property class holds the following properties:
 * - "population" (long) The size of the population (number of genomes).
 * - "subject" (propclass) The property class that will be evolved.
 * - "select_probability" (float) Parameter for stochastic selection.
 * - "mutate_probability" (float) Parameter for stochastic mutation.
 *
 * This property class supports the following actions (add prefix 'cel.evolve.action.'
 * if you want to access this action through a message):
 * - "Generate"	Begins evolution of one generation.
 * - "ReturnFitness" The behaviour calls this after evaluating the fitness
 *   of the presently selected genome. Parameter: "fitness" (float).
 * - "GetFitness" Gets the fitness of a genome. Parameter: "index" (long).
 * - "Select" Stores a genome in subject. Parameter: "index" (long).
 * - "Reset" Resets the population to its state before "Generate" was called.
 *
 * And sends the following message to the behaviour:
 * - "pcevolve_fitness" Tells the behaviour to evaluate the fitness of the
 *   genome currently stored in the subject propclass.
 * - "pcevolve_result" Signals completion of evolution of one generation.
 *   Parameters: "max_fitness", "min_fitness", "avg_fitness" (floats).
 */
struct iPcEvolve : public virtual iBase
{
  SCF_INTERFACE(iPcEvolve, 0, 0, 1);

  /**
   * Sets the population size (number of individual genomes).
   */
  virtual void SetPopulationSize(size_t size) = 0;

  /**
   * Returns the population size (number of individual genomes).
   */
  virtual size_t GetPopulationSize() const = 0;

  /**
   * Sets the subject property class (the property class that will be evolved
   * by this one). It must be of a type supported by the genetic algorithm.
   */
  virtual void SetSubject(iCelPropertyClass *pc) = 0;

  /**
   * Returns the subject property class (the property class being evolved by
   * this one.
   */
  virtual iCelPropertyClass* GetSubject() = 0;

  /**
   * Sets the probability parameters for the stochastic (non-deterministic)
   * randomness of the genetic algorithm.
   * \param select Genomes will be selected for crossover with a probability
   *   of <tt>p(1-p)^r</tt>, where p is this parameter and r is the genome's
   *   rank in descending order of fitness (0 being the fittest).
   * \param mutate The average number of genes to mutate in each genome.
   *   For example, 3.0 means that an average of 3 genes will be mutated,
   *   while 0.5 means that one gene will be mutated in every 2 genomes, on
   *   average.
   */
  virtual void SetProbabilities(float select, float mutate) = 0;

  /**
   * Instructs the property class to begin the evolution of one generation.
   * This will send the "pcevolve_fitness" message once for each individual
   * genome in the population, to ask the behaviour to evaluate its fitness.
   */
  virtual void Generate() = 0;

  /**
   * Informs the property class that the fitness of a genome has been
   * evaluated, as requested by the "pcevolve_fitness" message.
   * \param fitness The fitness value. May not be less than zero.
   */
  virtual void FitnessCallback(float fitness) = 0;

  /**
   * Returns the fitness of the genome specified by the index parameter.
   * This will be zero if its fitness has yet to be evaluated.
   * \param index The index into the population array, which is sorted in
   *   descending order of fitness (0 being the fittest).
   */
  virtual float GetFitness(size_t index) const = 0;

  /**
   * Stored the genome specified by the index parameter into the subject
   * property class.
   * \param index The index into the population array, which is sorted in
   *   descending order of fitness (0 being the fittest).
   */
  virtual void SelectGenome(size_t index) = 0;

  /**
   * Resets the property class to its initial state before Generate() was
   * first called.
   */
  virtual void ResetPopulation() = 0;
};

#endif // __CEL_PF_EVOLVE__

