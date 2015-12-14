#include "optimizersettings.h"


QList<int> OptimizerSettings::getSteps() const
{
    return steps;
}

void OptimizerSettings::setSteps(const QList<int> &value)
{
    steps = value;
}

SelectedOptimizer OptimizerSettings::getOptimizer() const
{
    return optimizer;
}

void OptimizerSettings::setOptimizer(const SelectedOptimizer &value)
{
    optimizer = value;
}

QString OptimizerSettings::toString() const
{
    QString optstr = "Optimizer settings:\n";
    optstr.append("Selected optimizer: ");
    switch (optimizer) {
    case OPT_NOT_SET:
        optstr.append("Not set\n");
        break;
    case RUNONCE:
        optstr.append("RUNONCE\n");
        break;
    case LSH:
        optstr.append("LSH\n");
        break;
    case EROPT:
        optstr.append("EROPT\n");
        break;
    default:
        break;
    }
    optstr.append("Max iterations: " + QString::number(maxIterations) + "\n");
    optstr.append("Max continous iterations: " + QString::number(maxIterContinuous) + "\n");
    optstr.append("Perturbation size: " + QString::number(perturbationSize) + "\n");
    optstr.append("Termination: " + QString::number(termination) + "\n");
    optstr.append("Termination start: " + QString::number(terminationStart) + "\n");
    optstr.append("Staring point update: " + QString::number(startingpointUpdate) + "\n");
    optstr.append("Initial step length: " + QString::number(initialStepLength) + "\n");
    optstr.append("Minimum step length: " + QString::number(minimumStepLength) + "\n");
    optstr.append("Bookkeeper tolerance: " + QString::number(bookkeeperTolerance) + "\n");
    return optstr;
}

double OptimizerSettings::getInitialStepLength() const
{
    return initialStepLength;
}

void OptimizerSettings::setInitialStepLength(double value)
{
    initialStepLength = value;
}

double OptimizerSettings::getMinimumStepLength() const
{
    return minimumStepLength;
}

void OptimizerSettings::setMinimumStepLength(double value)
{
    minimumStepLength = value;
}

double OptimizerSettings::getBookkeeperTolerance() const
{
    return bookkeeperTolerance;
}

void OptimizerSettings::setBookkeeperTolerance(double value)
{
    bookkeeperTolerance = value;
}

int OptimizerSettings::getMaxEvaluations() const
{
    return maxEvaluations;
}

void OptimizerSettings::setMaxEvaluations(int value)
{
    maxEvaluations = value;
}
OptimizerSettings::OptimizerSettings()
{
    optimizer = OPT_NOT_SET;
    maxIterations = 1;
    maxIterContinuous = 100;
    perturbationSize = 0.0001;
    termination = 0.0;
    terminationStart = 5;
    startingpointUpdate = false;
}

int OptimizerSettings::getMaxIterations() const
{
    return maxIterations;
}

void OptimizerSettings::setMaxIterations(int value)
{
    maxIterations = value;
}

int OptimizerSettings::getMaxIterContinuous() const
{
    return maxIterContinuous;
}

void OptimizerSettings::setMaxIterContinuous(int value)
{
    maxIterContinuous = value;
}

double OptimizerSettings::getPerturbationSize() const
{
    return perturbationSize;
}

void OptimizerSettings::setPerturbationSize(double value)
{
    perturbationSize = value;
}

double OptimizerSettings::getTermination() const
{
    return termination;
}

void OptimizerSettings::setTermination(double value)
{
    termination = value;
}

int OptimizerSettings::getTerminationStart() const
{
    return terminationStart;
}

void OptimizerSettings::setTerminationStart(int value)
{
    terminationStart = value;
}

bool OptimizerSettings::getStartingpointUpdate() const
{
    return startingpointUpdate;
}

void OptimizerSettings::setStartingpointUpdate(bool value)
{
    startingpointUpdate = value;
}