##PDB(mainly CG style in CafeMol) editer tool

必要に迫られて作ったものなど。

arabica is a name of one of the coffee trees.

###Usage

seqextr outputs DNA sequence in pdb file

`$./seqextr [option: -cg] <filename>.pdb`

mutator can change the sequence of DNA and Protein in CG style PDB file.

this outputs mutated pdb file named "outX.pdb". X is chain ID.

`$./mutator <filename>.pdb`

mutator regards '=' as "no change in this residue". so if you fill the input sequence with '=', mutator outputs the same file as input pdb file.

when you input "pass" instead of modified sequence, mutator outputs nothing and goes next chain.

if you want to quit the mutation, type "quit", "end", "exit", or "bye" instead of sequence.

###Reference

 "CafeMol: A coarse-grained biomolecular simulator for simulating proteins at work. H. Kenzaki, N. Koga, N. Hori, R. Kanada, W. Li, K. Okazaki, XQ. Yao, and S. Takada Journal of Chemical Theory and Computation (2011) 7(6) pp1979-1989 DOI:10.1021/ct2001045"
