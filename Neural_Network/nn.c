#include "nn.h"

struct NeuralNetwork
{
  int nbInputs;
  int nbHidden;
  int nbOutputs;

  struct Matrix InputValues;
  struct Matrix Aim;
  struct Matrix WeightsIH;
  struct Matrix WeightsHO;
  struct Matrix BiasH;
  struct Matrix OutputH;
  struct Matrix SumIH;
  struct Matrix Errors;
  struct Matrix Derivatives;
  struct Matrix GradientsHO;
  struct Matrix GradientsIH;
  struct Matrix GradientsB;
  struct Matrix PreviousWeightsIH;
  struct Matrix PreviousWeightsHO;
  struct Matrix PreviousBiasH;

  struct Matrix GradientBiasOutput;
  struct Matrix BiasO;
  struct Matrix SumHOutputs;
  struct Matrix FinalOutput;
  struct Matrix DerivativeO;
  double LearningRate;
  double ConstanteUpdate;
  struct Matrix PreviousBiasO;
};

struct NeuralNetwork InitializeNetwork()
{
  struct NeuralNetwork network;

  network.nbInputs = 28*28;
  network.nbHidden = 20;
  network.nbOutputs = 52;
  

  network.InputValues = CreateMatrix(52 * nbInputs, 1);
  // Chargement des caractères

  network.Aim = CreateMatrix(52, 1);
  // Chargement de l'étiquette

  network.Errors = CreateMatrix(52, 1);
  // Initialisation de la matrice à 1
  
  network.WeightsIH = CreateMatrix(network.nbHidden, network.nbInputs);
  network.WeightsHO = CreateMatrix(network.nbOutputs, network.nbHidden);
  network.OutputH = CreateMatrix(network.nbHidden, 1);
  network.BiasH = CreateMatrix(network.nbHidden, 1);
  network.SumIH = CreateMatrix(network.nbHidden, 1);
  network.Derivatives = CreateMatrix(network.nbHidden, 1);
  network.PreviousWeightsIH = CreateMatrix(network.nbHidden, network.nbInputs);
  network.PreviousWeightsHO = CreateMatrix(network.nbOutputs, network.nbHidden);
  network.PreviousBiasH = CreateMatrix(network.nbHidden, 1);
  network.GradientsIH = CreateMatrix(network.nbHidden, network.nbInputs);
  network.GradientsHO = CreateMatrix(network.nbOutputs, network.nbHidden);
  network.GradientsB = CreateMatrix(network.nbHidden, 1);

  network.BiasO = CreateMatrix(1, 1);
  network.SumHOutputs = CreateMatrix(1, 1);
  network.PreviousBiasO = CreateMatrix(1, 1);
  network.GradientBiasOutput = CreateMatrix(1, 1);
  network.SumHOutputs = CreateMatrix(1, 1);
  network.FinalOutput = CreateMatrix(1, 1);
  network.DerivativeO = CreateMatrix(1, 1);

  ChangeMatrix(network.BiasO, 0, 0, Random());
  InitMatrix(network.WeightsIH);
  InitMatrix(network.WeightsHO);
  InitMatrix(network.BiasH);
  InitMatrixZero(network.PreviousWeightsHO);
  InitMatrixZero(network.PreviousWeightsIH);
  InitMatrixZero(network.PreviousBiasH);
  ChangeMatrix(network.SumHOutputs, 0, 0, 0.0);
  network.LearningRate = 0.5;
  network.ConstanteUpdate =  1;
  ChangeMatrix(network.PreviousBiasO, 0, 0, 0.0);
  
  return network;
}


double Sigmoid(double Sum)
{
  return 1/(exp(-Sum)+1);
}

void ForwardPass(struct NeuralNetwork network, int inpValues)
{
  for (int h = 0; h < network.nbHidden; h++)
    {
      double SumIH = 0;

      for (int i = 0; i < network.nbInputs; i++)
	{
	  SumIH += NavMatrix(network.WeightsIH, h, i) *
	    NavMatrix(network.InputValues, inpValues+i, 0);
	}

      SumIH += NavMatrix(network.BiasH, h, 0);
      ChangeMatrix(network.SumIH, h, 0, SumIH);
      ChangeMatrix(network.OutputH, h, 0, Sigmoid(SumIH));
      }
  

  double SumHO = 0;
  for (int i = 0; i < network.nbHidden; i++)
    {
      SumHO += NavMatrix(network.WeightsHO, 0, i) *
	NavMatrix(network.OutputH, i, 0);
	}

  
  SumHO += NavMatrix(network.BiasO, 0, 0);
  ChangeMatrix(network.SumHOutputs, 0, 0, SumHO);
  ChangeMatrix(network.FinalOutput, 0, 0, Sigmoid(SumHO));
  ChangeMatrix(network.Errors, inpValues, 0,
	       NavMatrix(network.FinalOutput, 0, 0) -
	       NavMatrix(network.Aim, inpValues, 0));
}

