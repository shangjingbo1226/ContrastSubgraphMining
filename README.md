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

## Default Run

#### Reproduce Paper Result
`$ ./run.sh`

The default run will run the experiment described in paper in the order of

* Recommended algorithm in paper
* Contrast subgraph without core
* Contrast subgraph without neighboring constraint

#### Custom Run
`$ ./bin/analyze _dataFolderName_ _graphNumA_ _graphNumB_ _core_ _neighbor_ _useLogEdge_ _coreStepLen_ _contrastStepLen_ _listOfSeeds_`

Upon using, a folder containing graph data should be placed in folder `data`. The graph data consists of three parts:

* `GraphName.csv` in format `graphIndex, graphName`
* `NodeName.csv`  in format `nodeIndex, nodeName`
* `GraphData.csv` in format `graphIndex, nodeIndex1, nodeIndex2, edgeWeight`
