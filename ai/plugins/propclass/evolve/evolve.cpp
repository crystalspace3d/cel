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

#include "cssysdef.h"
#include "iutil/objreg.h"
#include "ivaria/reporter.h"

#include "evolve.h"

#include <stdlib.h>

CS_IMPLEMENT_PLUGIN
CEL_IMPLEMENT_FACTORY(Evolve, "pcai.evolve")

inline void celRandSeed() { srand(csGetTicks()); }
inline float celRand() { return (float)rand() / RAND_MAX; }

enum // actionID
{
  EVO_GENERATE = 0,
  EVO_RTNFITNESS,
  EVO_GETFITNESS,
  EVO_SELECT,
  EVO_RESET
};

enum // propertyID
{
  EVO_POPSIZE = 0,
  EVO_SUBJECT,
  EVO_SELECTPROB,
  EVO_MUTATEPROB,
  EVO_PROP_LAST
};

const char* const descriptions[] = {
  "The size of the population (number of different genomes).",
  "The property class to use when evaluating the fitness of a specific genome.",
  "P in the tournament selection probability formula (P * (1-P)^rank).",
  "The average number of genes to mutate per genome. Can be less than one."
};

enum // parameterID
{
  EVO_FITNESS = 0,
  EVO_INDEX,
  EVO_MAXFITNESS,
  EVO_MINFITNESS,
  EVO_AVGFITNESS,
  EVO_PARAM_LAST
};

csStringID paramid[EVO_PARAM_LAST] = { csInvalidStringID };

//----------------------------------------------------------- celPcEvolve --//

PropertyHolder celPcEvolve::propinfo;

celPcEvolve::celPcEvolve(iObjectRegistry *objreg)
: scfImplementationType (this, objreg),
  subject (0),
  popSize (0),
  selectProb (1),
  mutateProb (0),
  nextGenome (0)
{
  SetName("pcevolve");
  propholder = &propinfo;

  if (! propinfo.actions_done)
  {
    AddAction(EVO_GENERATE, "cel.action.Generate");
    AddAction(EVO_RTNFITNESS, "cel.action.ReturnFitness");
    AddAction(EVO_GETFITNESS, "cel.action.GetFitness");
    AddAction(EVO_SELECT, "cel.action.Select");
    AddAction(EVO_RESET, "cel.action.Reset");
  }

  propinfo.SetCount(EVO_PROP_LAST);
  AddProperty(EVO_POPSIZE, "cel.property.population", CEL_DATA_LONG,
	false, descriptions[EVO_POPSIZE], &popSize);
  AddProperty(EVO_SUBJECT, "cel.property.subject", CEL_DATA_PCLASS,
	false, descriptions[EVO_SUBJECT], &subject);
  AddProperty(EVO_SELECTPROB, "cel.property.selectProbability", CEL_DATA_FLOAT,
	false, descriptions[EVO_SELECTPROB], &selectProb);
  AddProperty(EVO_MUTATEPROB, "cel.property.mutateProbability", CEL_DATA_FLOAT,
	false, descriptions[EVO_MUTATEPROB], &mutateProb);

  if (paramid[0] == csInvalidStringID)
  {
    paramid[EVO_FITNESS] = pl->FetchStringID("cel.parameter.fitness");
    paramid[EVO_INDEX] = pl->FetchStringID("cel.parameter.index");
    paramid[EVO_MAXFITNESS] = pl->FetchStringID("cel.parameter.max_fitness");
    paramid[EVO_MINFITNESS] = pl->FetchStringID("cel.parameter.min_fitness");
    paramid[EVO_AVGFITNESS] = pl->FetchStringID("cel.parameter.avg_fitness");
  }

  params.AttachNew(new celGenericParameterBlock (3));
  params->SetParameterDef(0, paramid[EVO_MAXFITNESS], "max_fitness");
  params->SetParameterDef(1, paramid[EVO_MINFITNESS], "min_fitness");
  params->SetParameterDef(2, paramid[EVO_AVGFITNESS], "avg_fitness");
}

celPcEvolve::~celPcEvolve()
{
}

