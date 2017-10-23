TEMPLATE = subdirs

SUBDIRS += \
    TwoOhFourEightCore \
    Stinky \
    Bastard \
    NeuralNetwork

Stinky.depends = TwoOhFourEightCore NeuralNetwork
Bastard.depends = TwoOhFourEightCore NeuralNetwork
