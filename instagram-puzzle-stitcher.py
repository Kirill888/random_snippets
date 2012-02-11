#!/usr/bin/env python

"""
My take on the Instagram unshredder puzzle:

http://instagram-engineering.tumblr.com/post/12651721845/instagram-engineering-challenge-the-unshredder


"""

def loadImage(fname):
  import Image
  from numpy import ndarray

  im = Image.open(fname)
  w,h = im.size

  buf = im.tostring()
  nc = len(buf)/(w*h)

  dims = [h,w,nc] if nc > 1 else [h,w]
  return ndarray(dims ,dtype='uint8', buffer=buf)

def togray(im):
  if len(im.shape) == 2:
    return im.astype('f')/255.0
  return im.mean(2,dtype='float32')/255.0

def splitIm(im, sw):
  return [ im[:,i:i+sw] for i in range(0,im.shape[1],sw) ]

def probVE(y, medianWindow):
  from scipy import signal, c_, var, exp

  ee = signal.convolve2d(c_[ y[:,:1], y ], c_[1.,-1.],'valid')
  x = (-ee*ee/var(ee)).sum(0)
  x -= x.max()
  x /= abs(x.min())
  x  = exp(-x)

  xm = signal.medfilt(x,medianWindow)
  xd = (x-xm)/xm

  return x, xd

def findBestStripe(y, medianW = 5, peakThr = 0.1):
  def validStripSizes(w, minStripW=5):
    return [ i for i in range(minStripW,w/2+1) if w%i == 0]

  def getPeaks(x, thresh ):
    return [i for i,v in enumerate(x) if v > thresh]

  def stripeSupport(stripW, imW, peaks):
    edges     = set(range(stripW,imW, stripW))
    bad_edges = edges - peaks
    return 1.0 - len(bad_edges)/float(len(edges))

  def firstMax(x):
    xmax = max(x)
    for i,v in enumerate(x):
      if v == xmax:
        return i

  w      = y.shape[1]
  x,xd   = probVE(y, medianW)
  peaks  = set(getPeaks(xd,peakThr))

  strips = validStripSizes( w )
  sw     = [ stripeSupport(s, w, peaks) for s in strips ]
  si     = firstMax(sw)
  return strips[si]

def reconstruct(y,stripW):
  def stitchQ(y1,y2):
    """Smaller is better"""
    from numpy import c_
    x,xd = probVE(c_[y1,y2],5)
    return xd[y1.shape[1]]

  def greedy_find(nodes, nchunks):
    def follow(m, i):
      n = [i]
      while m[i] != None:
        n.append(m[i])
        i = m[i]
      return n

    ns = nchunks - 1 #Number of stitches is one less than chunks

    lr = [None]*nchunks #Left to right mapping
    rl = [None]*nchunks #Right to left mapping
    c  = 0 #Early exit counter

    for i,j in nodes:
      if lr[i] == None and rl[j] == None:
        leftOfI  = follow(rl,i)
        rightOfJ = follow(lr,j)

        if not(j in leftOfI) and not(i in rightOfJ) :
          lr[i] = j
          rl[j] = i
          c += 1

          if c == ns:
            return follow(lr,leftOfI[-1])

    return None
 
  def computeGraph(chunks):
    import itertools
    from numpy import argsort, r_

    n = len(chunks)
    ww,pairs = [],[]

    for i,j in itertools.permutations(range(n),2) :
      if i != j:
        pairs += [(i,j)]
        ww += [stitchQ( chunks[i], chunks[j])]
    return r_[ pairs ][ argsort(r_[ww]) ]

  chunks = splitIm(y,stripW)
  graph  = computeGraph(chunks)

  return greedy_find( graph , len(chunks))

if __name__ == '__main__':
  import sys
  from numpy import hstack
  from PIL import Image

  if not(len(sys.argv) in (3,4)):
    print "Usage: input-image output-image [strip-width in pixels or 'auto']"
    sys.exit(1)

  in_fname, out_fname = sys.argv[1:3]
  stripW = sys.argv[3] if len(sys.argv)>3  else  "auto"

  im = loadImage(in_fname)
  y  = togray(im)
  if stripW == "auto":
    sw  = findBestStripe(y)
  else:
    sw = int(stripW)
    
  print "Using strip width: %d" %(sw)

  ii = reconstruct(y,sw)
  if ii == None:
    print "Failed to reconstruct strip order"
    sys.exit(1)

  chunks = splitIm(im,sw)
  im_out = Image.fromarray( hstack([chunks[i] for i in ii]) )
  im_out.save( out_fname )
  print "Saved output to:", out_fname
  sys.exit(0)

