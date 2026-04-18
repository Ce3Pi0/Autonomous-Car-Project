#include "IrSensorArray.hpp"

IrSensorArray::IrSensorArray(const int sensValues[]) {
    for (int i = 0; i < this->_NUM_SENSORS; i++) {
        this->_sensValues[i] = sensValues[i];
    }
}

bool IrSensorArray::isValid() {
    int count = 0;

    for (int i = 0; i < _NUM_SENSORS; i++) {
        if (_sensValues[i] == 1) count ++;
    }

    if (count > 2) return false;

    for (int i = 0; i < _NUM_SENSORS; i++) {
        if (_sensValues[i] == 1 && count == 2) {
        if (i == 0) return _sensValues[i + 1] == 1;
        else if (i == _NUM_SENSORS - 1) return _sensValues[i - 1] == 1;

        return _sensValues[i + 1] == 1;
        }
    }

    return true;
}

float IrSensorArray::sumWeights() {
    return sumSide('L', 0, _NUM_SENSORS / 2) + sumSide('R', _NUM_SENSORS / 2, _NUM_SENSORS);
}

float IrSensorArray::getWeight(char side, int index){
    if (side == 'R' && index >= _NUM_SENSORS / 2 && index < _NUM_SENSORS) return _WEIGHTS[index - 3];
    else if (side == 'L' && index >= 0 && index < _NUM_SENSORS / 2) return -_WEIGHTS[2 - index];
    return 0.0;
}

float IrSensorArray::sumSide(char side, unsigned int start, unsigned int end) {
    if (side != 'L' && side != 'R') return 0.0;
    if (start < 0 || start >= _NUM_SENSORS) return -1.0;
    if (end <= 0 || end > _NUM_SENSORS || end <= start) return -1.0;

    const int k = side == 'R'? 1 : -1;

    int count = 0;

    for (int i = start; i < end; i++) {
        if (_sensValues[i] != 0) count += 1;
    }

    if (count == 1) {
        for (int i = start; i < end; i++) {
            if (_sensValues[i] != 0) return _sensValues[i] * getWeight(side, i);
        }
    } else if (count == 2) {
        if (side == 'R')
            for (int i = end - 1; i >= start; i--) {
                if (_sensValues[i] != 0) return _sensValues[i] * getWeight(side, i) + k * _DIST;
            }
        else 
            for (int i = start; i < end; i++) {
                if (_sensValues[i] != 0) return _sensValues[i] * getWeight(side, i) + k * _DIST;
            }
    }

    return 0.0;
}

int* IrSensorArray::getValues() {
    return this->_sensValues;
}
