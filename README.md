# GNU Radio 4 proposed directory structure

In order to preserve the LGPL licensing of graph-prototype from fair-acc, this structure proposes
being explicit in folders with LGPL as the default licensing

The only exceptions are the `blocks` and `kernel` folders which are GPLv3 licensed as they carry
over any non-rewritten IP from GR3


## kernel
These are the kernel methods that remain GPL

## blocks
These are the blocks from GR3 that remain GPL

## blocks-lgpl
These are blocks that are completely re-written for GR4 that the authors have chosen to make LGPL.
Obvious things that can go in here are simple math blocks (add, multiply, etc.), basic blocks for general functionality
like vector source and sink, null source and sink, etc.

## core
This is the graph-prototype code with the core functionalities for GR4

## schedulers
These are schedulers
