#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
  public:
    float Utilization();  // TODO: See src/processor.cpp

    // TODO: Declare any necessary private members
  private:
    long prevtotal{0};
    long previdle{0};
};

#endif