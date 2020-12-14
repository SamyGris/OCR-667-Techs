#include "nn.h"

struct NeuralNetwork
{
  int nbInputs;
  int nbHidden;
  int nbOutputs;

  struct Matrix matInputValues;
  struct Matrix matAim;
  struct Matrix matWeightsIH;
  struct Matrix matWeightsHO;
  struct Matrix matBiasH;
  struct Matrix matOutputH;
  struct Matrix matSumIH;
  struct Matrix matErrors;
  struct Matrix matDerivatives;
  struct Matrix matGradientsHO;
  struct Matrix matGradientsIH;
  struct Matrix matGradientsB;
  struct Matrix matPreviousWeightsIH;
  struct Matrix matPreviousWeightsHO;
  struct Matrix matPreviousBiasH;

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
  struct NeuralNetwork net;

  net.nbInputs = 28*28;
  net.nbHidden = 20;
  net.nbOutputs = 52;
  net.LearningRate = 0.5;
  net.ConstanteUpdate = 1.0;

  for (int i = 0; i < net.nbInputs; i++)
    {
      for (int h = 0; h < net.nbHidden; h++)
	{
	  ChangeMatrix(matWeightsIH, i, h, 
	}
    }
  
  return net;
}


double Sigmoid(double Sum)
{
  return (1.0/(1.0 + exp(-Sum)));
}

void ForwardPass(struct NeuralNetwork net, int inpValues)
{
  for (int h = 0; h < net.nbHidden; h++)
    {
      double SumIH = 0;

      for (int i = 0; i < net.nbInputs; i++)
	{
	  SumIH += NavMatrix(net.matWeightsIH, h, i) *
	    NavMatrix(net.matInputValues, inpValues + i, 0);
	}

      SumIH += NavMatrix(net.matBiasH, h, 0);
      ChangeMatrix(net.matSumIH, h, 0, SumIH);
      ChangeMatrix(net.matOutputH, h, 0, Sigmoid(SumIH));
    }

  double SumHO = 0;
  for (int i = 0; i < net.nbHidden; i++)
    {
      SumHO += NavMatrix(net.matWeightsHO, 0, i) *
	NavMatrix(net.matOutputH, i, 0);
    }

  SumHO += NavMatrix(net.BiasO, 0, 0);
  ChangeMatrix(net.SumHOutputs, 0, 0, SumHO);
  ChangeMatrix(net.FinalOutput, 0, 0, Sigmoid(SumHO));
  ChangeMatrix(net.matErrors, inpValues, 0,
	       NavMatrix(net.FinalOutput, 0, 0) -
	       NavMatrix(net.matAim, inpValues, 0));
}

void CalculateDerivative(struct NeuralNetwork net, int pattern)
{
  ChangeMatrix(net.DerivativeO, 0, 0, -(NavMatrix(net.matErrors, pattern, 0) *
				      exp(NavMatrix(net.SumHOutputs, 0, 0))) /
	               (pow((1 + exp(NavMatrix(net.SumHOutputs, 0, 0))), 2)));

  for (int i = 0; i < net.nbHidden; i++)
    {
      ChangeMatrix(net.matDerivatives, i, 0,
		   (exp(NavMatrix(net.matSumIH, i, 0)) /
		   pow((1 + exp(NavMatrix(net.matSumIH, i, 0))), 2)) *
		   NavMatrix(net.matWeightsHO, 0, i) *
		   (NavMatrix(net.DerivativeO, 0, 0)));
    }
}

void CalculateGradient(struct NeuralNetwork net, int pattern)
{
  for (int i = 0; i < net.nbInputs; i++)
    {
      for (int h = 0; h < net.nbHidden; h++)
	{
	  ChangeMatrix(net.matGradientsIH, h, i,
		       Sigmoid(NavMatrix(net.matInputValues,
					 pattern + i, 0)) *
		       NavMatrix(net.matDerivatives, h, 0));
	}
    }

  for (int h = 0; h < net.nbHidden; h++)
    {
      ChangeMatrix(net.matGradientsB, h, 0, Sigmoid(1) *
		   NavMatrix(net.matDerivatives, h, 0));
    }

  ChangeMatrix(net.GradientBiasOutput, 0, 0, Sigmoid(1) *
	       (NavMatrix(net.DerivativeO, 0, 0)));

  for (int h = 0; h < net.nbHidden; h++)
    {
      ChangeMatrix(net.matGradientsHO, 0, h,
		   Sigmoid(NavMatrix(net.matOutputH, h, 0)) *
		   NavMatrix(net.DerivativeO, 0, 0));
    }
}

void UpdateWeights(struct NeuralNetwork net)
{
  for (int h = 0; h < net.nbHidden; h++)
    {
      for (int i = 0; i < net.nbInputs; i++)
	{
	  ChangeMatrix(net.matPreviousWeightsIH, h, i,
		       (net.LearningRate *
			NavMatrix(net.matGradientsIH, h, i)) +
		       (net.ConstanteUpdate *
			NavMatrix(net.matPreviousWeightsIH, h, i)));

	  ChangeMatrix(net.matWeightsIH, h, i,
		       (NavMatrix(net.matWeightsIH, h, i) +
			NavMatrix(net.matPreviousWeightsIH, h, i)));
	}
    }


  for (int h = 0; h < net.nbHidden; h++)
    {
      ChangeMatrix(net.matPreviousWeightsHO, 0, h,
		   (net.LearningRate *
		    NavMatrix(net.matGradientsHO, 0, h)) +
		   (net.ConstanteUpdate *
		    NavMatrix(net.matPreviousWeightsHO, 0, h)));

      ChangeMatrix(net.matWeightsHO, 0, h,
		   (NavMatrix(net.matWeightsHO, 0, h) +
		    NavMatrix(net.matPreviousWeightsHO, 0, h)));
    }

  for (int h = 0; h < net.nbHidden; h++)
    {
      ChangeMatrix(net.matPreviousBiasH, h, 0,
		   (net.LearningRate *
		    NavMatrix(net.matGradientsB, h, 0)) +
		   (net.ConstanteUpdate *
		    NavMatrix(net.matPreviousBiasH, h, 0)));

      ChangeMatrix(net.matBiasH, h, 0,
		   (NavMatrix(net.matBiasH, h, 0) +
		    NavMatrix(net.matPreviousBiasH, h, 0)));
    }

  ChangeMatrix(net.PreviousBiasO, 0, 0,
	       (net.LearningRate *
		NavMatrix(net.GradientBiasOutput, 0, 0)) +
		   (net.ConstanteUpdate *
		    NavMatrix(net.PreviousBiasO, 0, 0)));
  ChangeMatrix(net.BiasO, 0, 0, NavMatrix(net.BiasO, 0, 0) +
	                        NavMatrix(net.PreviousBiasO, 0, 0));
}

void BackPropagation(struct NeuralNetwork net, int pattern)
{
  CalculateDerivative(net, pattern);
  CalculateGradient(net, pattern);
  UpdateWeights(net);
}

void TestForNeuralNetwork()
{
  struct NeuralNetwork nettest = InitializeNetwork();
  for (int epoch = 0; epoch < 50000; epoch++)
    {
      for (int i = 0; i < 7; i += 2)
	{
	  ForwardPass(nettest, i);
	  BackPropagation(nettest, i);

	  if (epoch == 499)
	    {
	      printf("Input 1: %f | Input 2: %f | Ouput: %f\n",
		     NavMatrix(nettest.matInputValues, i, 0),
		     NavMatrix(nettest.matInputValues, i+1, 0),
		     NavMatrix(nettest.FinalOutput, 0, 0));
	    }
	}
    }
}
