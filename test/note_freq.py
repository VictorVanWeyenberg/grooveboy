def midi_note_to_wanted_freq(note_index):
    return 440*(2**((note_index-69)/12))

def gba_note_to_real_freq(note_index):
    return 131072/(2048-note_index)

def real_freq_to_gba_note(freq):
    return round((131072-(2048*freq))/-freq)

def midi_note_to_gba_note(note_index):
    wanted_freq = midi_note_to_wanted_freq(note_index)
    gba_note = real_freq_to_gba_note(wanted_freq)
    return gba_note

print("%6s | %6s | %16s | %16s | %16s" % ("MIDI", "GBA", "WANTED", "ACTUAL", "ERROR"))
for midi_note in range(36, 97):
    gba_note = midi_note_to_gba_note(midi_note)
    wanted_freq = midi_note_to_wanted_freq(midi_note)
    actual_freq = gba_note_to_real_freq(gba_note);
    print("%6d | %6d | %16.2f | %16.2f | %16.2f" % (midi_note, gba_note, wanted_freq, actual_freq, wanted_freq - actual_freq))
