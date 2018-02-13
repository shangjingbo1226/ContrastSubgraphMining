make

#full recommended version
.\bin\analyze.exe DBLP 0 1 1 1 1 1 1 585

#without core
.\bin\analyze.exe DBLP 0 1 0 0 1 1 -1 585

#with no neighbor restriction
.\bin\analyze.exe DBLP 0 1 1 0 1 1 -1 585

python post_processing_src/densityMatrix.py