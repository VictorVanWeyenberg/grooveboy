CLK_FREQ = 16777216
UINT16_MAX = 65536

def dpm_to_start(bpm):
    freq = CLK_FREQ
    for scaler in [1, 64, 256, 1024]:
        bps = bpm/60
        start = (freq/scaler)/bps
        if round(start) < UINT16_MAX:
            return [round(start), scaler]
    return [0, 1]

[print("%dbpm => %s" % (bpm, dpm_to_start(bpm))) for bpm in range(1, 441)]
