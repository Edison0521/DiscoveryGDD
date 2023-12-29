

# README

## System Requirements
- Java 1.8
- C++17 and GCC 7.x or later
- Python 3

## Python Dependencies
- pandas
- glob
- itertools
- Levenshtein
- multiprocessing

## Compilation
Compile the homomorphic frequent subgraph mining part using `./build.sh` in the `homo_freq_pattern` folder. Run with parameters `./gpmh -d file-t Threshold`.

## Execution Steps
1. **Graph Sampling**: 
   - Run `j2p.py` in `\example\resource`.
   - Paste the result file path into `main.py` in the `sampling` folder. This generates `subgraph.csv`.
   - Place `subgraph.csv` back in the original directory and run `nn2l.py` to get lg graph files in `# t 1` format for graph mining.

2. **Graph Mining**: 
   - Two methods provided: homomorphic frequent subgraph mining and frequent subgraph mining followed by homomorphic subgraph matching.
   - For the first method, use the compiled files from Step 2 to get homomorphic frequent subgraph mining results.
   - For the second method, run `java -jar GraMi_ExactSubgraphMatching-1.0-SNAPSHOT.jar filename=graph.lg datasetFolder=resource/ freq=T`, followed by `java -jar graphflow-0.1.0.jar`.

3. **Generating Pseudo Relationship Tables**: 
   - Run `java -jar TableCreater.jar`.
   - Input data folder: `test/process_2_producer/pre_Table`.
   - Output data folder: `result/`.

4. **Dependency Mining**: 
   - Run `python minGDD.py -c -s -a`, where `c` is confidence, `s` is the number of segments for each attribute, and `a` is the maximum number of attributes for mining.

## Additional Information
- `example\test\process_1_producer` contains `GraphEdges.csv` and `GraphVertices.csv` for the original knowledge graph.
- File formats in `example\test\process_1_producer\purePattern2` should be `(num:NodeType)-[EdgeType]->(Num:NodeType)`.
- The format for `example\test\process_2_producer\pre_Table` should be specified.
- A Yago subset example is provided. To mine graph differential dependencies, run `python minGDD.py -c -s -a` in `example\test\process_3_producer\proTable_meaning`.

## Datasets
The graph datasets used in our paper can be downloaded [here](https://drive.google.com/drive/folders/1VodFmIh5UZ4pShYM58x11iR_aRBD9PZ3?usp=drive_link).

## References
@article{sun2022depth,
  title={An in-depth study of continuous subgraph matching},
  author={Sun, Xibo and Sun, Shixuan and Luo, Qiong and He, Bingsheng},
  journal={Proceedings of the VLDB Endowment},
  volume={15},
  number={7},
  pages={1403--1416},
  year={2022},
  publisher={VLDB Endowment}
}

@article{elseidy2014grami,
  title={Grami: Frequent subgraph and pattern mining in a single large graph},
  author={Elseidy, Mohammed and Abdelhamid, Ehab and Skiadopoulos, Spiros and Kalnis, Panos},
  journal={Proceedings of the VLDB Endowment},
  volume={7},
  number={7},
  pages={517--528},
  year={2014},
  publisher={VLDB Endowment}
}
@inproceedings{vezvaei2022fine,
  title={Fine-Tuning Dependencies with Parameters.},
  author={Vezvaei, Alireza and Golab, Lukasz and Kargar, Mehdi and Srivastava, Divesh and Szlichta, Jaroslaw and Zihayat, Morteza},
  booktitle={EDBT},
  pages={2--393},
  year={2022}
}
