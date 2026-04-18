#ifndef IRSENSORARRAY_HPP
#define IRSENSORARRAY_HPP

class IrSensorArray {
public:
    static constexpr unsigned int _NUM_SENSORS = 6;

    IrSensorArray(const int sensValues[]);
    bool isValid();
    float sumWeights();
    int* getValues();

private:
    
    static constexpr float _DIST = 1.27f / 2;
    static constexpr float _SENS_W = 1.27f;

    const float _WEIGHTS[_NUM_SENSORS / 2] = {-_DIST, -(_DIST + _SENS_W * 2), -(_DIST + _SENS_W * 4)};

    int _sensValues[_NUM_SENSORS];

    float getWeight(char, int);
    float sumSide(char, unsigned int, unsigned  int);
};

#endif
