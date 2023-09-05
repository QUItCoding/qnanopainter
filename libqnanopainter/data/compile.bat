qsb --qt6 fill.vert -O -c -o fill.vert.qsb
qsb --qt6 fill.frag -O -c -o fill.frag.qsb
qsb --qt6 fill.frag -D SCISSORING -O -c -o fill_sc.frag.qsb
qsb --qt6 fill.frag -D EDGE_AA=1 -O -c -o fill_aa.frag.qsb
qsb --qt6 fill.frag -D EDGE_AA=1 -D STENCIL_STROKES=1 -O -c -o fill_aa_ss.frag.qsb
qsb --qt6 fill.frag -D EDGE_AA=1 -D SCISSORING -O -c -o fill_aa_sc.frag.qsb
qsb --qt6 fill.frag -D EDGE_AA=1 -D STENCIL_STROKES=1 -D SCISSORING -O -c -o fill_aa_ss_sc.frag.qsb
