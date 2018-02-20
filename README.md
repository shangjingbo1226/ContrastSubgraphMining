# Contrast Subgraph Mining from Coherent Cores

## Publication

Jingbo Shang, Xiyao Shi, Meng Jiang, Liyuan Liu, Timothy Hanratty, Jiawei Han, "[Contrast Subgraph Mining from Coherent Cores](https://arxiv.org/abs/1802.06189)", submitted to SIGKDD 18, under review. arXiv:1802.06189 [cs.SI]

## Requirements

Linux or MacOS with g++ and Python 2.7 installed.

Ubuntu:

* g++ 4.8 `$ sudo apt-get install g++-4.8`
* Python 2.7 `$ sudo apt-get install python2.7`

MacOS:

* g++ 6 `$ brew install gcc6`
* Python 2.7 `$ brew install python`

Python 2.7 package requirements :
 
* PIL
* glob
* numpy
* matplotlib
* seaborn
* pandas

## Run Command

Each run will generate a file containing contrast subgraph information and potentially a file containing core subgraph information if `<core>` is enabled. And there is also a post processing toolkit provided to generate heatmap of both contrast subgraph and coherent core subgraph.

#### Run Toy Example
`$ ./run.sh`

The default run will run the experiment on a toy example we created. The graph contains `18` nodes and `45` edges in total (i.e. in both layers). We are using node `1` as seed.

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

* `GraphName.csv` comma separated `int`, `string` pair, corresponding to `graph index, graph name`.

  Example file :
  ```
  $ head data/toy/GraphName.csv
  0,Graph 1
  1,Graph 2
  ```

* `NodeName.csv` comma separated `int`, `string` pair, corresponding to `node index, node name`.

  Example file :
  ```
  $ head data/toy/NodeName.csv
  0,node_0
  1,node_1
  2,node_2
  3,node_3
  4,node_4
  5,node_5
  6,node_6
  7,node_7
  8,node_8
  9,node_9
  ```

* `GraphData.csv` comma separated `list` of `int`, correspongding to graph `index, node u, node v, weight(u,v)`.

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

## Result

After each run of scipt `run.sh`, one can find corresponding raw data and heatmap for contrast subgraph and potentially coherent core in `output` folder. There will be three diagrams for each core/contrast subgraph, one for first graph, one for second graph and one for two diagrams combined in one. 

Find the core and contrast subgraph heatmaps for our toy example below :
 ![alt text][toy_core]

 ![alt text][toy_contrast]
 
[toy_core]: https://github.com/shangjingbo1226/ContrastSubgraphMining/blob/master/output/coherent.out_bi.png "Heatmap for coherent core in toy data"

[toy_contrast]: https://github.com/shangjingbo1226/ContrastSubgraphMining/blob/master/output/contrast.out_bi.png "Heatmap for contrast subgraph in toy data"
