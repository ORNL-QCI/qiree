### Build apptainer
```
$  apptainer build --fakeroot qiree.sif scripts/apptainers/apptainer.def
```
"--fakeroot" is optional.

### Run apptainer
```
$ apptainer exec qiree.sif /bin/bash
```
You have qir-xacc binary executable set in your PATH environment.
