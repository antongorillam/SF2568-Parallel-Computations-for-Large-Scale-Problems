# Instruction for loggin in Dardel

Create a Kerberos ticket and then ssh to Dardel:
```
kinit -f whafu@NADA.KTH.SE
ssh -o GSSAPIDelegateCredentials=yes -o GSSAPIKeyExchange=yes -o GSSAPIAuthentication=yes whafu@dardel.pdc.kth.se
```

To allocate time-slot, run `salloc -n <#core> -t <#time> -p shared -A edu23.sf2568`, an example might be:
```
cc mandelbrot_mpi_new.c -o mandelbrot_mpi_new.x
salloc -n 1 -t 00:00:45 -p shared -A edu23.sf2568
srun ./mandelbrot_mpi_new.x
```

