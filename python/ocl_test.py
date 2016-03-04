import pyopencl as cl
import numpy as np
import numpy.linalg as la
mf       = cl.mem_flags


def slurp(fname):
  with open(fname,"r") as f:
    return f.read()

def box13_(a):
  b = np.empty_like(a)[0:-12]
  for i in xrange(len(b)):
    b[i] = a[i:(i+13)].sum()

  return b

def box_13x13(a, dtype = np.int32):
  """ Very slow version of 13x13 box filter.
  """
  from numpy import ndarray
  H,W = a.shape
  
  b = ndarray( (H-12,W-12), dtype=dtype )

  for r in xrange(H-12):
    for c in xrange(W-12):
      b[r,c] = a[r:r+13,c:c+13].sum()

  return b



N = 1024
a = np.random.random_integers(0,255,N).astype(np.int32)
b = np.zeros([N-12], dtype=np.int32)

ctx   = cl.create_some_context()
queue = cl.CommandQueue(ctx, properties=cl.command_queue_properties.PROFILING_ENABLE)

g_a = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=a)
g_b = cl.Buffer(ctx, mf.WRITE_ONLY, b.nbytes )

src = slurp("k.cl")
prg = cl.Program(ctx, src).build()

#prg.sum(queue, a.shape, None, a_buf, b_buf, dest_buf)
#a_plus_b = np.empty_like(a)
#cl.enqueue_copy(queue, a_plus_b, dest_buf)

g_sz = b.shape
l_sz = None

kernel = cl.Kernel(prg, "box13x13")

evt = kernel(queue, g_sz, l_sz, g_a, g_b)
evt.wait()
dt = evt.profile.end - evt.profile.start

print dt, " ns"

cl.enqueue_copy(queue, b, g_b)

b_ref = box13_(a)

if (b_ref == b).all():
  print "OK!"
else:
  print "FAILURE!"




#print(la.norm(a_plus_b - (a+b)), la.norm(a_plus_b))
