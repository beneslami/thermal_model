## Compilation
Simply run ```make``` in the parent directory.

## Execution
Since it is the preliminary level, you simply run ```./AccelTherm```. It generates a set of random numbers and feeds them to each component. Those component (for now it's GPU, HBM and PIM) receives the random number and calculate the temperature.

## visualizer
To visualize the thermal flow, you may run the below command in the parent directory
```python3 -m visualizer --input output.txt --show```
