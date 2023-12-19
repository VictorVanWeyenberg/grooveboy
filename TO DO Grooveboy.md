# TO DO Grooveboy

- User experience
  - Turn notes off on new tracker.
  - Turn notes on on change.
- Finish channel 3 and 4 sound.
  - Channel 3: set to generic wave. Experiment with forms.
  - Channel 4:
    - Edit screen UI change for drums.
    - Tracker data structure changes for drums. How will three percussives play on one sound channel?
- Clean up. Remove kinks for first POC release.
  - Polyrhythms
    1. Step keeps counting up to #notes * LCM.
    2. Every time step modulo rhythm equals zero for a certain pattern,
       trigger the step divided by rhythm step of that pattern.
  - Play/Pauze/Stop
  - Cursor path between Amplitude and Wave Settings.
- Documentation / Manual
- Github workflows.
  - Action to build grooveboy.
  - Only branches that succeed on that action may be merged into master.
  - Nothing may directly be pushed to master.
  - Merges to master must be done through a PR.
- **<u>RELEASE</u>**
- Play Screen.
  - Edit tracks
  - BPM
  - Start, Stop, Pause
  - Comm actions and feedback?
- Save Screen.
  - List of saved data.
  - Spike compression.
  - Keyboard.
- libsavgba
- **<u>RELEASE</u>**
- Wave Settings?
- Noise Settings?
- Comm
- **<u>RELEASE</u>**