bool celPcEvolve::PerformActionIndexed(int index, iCelParameterBlock *params, celData &ret)
{
  if (! subject) return Error("Propclass not properly set up.");

  switch (index)
  {
    case EVO_GENERATE: Generate(); return true;
    case EVO_RTNFITNESS: return FitnessCallback(params);
    case EVO_GETFITNESS: return GetFitness(params, ret);
    case EVO_SELECT: return SelectGenome(params);
    case EVO_RESET: ResetPopulation(); return true;
    default:;
  }
  return false;
}

void celPcEvolve::Generate()
{
  nextGenome = 0;

  if (popSize == 0 || ! subject)
  {
    Error("Generate: propclass not properly set up.");
    return;
  }

  if (population.GetSize() == 0)
  {
    if (celNeuralNetGenome::Supports(subject))
      InitPopulation<celNeuralNetGenome>();
    // else if...
      // add other iCelGenome implementations here
    else
    {
      Error("Unsupported subject propclass.");
      return;
    }

    population[0]->Retrieve();
    celRandSeed();
    for (size_t i = 1; i < size_t(popSize); i++)
      population[i]->Randomize();
  }

  Evolve();
}

void celPcEvolve::Evolve()
{
  if (nextGenome == 0) // First call to Evolve() for the current generation.
  {
    celRandSeed();

    size_t genomeSize = population[0]->GetSize();
    csRefArray<iCelGenome> temp (popSize);

    // Keep the fittest 25% of the population.
    for (size_t i = 0; i < size_t(popSize / 4); i++)
      temp.Push(const_cast<iCelGenome*> (population[i]));

    // Replace the other 75% with offspring.
    for (size_t i = temp.GetSize(); i < size_t(popSize); i++)
    {
      csRef<iCelGenome> g (Crossover((size_t) (genomeSize * celRand())));
      g->Mutate(mutateProb / genomeSize);
      temp.Push(g);
    }

    population = temp;
  }

  if (nextGenome < size_t(popSize))
  {
    EvaluateFitness(nextGenome++); // Tell the behaviour to evaluate fitness.
  }
  else // Finished evolving the current generation.
  {
    SortByFitness();

    float maxFitness = population[0]->GetFitness();
    float minFitness = population.Top()->GetFitness();
    float avgFitness = 0;
    for (size_t i = 0; i < size_t(popSize); i++)
      avgFitness += population[i]->GetFitness();
    avgFitness /= popSize;

    params->GetParameter(0).Set(maxFitness);
    params->GetParameter(1).Set(minFitness);
    params->GetParameter(2).Set(avgFitness);
    celData ret;
    entity->GetBehaviour()->SendMessage("pcevolve_result", this, ret, params);
  }
}

csPtr<iCelGenome> celPcEvolve::Crossover(size_t point)
{
  // Do tournament selection.
  size_t mom, dad;
  for (mom = 0; mom < size_t(popSize - 1); mom++)
  {
    if (celRand() < selectProb) break;
  }
  for (dad = 0; dad < size_t(popSize - 1); dad++)
  {
    if (celRand() < selectProb) break;
  }

  // Do one-point crossover.
  return population[mom]->Crossover(population[dad], point);
}

void celPcEvolve::EvaluateFitness(size_t genome)
{
  population[genome]->Store(); // Store the genome in the subject propclass.

  celData ret;
  entity->GetBehaviour()->SendMessage("pcevolve_fitness", this, ret, 0);
}

void celPcEvolve::FitnessCallback(float fitness)
{
  // The behaviour has finished evaluating the fitness.
  CS_ASSERT(nextGenome > 0);
  population[ nextGenome - 1 ]->SetFitness(fitness);
  Evolve();
}

bool celPcEvolve::FitnessCallback(iCelParameterBlock *params)
{
  if (nextGenome == 0)
    return Error("FitnessCallback: propclass not properly set up.");

  const celData *data = params->GetParameter(paramid[EVO_FITNESS]);
  if (! (data && data->type == CEL_DATA_FLOAT))
    return Error("ReturnFitness takes 1 parameter, float 'fitness'.");
  float fitness = data->value.f;
  if (fitness < 0.0)
    return Error("Fitness may not be less than 0.");

  FitnessCallback(fitness);
  return true;
}

bool celPcEvolve::GetFitness(iCelParameterBlock *params, celData &ret)
{
  const celData *data = params->GetParameter(paramid[EVO_INDEX]);
  if (! (data && data->type == CEL_DATA_LONG))
    return Error("GetFitness takes 1 parameter, long 'index'.");
  size_t index = size_t(data->value.l);
  if (index >= size_t(popSize))
    return Error("GetFitness: 'index' out of bounds.");

  ret.Set(population[index]->GetFitness());
  return true;
}

