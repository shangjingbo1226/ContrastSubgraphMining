make

#full recommended version
./bin/analyze.exe toy 0 1 1 1 1 1 1 1

#without core
#./bin/analyze.exe toy 0 1 0 0 1 1 -1 1

#with no neighbor restriction
#./bin/analyze.exe toy 0 1 1 0 1 1 -1 1

python post_processing_src/visualize.py