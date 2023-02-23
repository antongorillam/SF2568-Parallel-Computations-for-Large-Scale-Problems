# Instruction for loggin in Dardel

Create a Kerberos ticket and then ssh to Dardel:
```
kinit -f whafu@NADA.KTH.SE
ssh -o GSSAPIDelegateCredentials=yes -o GSSAPIKeyExchange=yes -o GSSAPIAuthentication=yes whafu@dardel.pdc.kth.se
```

To allocate time-slot, run `salloc -n <#core> -t <#time> -p shared -A edu23.sf2568`, an example might be:
```
cc poisson1D.skel.c -o poisson1D.skel.x
salloc -n 5 -t 00:00:10 -p shared -A edu23.sf2568
srun ./poisson1D.skel.x -03
```

