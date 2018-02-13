# ContrastSubgraphMining
Contrast Subgraph Mining from Coherent Cores

## Requirements

Linux or MacOS with g++ and Python 2.7 installed.

Ubuntu:

* g++ 4.8 `$ sudo apt-get install g++-4.8`
* Python 2.7 `$ sudo apt-get install python2.7`

MacOS:

* g++ 6 `$ brew install gcc6`
* Python 2.7 `$ brew install python`

## Run Command

Each run will generate a file containing contrast subgraph information and potentially a file containing core subgraph information if `<core>` is enabled. And there is also a post processing toolkit provided to generate heatmap of both contrast subgraph and coherent core subgraph.

#### Run Toy Example
`$ ./run.sh`

The default run will run the experiment on a toy example we created. We are using node `1` as seed, and we label those nodes irrelevant to `1` by `noise`, nodes closely related to `1` in both graphs by `core` and nodes related to `1` but structured differently by `contrast`.

#### Custom Run
```
$ ./bin/analyze <dataFolderName> <graphNumA> <graphNumB> <core> <neighbor> <useLogEdge> <coreStepLen> <contrastStepLen> <listOfSeeds>
$ python post_processing_src/visualize.py
```

Parameter explanation:

```
<dataFolderName> : string, data folder as described below
<graphNumA> : int, index of the first graph/layer
<graphNumB> : int, index of the second graph/layer
<core> : bool, 1 to enable coherent core
<neighbor> : bool, 1 to enable neighbor constraint on contrast subgraph
<useLogEdge> : bool, 1 to enable log edge weight
<coreStepLen> : int, neighbor step length for coherent core
<contrastStepLen> : int, neighbor step length for contrast subgraph
<listOfSeeds> : optional, comma separated integers, list of node indices as seeds
```

Upon using, a folder containing graph data should be placed in folder `data`. The graph data consists of three parts:

* `GraphName.csv`

  Example file :
  ```
  $ head data/toy/GraphName.csv
  0,Graph 1
  1,Graph 2
  ```

* `NodeName.csv` 

  Example file :
  ```
  $ head data/toy/NodeName.csv
  1,core
  2,core
  3,core
  4,core
  5,core
  6,contrast
  7,contrast
  8,contrast
  9,contrast
  ```

* `GraphData.csv`

  Example file :
  ```
  $ head data/toy/NodeName.csv
  0,1,2,3
  0,1,3,1
  0,1,4,2
  0,1,5,3
  0,2,3,3
  0,2,4,2
  0,3,5,4
  0,4,5,2
  0,0,1,2
  0,2,6,3
  ```
