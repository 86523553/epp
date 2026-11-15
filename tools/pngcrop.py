import os
# 最小 PNG 裁剪缩放工具（纯标准库：zlib + struct）
import zlib, struct, sys

def read_png(path):
    d = open(path, 'rb').read()
    assert d[:8] == b'\x89PNG\r\n\x1a\n'
    pos = 8; w = h = 0; bitd = ctype = None; idat = b''
    while pos < len(d):
        ln, typ = struct.unpack('>I4s', d[pos:pos+8]); pos += 8
        data = d[pos:pos+ln]; pos += ln + 4
        if typ == b'IHDR':
            w, h, bitd, ctype, _, _, _ = struct.unpack('>IIBBBBB', data)
            assert bitd == 8 and ctype in (2, 6), f'unsupported {bitd} {ctype}'
        elif typ == b'IDAT':
            idat += data
        elif typ == b'IEND':
            break
    raw = zlib.decompress(idat)
    bpp = 4 if ctype == 6 else 3
    stride = w * bpp
    out = bytearray(w * h * bpp)
    prev = bytearray(stride)
    p = 0
    for y in range(h):
        f = raw[p]; p += 1
        line = bytearray(raw[p:p+stride]); p += stride
        if f == 1:
            for i in range(bpp, stride): line[i] = (line[i] + line[i-bpp]) & 255
        elif f == 2:
            for i in range(stride): line[i] = (line[i] + prev[i]) & 255
        elif f == 3:
            for i in range(stride):
                a = line[i-bpp] if i >= bpp else 0
                line[i] = (line[i] + ((a + prev[i]) >> 1)) & 255
        elif f == 4:
            for i in range(stride):
                a = line[i-bpp] if i >= bpp else 0
                b = prev[i]
                c = prev[i-bpp] if i >= bpp else 0
                pp = a + b - c
                pa, pb, pc = abs(pp-a), abs(pp-b), abs(pp-c)
                pr = a if (pa <= pb and pa <= pc) else (b if pb <= pc else c)
                line[i] = (line[i] + pr) & 255
        out[y*stride:(y+1)*stride] = line
        prev = line
    return w, h, bpp, bytes(out)

def write_png(path, w, h, bpp, pix):
    ctype = 6 if bpp == 4 else 2
    stride = w * bpp
    raw = bytearray()
    for y in range(h):
        raw.append(0)   # 每行扫描线前置过滤器类型 0
        raw += pix[y*stride:(y+1)*stride]
    comp = zlib.compress(bytes(raw), 6)
    def chunk(t, data):
        c = struct.pack('>I', len(data)) + t + data
        return c + struct.pack('>I', zlib.crc32(t + data) & 0xffffffff)
    png = b'\x89PNG\r\n\x1a\n'
    png += chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, ctype, 0, 0, 0))
    png += chunk(b'IDAT', comp)
    png += chunk(b'IEND', b'')
    open(path, 'wb').write(png)

def crop_scale(src, dst, cx, cy, cw, ch, scale):
    w, h, bpp, pix = read_png(src)
    cx = max(0, cx); cy = max(0, cy)
    cw = min(cw, w - cx); ch = min(ch, h - cy)
    ow, oh = cw * scale, ch * scale
    out = bytearray(ow * oh * bpp)
    stride = w * bpp
    for y in range(oh):
        sy = cy + y // scale
        for x in range(ow):
            sx = cx + x // scale
            so = (y * ow + x) * bpp
            si = sy * stride + sx * bpp
            out[so:so+bpp] = pix[si:si+bpp]
    write_png(dst, ow, oh, bpp, out)
    print(f'{dst}: {ow}x{oh}')

if __name__ == '__main__':
    src, dst = sys.argv[1], sys.argv[2]
    cx, cy, cw, ch, scale = map(int, sys.argv[3:8])
    crop_scale(src, dst, cx, cy, cw, ch, scale)
