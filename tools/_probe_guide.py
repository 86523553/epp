
import struct, zlib, sys
def load(p):
    d = open(p,'rb').read(); i = 8; w=h=0; idat=b''; ct=0
    while i < len(d):
        ln = struct.unpack('>I', d[i:i+4])[0]; typ = d[i+4:i+8]; body = d[i+8:i+8+ln]
        if typ == b'IHDR':
            w,h,bd,ct = struct.unpack('>IIBB', body[:10])
        elif typ == b'IDAT': idat += body
        elif typ == b'IEND': break
        i += 12 + ln
    raw = zlib.decompress(idat)
    ch = {0:1,2:3,3:1,4:2,6:4}[ct]
    stride = w*ch
    out = bytearray(h*stride); prev = bytearray(stride); pos = 0
    for y in range(h):
        f = raw[pos]; pos += 1
        line = bytearray(raw[pos:pos+stride]); pos += stride
        if f == 1:
            for x in range(ch, stride): line[x] = (line[x] + line[x-ch]) & 255
        elif f == 2:
            for x in range(stride): line[x] = (line[x] + prev[x]) & 255
        elif f == 3:
            for x in range(stride):
                a = line[x-ch] if x >= ch else 0
                line[x] = (line[x] + ((a + prev[x]) >> 1)) & 255
        elif f == 4:
            for x in range(stride):
                a = line[x-ch] if x >= ch else 0
                b = prev[x]; c = prev[x-ch] if x >= ch else 0
                pa=abs(b-c); pb=abs(a-c); pc=abs(a+b-2*c)
                pr = a if (pa<=pb and pa<=pc) else (b if pb<=pc else c)
                line[x] = (line[x] + pr) & 255
        out[y*stride:(y+1)*stride] = line; prev = line
    return w,h,ch,out

for p in sys.argv[1:]:
    w,h,ch,px = load(p)
    targets = {'dark_guide':(0x6E,0x74,0x84)}
    print(p, 'size=%dx%d ch=%d'%(w,h,ch))
    for name,t in targets.items():
        n = 0
        for y in range(h):
            base = y*w*ch
            for x in range(w):
                o = base + x*ch
                if px[o]==t[0] and px[o+1]==t[1] and px[o+2]==t[2]: n += 1
        print('  %s = %d px' % (name, n))
