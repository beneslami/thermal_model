#include <iostream>
#include "AccelTherm.h"
#include <unistd.h>
#include <random>

int main() {
    std::string configFilePath = "config.cfg";
    std::string outputFilePath = "output.txt";
    AccelTherm *accelTherm = new AccelTherm(configFilePath, outputFilePath);

    int i = 0;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 100); 
    while(i < 20) { 
        int random = dist(rng);
        accelTherm->calculate("HBM", 0, random);
        random = dist(rng);
        accelTherm->calculate("HBM", 1, random);
        random = dist(rng);
        accelTherm->calculate("HBM", 2, random);
        random = dist(rng);
        accelTherm->calculate("HBM", 3, random);
        random = dist(rng);
        accelTherm->calculate("HBM", 4, random);
        random = dist(rng);
        accelTherm->calculate("HBM", 5, random);
        random = dist(rng);
        accelTherm->calculate("HBM", 6, random);
        random = dist(rng);
        accelTherm->calculate("HBM", 7, random);


        random = dist(rng);
        accelTherm->calculate("GPU", 0, random);
        random = dist(rng);
        accelTherm->calculate("GPU", 1, random);
        random = dist(rng);
        accelTherm->calculate("GPU", 2, random);
        random = dist(rng);
        accelTherm->calculate("GPU", 3, random);

        
        random = dist(rng);
        accelTherm->calculate("PIM", 0, random);
        random = dist(rng);
        accelTherm->calculate("PIM", 1, random);
        random = dist(rng);
        accelTherm->calculate("PIM", 2, random);
        random = dist(rng);
        accelTherm->calculate("PIM", 3, random);
        i++;
        sleep(1);
    }
    accelTherm->printAll();
    accelTherm->finish();
    delete accelTherm;
    return 0;
}