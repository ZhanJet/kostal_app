#include <iostream>
#include <vector>

class MovingAverage {
public:
    MovingAverage(int size) : size(size), sum(0) {}

    double next(double val) {
        if (window.size() == size) {
            sum -= window.front();
            window.erase(window.begin());
        }

        window.push_back(val);
        sum += val;

        return sum / window.size();
    }

private:
    int size;
    double sum;
    std::vector<double> window;
};