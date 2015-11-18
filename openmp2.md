## Test Results for openmp1.c ##

On the workstations of NCTU CSCC, using up to 4 threads.

Data Size: L

| # | rev.   | Init Time (s) | Exec Time (s) | Speedup |
|---|--------|---------------|---------------|---------|
| 1 | origin | 5.950         | 89.712003     |         |
| 2 | origin | 5.954         | 89.750979     |         |
| 3 | origin | 5.952         | 89.791790     |         |
|   |        | 5.952         | 89.751591     | 1       |
| 4 | rev1.0 | 5.319         | 47.724929     | 1.88    |
| 5 | rev1.0 | 5.317         | 47.636639     | 1.88    |
| 6 | rev1.0 | 5.293         | 47.598091     | 1.89    |

Data Size: M

| # | rev.   | Init Time (s) | Exec Time (s) | Speedup |
|---|--------|---------------|---------------|---------|
| 1 | origin | 0.587         | 1.665527      |         |
| 2 | origin | 0.588         | 1.664340      |         |
| 3 | origin | 0.586         | 1.664885      |         |
|   |        | 0.586         | 1.664917      | 1       |
| 4 | rev1.0 | 0.570         | 1.233768      | 1.35    |
| 5 | rev1.0 | 0.577         | 1.223876      | 1.36    |
| 6 | rev1.0 | 0.564         | 1.194111      | 1.39    |

**Question: Most of the time I will get extremely slow results. Why?**  
Solution: Resolve the false sharing issue.
