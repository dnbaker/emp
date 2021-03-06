import struct
import gzip
import numpy as np

def load_from_path(path, minhash_sample=-1):
    def dec(x):
        return struct.unpack('l', x)[0]

    if int.from_bytes(open(path, "rb").read(2)) == 0x1f8b:
        with gzip.open(path, "rb") as f:
            s = f.read(8)
            nelem = dec(s)
            arr = np.zeros(nelem if minhash_sample < 0 else minhash_sample, dype=np.uint64)
            s = f.read(8)
            i = 0
            if minhash_sample > 0:
                while len(s) == 8 and minhash_sample < i:
                    arr[i] = dec(s)
                    i += 1
                    s = f.read(8)
            else:
                while len(s) == 8:
                    arr[i] = dec(s)
                    i += 1
                    s = f.read(8)
    else:
        arr = np.from_file(path, dtype=np.uint64)
    return arr


def shsets2dict(prefix, minhash_sample=-1):
    import glob
    fns = glob.glob(prefix + "*.shs")
    if not fns: return {}
    return {int(x.split(".")[-2]): load_from_path(x, minhash_sample) for x in fns}

'''
TODO:
1. Turn every genome into sets of sketches.
2. Do all pairwise comparisons.
3. Add curve-fitting code.
4. ???
5. PROFIT
'''
