# 2019-05 PM workshops Korea and Japan #




## libpmemobj-cpp workshop ##

These are the materials used during the workshop:
- slides.pdf contains the slides shown
- Commands demonstrated during the workshop are listed below.
- Source files for programming examples are in this repo.
These instructions are designed to work on the workshop guest VMs.
Start by cloning this repo, so you can easily cut and paste commands from
this README into your shell.
Many of the sys admin steps described here can be found in the
Getting Started Guide on pmem.io:
https://docs.pmem.io/getting-started-guide

Start by making a clone of this repo...
```sh
$ git clone https://github.com/pmemhackathon/2019-05
$ cd 2019-05
```

Compile examples.
```sh
$ make
```

export ${USER} variable for convenient copy-pasting commands
```sh
$ export USER=pmdkuser{1..100}
```

## Warmup - a simple persistent counter. ##

```sh
$ pmempool create obj --layout=warmup -s 100M /mnt/pmem-fsdax0/${USER}/warmup
$ ./warmup /mnt/pmem-fsdax0/${USER}/warmup
```

## find_bugs.cpp ##

Program which contains few bugs. Can you find them?
```sh
$ pmempool create obj --layout=find_bugs -s 100M /mnt/pmem-fsdax0/${USER}/find_bugs
$ ./find_bugs /mnt/pmem-fsdax0/${USER}/find_bugs
```

run find-bugs under pmemcheck
```sh
$ valgrind --tool=pmemcheck ./find_bugs /mnt/pmem-fsdax0/${USER}/find_bugs
```

## queue.cpp ##

Simple implementation of a volatile queue.
```sh
$ ./queue
$ push 1
$ push 2
$ push 3
$ pop
$ show
$ exit
```

## queue_pmemobj.cpp ##

Simple implementation of a persistent queue.
```sh
$ pmempool create obj --layout=queue -s 100M /mnt/pmem-fsdax0/${USER}/queue
$ pmempool info /mnt/pmem-fsdax0/${USER}/queue
$ ./queue_pmemobj /mnt/pmem-fsdax0/${USER}/queue
$ push 1
$ push 2
$ push 3
$ pop
$ show
$ exit
```

## simplekv_simple.cpp ##

Hashmap test program.
```sh
$ pmempool create obj --layout=simplekv -s 100M /mnt/pmem-fsdax0/${USER}/simplekv-simple
$ pmempool info /mnt/pmem-fsdax0/${USER}/simplekv-simple
$ ./simplekv_simple /mnt/pmem-fsdax0/${USER}/simplekv-simple
```

## simplekv_word_count.cpp ##

A C++ program which reads words to a simplekv hashtable and uses MapReduce
to count words in specified text files.
```sh
$ pmempool create obj --layout=simplekv -s 100M /mnt/pmem-fsdax0/${USER}/simplekv-words
$ pmempool info /mnt/pmem-fsdax0/${USER}/simplekv-words
$ ./simplekv_word_count /mnt/pmem-fsdax0/${USER}/simplekv-words words1.txt words2.txt
```