bool celPcEvolve::SelectGenome(iCelParameterBlock *params)
{
  const celData *data = params->GetParameter(paramid[EVO_INDEX]);
  if (! (data && data->type == CEL_DATA_LONG)) return false;
    return Error("SelectGenome takes 1 parameter, long 'index'.");
  size_t index = size_t(data->value.l);
  if (index >= size_t(popSize)) return false;
    return Error("SelectGenome: 'index' out of bounds.");

  population[index]->Store();
  return true;
}

bool celPcEvolve::Error(char const *desc, ...) const
{
  va_list args;
  va_start(args, desc);
  csReportV(const_cast<iObjectRegistry*>(object_reg),
	CS_REPORTER_SEVERITY_ERROR, "cel.propclass.ai.evolve", desc, args);
  va_end(args);
  return false;
}

bool celPcEvolve::Warning(char const *desc, ...) const
{
  va_list args;
  va_start(args, desc);
  csReportV(const_cast<iObjectRegistry*>(object_reg),
	CS_REPORTER_SEVERITY_WARNING, "cel.propclass.ai.evolve", desc, args);
  va_end(args);
  return false;
}

bool celPcEvolve::Bug(char const *desc, ...) const
{
  va_list args;
  va_start(args, desc);
  csReportV(const_cast<iObjectRegistry*>(object_reg),
	CS_REPORTER_SEVERITY_BUG, "cel.propclass.ai.evolve", desc, args);
  va_end(args);
  return false;
}

//---------------------------------------------------- celNeuralNetGenome --//

void celNeuralNetGenome::Randomize()
{
  csArray< csArray< csArray<float> > > &d = data->Data();

  for (size_t i = 0, endi = d.GetSize(); i < endi; i++)
  {
    for (size_t j = 0, endj = d[i].GetSize(); j < endj; j++)
    {
      for (size_t k = 0, endk = d[i][j].GetSize(); k < endk; k++)
      {
        d[i][j][k] = 4 * celRand() - 2;
      }
    }
  }
}

size_t celNeuralNetGenome::GetSize() const
{
  const csArray< csArray< csArray<float> > > &d = data->Data();

  size_t size = 0;
  for (size_t i = 0, endi = d.GetSize(); i < endi; i++)
  {
    for (size_t j = 0, endj = d[i].GetSize(); j < endj; j++)
    {
      size += d[i][j].GetSize();
    }
  }
  return size;
}

csPtr<iCelGenome> celNeuralNetGenome::Crossover(iCelGenome *other, size_t point) const
{
  iCelGenome *childgenome = new celNeuralNetGenome ((celNeuralNetGenome*)other);
  csArray< csArray< csArray<float> > > &child =
    ((iCelNNWeights *) childgenome->GetInternalData())->Data();
  const csArray< csArray< csArray<float> > > &d = data->Data();

  bool beyond = false;
  for (size_t i = 0, endi = d.GetSize(); i < endi; i++)
  {
    for (size_t j = 0, endj = d[i].GetSize(); j < endj; j++)
    {
      const csArray<float> &d_i_j = d[i][j];

      if (beyond)
      {
	for (size_t k = 0, endk = d_i_j.GetSize(); k < endk; k++)
	  child[i][j][k] = d_i_j[k];
      }
      else if (point < d_i_j.GetSize())
      {
	beyond = true;
	for (size_t k = point + 1, endk = d_i_j.GetSize(); k < endk; k++)
	  child[i][j][k] = d_i_j[k];
      }
      else
      {
	point -= d_i_j.GetSize();
      }
    }
  }
  return childgenome;
}

void celNeuralNetGenome::Mutate(float probability)
{
  csArray< csArray< csArray<float> > > &d = data->Data();

  for (size_t i = 0, endi = d.GetSize(); i < endi; i++)
  {
    for (size_t j = 0, endj = d[i].GetSize(); j < endj; j++)
    {
      csArray<float> &d_i_j = d[i][j];

      for (size_t k = 0, endk = d_i_j.GetSize(); k < endk; k++)
      {
	if (celRand() < probability) d_i_j[k] = 4 - celRand() * 2;
      }
    }
  }
}
