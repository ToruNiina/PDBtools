##PDB(mainly CG style in CafeMol) editer tool

必要に迫られて作ったものなど。

arabica is a name of one of the coffee trees.

###Usage

seqextr outputs DNA sequence in pdb file

`$./seqextr [option: -cg] <filename>.pdb`

mutator outputs mutated DNA pdb file to "out.pdb" (cg style only)

`$./mutator <filename>.pdb`

NOTE: mutator can read only one block. If you have more than one DNA strand to change the sequence (eg, double strand DNA), please prepare the complemental sequence and separate input CG pdb file into files include only one block that contain only one strand. I will implove this defect soon.

###Reference

 "CafeMol: A coarse-grained biomolecular simulator for simulating proteins at work. H. Kenzaki, N. Koga, N. Hori, R. Kanada, W. Li, K. Okazaki, XQ. Yao, and S. Takada Journal of Chemical Theory and Computation (2011) 7(6) pp1979-1989 DOI:10.1021/ct2001045"
