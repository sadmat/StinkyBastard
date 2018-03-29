#ifndef DEFAULTS_H
#define DEFAULTS_H

namespace nn2048
{

const double DefaultWeightDistribution = 0.02;

const double DefaultGammaFactor = 0.5;
const double DefaultLearningRate = 0.08;
const double DefaultMomentumFactor = 0.02;
const double DefaultEpsilonFactor = 0.15;
const unsigned DefaultReplayMemorySize = 100000;
const unsigned DefaultReplayBatchSize = 5000;

const unsigned short DefaultServerPort = 4000;

const unsigned long DefaultHighscoreToRecordThreshold = 10000;

}

#endif // DEFAULTS_H