void CalculateDerivative(struct NeuralNetwork network, int pattern)
{
  ChangeMatrix(network.DerivativeO, 0, 0, -(NavMatrix(network.Errors, pattern, 0) *
				      exp(NavMatrix(network.SumHOutputs, 0, 0))) /
	       (pow(exp(NavMatrix(network.SumHOutputs, 0, 0))+1, 2)));

  for (int i = 0; i < network.nbHidden; i++)
    {
      ChangeMatrix(network.Derivatives, i, 0,
		   (exp(NavMatrix(network.SumIH, i, 0)) /
		   pow((1 + exp(NavMatrix(network.SumIH, i, 0))), 2)) *
		   NavMatrix(network.WeightsHO, 0, i) *
		   (NavMatrix(network.DerivativeO, 0, 0)));
    }
}

void CalculateGradient(struct NeuralNetwork network, int pattern)
{
  for (int i = 0; i < network.nbInputs; i++)
    {
      for (int h = 0; h < network.nbHidden; h++)
	{
	  ChangeMatrix(network.GradientsIH, h, i,
		       Sigmoid(NavMatrix(network.InputValues,
					 pattern + i, 0)) *
		       NavMatrix(network.Derivatives, h, 0));
	}
    }

  for (int h = 0; h < network.nbHidden; h++)
    {
      ChangeMatrix(network.GradientsB, h, 0, Sigmoid(1) *
		   NavMatrix(network.Derivatives, h, 0));
    }

  ChangeMatrix(network.GradientBiasOutput, 0, 0, Sigmoid(1) *
	       (NavMatrix(network.DerivativeO, 0, 0)));

  for (int h = 0; h < network.nbHidden; h++)
    {
      ChangeMatrix(network.GradientsHO, 0, h,
		   Sigmoid(NavMatrix(network.OutputH, h, 0)) *
		   NavMatrix(network.DerivativeO, 0, 0));
    }
}

void UpdateWeights(struct NeuralNetwork network)
{
  for (int h = 0; h < network.nbHidden; h++)
    {
      for (int i = 0; i < network.nbInputs; i++)
	{
	  ChangeMatrix(network.PreviousWeightsIH, h, i,
		       (network.LearningRate *
			NavMatrix(network.GradientsIH, h, i)) +
		       (network.ConstanteUpdate *
			NavMatrix(network.PreviousWeightsIH, h, i)));

	  ChangeMatrix(network.WeightsIH, h, i,
		       (NavMatrix(network.WeightsIH, h, i) +
			NavMatrix(network.PreviousWeightsIH, h, i)));
	}
    }


  for (int h = 0; h < network.nbHidden; h++)
    {
      ChangeMatrix(network.PreviousWeightsHO, 0, h,
		   (network.LearningRate *
		    NavMatrix(network.GradientsHO, 0, h)) +
		   (network.ConstanteUpdate *
		    NavMatrix(network.PreviousWeightsHO, 0, h)));

      ChangeMatrix(network.WeightsHO, 0, h,
		   (NavMatrix(network.WeightsHO, 0, h) +
		    NavMatrix(network.PreviousWeightsHO, 0, h)));
    }

  for (int h = 0; h < network.nbHidden; h++)
    {
      ChangeMatrix(network.PreviousBiasH, h, 0,
		   (network.LearningRate *
		    NavMatrix(network.GradientsB, h, 0)) +
		   (network.ConstanteUpdate *
		    NavMatrix(network.PreviousBiasH, h, 0)));

      ChangeMatrix(network.BiasH, h, 0,
		   (NavMatrix(network.BiasH, h, 0) +
		    NavMatrix(network.PreviousBiasH, h, 0)));
    }

  ChangeMatrix(network.PreviousBiasO, 0, 0,
	       (network.LearningRate *
		NavMatrix(network.GradientBiasOutput, 0, 0)) +
		   (network.ConstanteUpdate *
		    NavMatrix(network.PreviousBiasO, 0, 0)));
  ChangeMatrix(network.BiasO, 0, 0, NavMatrix(network.BiasO, 0, 0) +
	                        NavMatrix(network.PreviousBiasO, 0, 0));
}

void BackPropagation(struct NeuralNetwork network, int pattern)
{
  CalculateDerivative(network, pattern);
  CalculateGradient(network, pattern);
  UpdateWeights(network);
}

void Test()
{
  struct NeuralNetwork network = InitializeNetwork();
  int nbEpoch = 50000;
  
  for (int epoch = 0; epoch < nbEpoch; epoch++)
    {
      for (int i = 0; i < 52*28*28-1; i += 52)
	{
	  ForwardPass(network, i);
	  BackPropagation(network, i);
	}
    }
}
